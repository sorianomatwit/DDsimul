// DDsimul.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SimulatorData.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include <cmath>
#include <algorithm>
#include <set>
#include <random>

struct BattleData {
	DemigodData* unitsData;
	PoolParams* poolData;
	bool* deadList;
};
typedef uint8_t*(*SearchFunc)(Entity*, Entity*, bool*, uint8_t, uint8_t, uint8_t);
typedef void(*AbilityFunc)(Entity*, uint8_t*, uint8_t, BattleData*, Entity*);
struct TargetSearch { 
	uint8_t targetAmt = 1; 
	SearchFunc search;
};
struct AbilityParams {
	uint8_t dmg;
	uint8_t dmgAdd;
	uint8_t hpAdd;
	unsigned short summonId = Sets::MAX_ENTITIES+1;
	AbilityFunc ability;
};


#pragma region Search Systems
uint8_t* ExecuteSelfSearch(Entity* e, Entity* entityList, bool* deadList, uint8_t searchRange, uint8_t beginIndex, uint8_t entityIndex) {
	uint8_t* targets = (uint8_t*)malloc(sizeof(uint8_t));
	targets[0] = entityIndex;
	return targets;
}

uint8_t* ExecuteBehindSearch(Entity* e, Entity* entityList, bool* deadList, uint8_t searchRange, uint8_t beginIndex, uint8_t entityIndex) {
	uint8_t targetAmt = e->GetComponent<TargetSearch>()->targetAmt;
	uint8_t* targets = (uint8_t*)malloc(sizeof(uint8_t) * targetAmt);
	bool reachEntity = false;
	for (uint8_t i = entityIndex+1; i < beginIndex + searchRange && targetAmt > 0; i++) {
		if (!deadList[i]) {
			targets[e->GetComponent<TargetSearch>()->targetAmt - targetAmt] = i;
			targetAmt--;
		}
	}
	return targets;
}

uint8_t* ExecuteRandomSearch(Entity* e, Entity* entityList, bool* deadList, uint8_t searchRange, uint8_t beginIndex, uint8_t entityIndex) {
	uint8_t targetAmt = e->GetComponent<TargetSearch>()->targetAmt;
	uint8_t* targets = (uint8_t*)malloc(sizeof(uint8_t) * targetAmt);
	BitTracker positions;
	uint8_t i = 0;
	while (targetAmt >= 0 && i < searchRange) {
		uint8_t pos = beginIndex + (std::rand() % (searchRange - 0));
		bool isValidPos = (pos != entityIndex && !deadList[pos] && !positions.Has(pos));
		targets[targetAmt - 1] = pos;
		targetAmt -= isValidPos;
		positions.Add(pos);
		i++;
	}
	return targets;
}

uint8_t* ExecuteSummonSearch(Entity* e, Entity* entityList, bool* deadList, uint8_t searchRange, uint8_t beginIndex, uint8_t entityIndex) {
	uint8_t targetAmt = e->GetComponent<TargetSearch>()->targetAmt;
	uint8_t* targets = (uint8_t*)malloc(sizeof(uint8_t) * targetAmt);
	for (uint8_t i = beginIndex; i < beginIndex + searchRange; i++)
	{
		bool hasSummon = entityList[i].HasComponent<Summon>();
		bool isntDead = !deadList[i];
		if (hasSummon && isntDead) {
			targets[e->GetComponent<TargetSearch>()->targetAmt - targetAmt] = i;
			targetAmt--;
		}
	}
	return targets;
}

uint8_t* ExecuteAdjancentSearch(Entity* e, Entity* entityList, bool* deadList, uint8_t searchRange, uint8_t beginIndex, uint8_t entityIndex) {
	uint8_t targetAmt = e->GetComponent<TargetSearch>()->targetAmt;
	uint8_t* targets = (uint8_t*)malloc(sizeof(uint8_t) * targetAmt);
	// 0 = left, 1 = right
	double teamSize = searchRange / 2.0;
	uint8_t* indexes = (uint8_t*)malloc(sizeof(uint8_t) * searchRange);
	for (uint8_t i = beginIndex; i < beginIndex + searchRange; i++)
	{
		if (i < teamSize && std::fmod(teamSize, 1.0) == 0) {
			indexes[i] = teamSize - (i + 1);
		}
		else {
			indexes[i - beginIndex] = i;
		}
	}
	// 0 = left, 1 = right
	for (uint8_t i = beginIndex; i < beginIndex + searchRange; i++)
	{
		if (indexes[i - beginIndex] == entityIndex) {
			for (uint8_t k = i - 1; k >= beginIndex; k--)
			{
				targets[e->GetComponent<TargetSearch>()->targetAmt - targetAmt] = indexes[k - beginIndex];
				bool notDead = !deadList[e->GetComponent<TargetSearch>()->targetAmt - targetAmt];
				targetAmt -= notDead;
				if (notDead) break;
			}
			for (uint8_t k = i + 1; k < beginIndex + searchRange; k--)
			{
				targets[e->GetComponent<TargetSearch>()->targetAmt - targetAmt] = indexes[k - beginIndex];
				bool notDead = !deadList[indexes[k - beginIndex]];
				targetAmt -= notDead;
				if (notDead) break;
			}
			break;
		}
	}
	free(indexes);
	return targets;
}

uint8_t* ExecuteBackOfLineupSearch(Entity* e, Entity* entityList, bool* deadList, uint8_t searchRange, uint8_t beginIndex, uint8_t entityIndex) {
	uint8_t targetAmt = e->GetComponent<TargetSearch>()->targetAmt;
	uint8_t* targets = (uint8_t*)malloc(sizeof(uint8_t) * targetAmt);
	targets[0] = beginIndex + searchRange - 1;

	uint8_t i = beginIndex + searchRange - 1;
	while (deadList[targets[0]] && i > beginIndex) {
		i--;
		targets[0] = i;
	}
	targets[0] = deadList[targets[0]] ? (uint8_t)malloc(sizeof(uint8_t) * targetAmt) : targets[0];
	return targets;
}

uint8_t* ExecuteFrontOfLineupSearch(Entity* e, Entity* entityList, bool* deadList, uint8_t searchRange, uint8_t beginIndex, uint8_t entityIndex) {
	uint8_t targetAmt = e->GetComponent<TargetSearch>()->targetAmt;
	uint8_t* targets = (uint8_t*)malloc(sizeof(uint8_t) * targetAmt);
	targets[0] = beginIndex;

	uint8_t i = beginIndex;
	while (deadList[targets[0]] && i < beginIndex + searchRange) {
		i++;
		targets[0] = i;
	}
	targets[0] = deadList[targets[0]]? (uint8_t)malloc(sizeof(uint8_t) * targetAmt) : targets[0];
	return targets;
}

uint8_t* ExecuteSecondInOfLineupSearch(Entity* e, Entity* entityList, bool* deadList, uint8_t searchRange, uint8_t beginIndex, uint8_t entityIndex) {
	uint8_t targetAmt = e->GetComponent<TargetSearch>()->targetAmt;
	uint8_t* targets = (uint8_t*)malloc(sizeof(uint8_t) * targetAmt);
	targets[0] = beginIndex + searchRange - 2;

	uint8_t i = beginIndex + searchRange - 2;
	while (deadList[targets[0]] && i > beginIndex) {
		i--;
		targets[0] = i;
	}
	targets[0] = deadList[targets[0]] ? (uint8_t)malloc(sizeof(uint8_t) * targetAmt) : targets[0];
	return targets;
}

uint8_t* ExecuteHighestHealthSearch(Entity* e, Entity* entityList, bool* deadList, uint8_t searchRange, uint8_t beginIndex, uint8_t entityIndex) {
	uint8_t targetAmt = e->GetComponent<TargetSearch>()->targetAmt;
	uint8_t* targets = (uint8_t*)malloc(sizeof(uint8_t) * targetAmt);
	targets[0] = beginIndex;
	uint8_t max = entityList[targets[0]].GetComponent<DemigodData>()->hp;
	for (uint8_t i = beginIndex; i < beginIndex + searchRange; i++)
	{
		bool isGreater = max < entityList[i].GetComponent<DemigodData>()->hp;
		max = (isGreater) ? entityList[i].GetComponent<DemigodData>()->hp : max;
		targets[0] = (isGreater && !deadList[i]) ? i : targets[0];
	}
	targets[0] = deadList[targets[0]] ? (uint8_t)malloc(sizeof(uint8_t) * targetAmt) : targets[0];
	return targets;
}

uint8_t* ExecuteHighestAttackSearch(Entity* e, Entity* entityList, bool* deadList, uint8_t searchRange, uint8_t beginIndex, uint8_t entityIndex){
	uint8_t targetAmt = e->GetComponent<TargetSearch>()->targetAmt;
	uint8_t* targets = (uint8_t*)malloc(sizeof(uint8_t) * targetAmt);
	targets[0] = beginIndex;
	uint8_t max = entityList[targets[0]].GetComponent<DemigodData>()->atk;
	for (uint8_t i = beginIndex; i < beginIndex + searchRange; i++)
	{
		bool isGreater = max < entityList[i].GetComponent<DemigodData>()->atk;
		max = (isGreater) ? entityList[i].GetComponent<DemigodData>()->atk : max;
		targets[0] = (isGreater && !deadList[i]) ? i : targets[0];
	}
	targets[0] = deadList[targets[0]] ? (uint8_t)malloc(sizeof(uint8_t) * targetAmt) : targets[0];
	return targets;
}

uint8_t* ExecuteLowestHealthSearch(Entity* e, Entity* entityList, bool* deadList, uint8_t searchRange, uint8_t beginIndex, uint8_t entityIndex) {
	uint8_t targetAmt = e->GetComponent<TargetSearch>()->targetAmt;
	uint8_t* targets = (uint8_t*)malloc(sizeof(uint8_t) * targetAmt);
	targets[0] = beginIndex;
	uint8_t low = entityList[targets[0]].GetComponent<DemigodData>()->hp;
	for (uint8_t i = beginIndex; i < beginIndex + searchRange; i++)
	{
		bool isLower = low > entityList[i].GetComponent<DemigodData>()->hp;
		low = (isLower) ? entityList[i].GetComponent<DemigodData>()->hp : low;
		targets[0] = (isLower && !deadList[i]) ? i : targets[0];
	}
	targets[0] = deadList[targets[0]] ? (uint8_t)malloc(sizeof(uint8_t) * targetAmt) : targets[0];
	return targets;
}

uint8_t* ExecuteLowestAttackSearch(Entity* e, Entity* entityList, bool* deadList, uint8_t searchRange, uint8_t beginIndex, uint8_t entityIndex) {
	uint8_t targetAmt = e->GetComponent<TargetSearch>()->targetAmt;
	uint8_t* targets = (uint8_t*)malloc(sizeof(uint8_t) * targetAmt);
	targets[0] = beginIndex;
	uint8_t low = entityList[targets[0]].GetComponent<DemigodData>()->atk;
	for (uint8_t i = beginIndex; i < beginIndex + searchRange; i++)
	{
		bool isLower = low > entityList[i].GetComponent<DemigodData>()->atk;
		low = (isLower) ? entityList[i].GetComponent<DemigodData>()->atk : low;
		targets[0] = (isLower && !deadList[i]) ? i : targets[0];
	}
	targets[0] = deadList[targets[0]] ? (uint8_t)malloc(sizeof(uint8_t) * targetAmt) : targets[0];
	return targets;
}
#pragma endregion

#pragma region Ability System

void ExecuteBuffAbility(Entity* e, uint8_t* targets, uint8_t targetAmount, BattleData* battleData, Entity* units) {
	AbilityParams* buff = e->GetComponent<AbilityParams>();
	std::cout << "Buff\n";
	for (uint8_t i = 0; i < targetAmount && targets[i] < EntityManager::entityCount; i++)
	{
		DemigodData* data = &(battleData->unitsData[targets[i]]);
		data->atk = (data->atk + buff->dmgAdd < 0)? 0 : data->atk + buff->dmgAdd;
		data->hp = (data->hp + buff->hpAdd <= 0) ? 1 : data->hp + buff->hpAdd;
	}
}


void ExecuteSummonAbility(Entity* e, uint8_t* targets, uint8_t targetAmount, BattleData* battleData, Entity* units) {
	std::cout << "Summon\n";
	AbilityParams* summon = e->GetComponent<AbilityParams>();
	for (uint8_t i = 0; i < targetAmount && targets[i] < EntityManager::entityCount; i++)
	{
		battleData->unitsData[targets[i]] = *(Entity(summon->summonId).GetComponent<DemigodData>());
		units[targets[i]] = Entity(summon->summonId);
		battleData->deadList[targets[i]] = false;
	}
}

void Ability(Entity* e, Entity* units, BattleData* battleData, PoolParams pool, uint8_t unitIndex) {
	SearchFunc search = e->GetComponent<TargetSearch>()->search;
	AbilityFunc ability = e->GetComponent<AbilityParams>()->ability;
	//PoolParams* pool = e->GetComponent<PoolParams>();
	uint8_t tAmt = e->GetComponent<TargetSearch>()->targetAmt;

	uint8_t* targets = (*search)(e, units, battleData->deadList, pool.poolSize, pool.begin,unitIndex);
	(*ability)(e, targets, tAmt, battleData, units);
	//free(targets);
}

void DealDamage(uint8_t unitIndex, uint8_t Damage, BattleData* battleData, Entity* units) {
	DemigodData* unitData = &(battleData->unitsData[unitIndex]);
	unitData->hp = (unitData->hp - Damage < 0)? 0 : unitData->hp- Damage;
	if (unitData->hp <= 0) {
		battleData->deadList[unitIndex] = true;
	}
	if (units[unitIndex].HasComponent<OnDamagedTrigger>() && unitData->hp > 0) {
		Ability(&units[unitIndex], units, battleData, battleData->poolData[unitIndex], unitIndex);
	}
}

void ExecuteDamageAbility(Entity* e, uint8_t* targets, uint8_t targetAmount, BattleData* battleData, Entity* units) {
	std::cout << "Damage\n";
	AbilityParams* damage = e->GetComponent<AbilityParams>();
	for (uint8_t i = 0; i < targetAmount && targets[i] < EntityManager::entityCount; i++)
	{
		DemigodData* data = &(battleData->unitsData[targets[i]]);
		DealDamage(targets[0], damage->dmg, battleData, units);
	}
}
#pragma endregion

void SetupComponents() {
	ComponentManager::CreateComponentSet<DemigodData>();
	ComponentManager::CreateComponentSet<Stats>();
	ComponentManager::CreateComponentSet<Summon>();
	ComponentManager::CreateComponentSet<Summoner>();

	ComponentManager::CreateComponentSet<TargetSearch>();

	ComponentManager::CreateComponentSet<PreAttackTrigger>();
	ComponentManager::CreateComponentSet<OnDamagedTrigger>();
	ComponentManager::CreateComponentSet<PostFaintTrigger>();
	ComponentManager::CreateComponentSet<PreMatchTrigger>();
	ComponentManager::CreateComponentSet<OnSummonTrigger>();

	ComponentManager::CreateComponentSet<PoolParams>();

	ComponentManager::CreateComponentSet<AbilityParams>();
}

void SetupDemigods() {
	DemigodData* data;
	AbilityParams* abil;
	#pragma region Zeus 0
		// Zeus
		Entity* Zeus = EntityManager::CreateEntity();
		data = Zeus->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 2;
		Zeus->AddComponent<Stats>()->name = "Zeus_DamageERD1(T2)";
		abil = Zeus->AddComponent<AbilityParams>();
		abil->dmg = 1; abil->ability =ExecuteDamageAbility;
		Zeus->AddComponent<PoolParams>()->target = ENEMY;
		Zeus->AddComponent<TargetSearch>()->targetAmt = 2;
		Zeus->GetComponent<TargetSearch>()->search = ExecuteRandomSearch;
		Zeus->AddComponent<OnDamagedTrigger>();

		Zeus = EntityManager::CreateEntity();
		data = Zeus->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 2;
		Zeus->AddComponent<Stats>()->name = "Zeus_DamageERD1(T2)";
		abil = Zeus->AddComponent<AbilityParams>();
		abil->hpAdd = 1; abil->ability = ExecuteBuffAbility;
		Zeus->AddComponent<PoolParams>()->target = TEAM;
		Zeus->AddComponent<TargetSearch>()->search = ExecuteRandomSearch;
		Zeus->AddComponent<PreMatchTrigger>();

		Zeus = EntityManager::CreateEntity();
		data = Zeus->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 2;
		Zeus->AddComponent<Stats>()->name = "Zeus_DamageARF1(T4)";
		abil = Zeus->AddComponent<AbilityParams>();
		abil->dmg = 1; abil->ability = ExecuteDamageAbility;
		Zeus->AddComponent<PoolParams>()->target = ALL;
		Zeus->AddComponent<TargetSearch>()->targetAmt = 4;
		Zeus->GetComponent<TargetSearch>()->search = ExecuteRandomSearch;
		Zeus->AddComponent<PostFaintTrigger>();

		Zeus = EntityManager::CreateEntity();
		data = Zeus->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 2;
		Zeus->AddComponent<Stats>()->name = "Zeus_BuffPLAD01";
		abil = Zeus->AddComponent<AbilityParams>();
		abil->dmgAdd = 1;abil->ability = ExecuteBuffAbility;
		Zeus->AddComponent<PoolParams>()->target = TEAM;
		Zeus->AddComponent<TargetSearch>()->search = ExecuteLowestAttackSearch;
		Zeus->AddComponent<OnDamagedTrigger>();
	#pragma endregion     

	#pragma region Hestia_1
		// Hestia
		Entity* Hestia = EntityManager::CreateEntity();
		data = Hestia->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 3;
		Hestia->AddComponent<Stats>()->name = "Hestia_BuffAAM11";
		abil = Hestia->AddComponent<AbilityParams>();
		abil->dmgAdd = 1; abil->hpAdd = 1;abil->ability = ExecuteBuffAbility;
		Hestia->AddComponent<PoolParams>()->target = ALL;
		Hestia->AddComponent<TargetSearch>()->search = ExecuteAdjancentSearch;
		Hestia->AddComponent<PreMatchTrigger>();

		Hestia = EntityManager::CreateEntity();
		data = Hestia->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 3;
		Hestia->AddComponent<Stats>()->name = "Hestia_BuffPFLM12";
		abil = Hestia->AddComponent<AbilityParams>();
		abil->dmgAdd = 1; abil->hpAdd = 2;abil->ability = ExecuteBuffAbility;
		Hestia->AddComponent<PoolParams>()->target = TEAM;
		Hestia->AddComponent<TargetSearch>()->search = ExecuteFrontOfLineupSearch;
		Hestia->AddComponent<PreMatchTrigger>();

		Hestia = EntityManager::CreateEntity();
		data = Hestia->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 3;
		Hestia->AddComponent<Stats>()->name = "Hestia_BuffPBF20";
		abil = Hestia->AddComponent<AbilityParams>();
		abil->dmgAdd = 2;abil->ability = ExecuteDamageAbility;
		Hestia->AddComponent<PoolParams>()->target = TEAM;
		Hestia->AddComponent<TargetSearch>()->search = ExecuteBehindSearch;
		Hestia->AddComponent<PostFaintTrigger>();

		Hestia = EntityManager::CreateEntity();
		data = Hestia->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 3;
		Hestia->AddComponent<Stats>()->name = "Hestia_DamageAHHM1";
		abil = Hestia->AddComponent<AbilityParams>();
		abil->dmg = 1; abil->ability = ExecuteDamageAbility;
		Hestia->AddComponent<PoolParams>()->target = ALL;
		Hestia->AddComponent<TargetSearch>()->search = ExecuteHighestHealthSearch;
		Hestia->AddComponent<PreMatchTrigger>();
	#pragma endregion

	#pragma region Poseidon_2
		// Poseidon
		Entity* Poseidon = EntityManager::CreateEntity();
		data = Poseidon->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 3;
		Poseidon->AddComponent<Stats>()->name = "Poseidon_BuffPBA11";
		abil = Poseidon->AddComponent<AbilityParams>();
		abil->dmgAdd = 1; abil->hpAdd = 1;abil->ability = ExecuteBuffAbility;
		Poseidon->AddComponent<PoolParams>()->target = TEAM;
		Poseidon->AddComponent<TargetSearch>()->search = ExecuteBehindSearch;
		Poseidon->AddComponent<PreAttackTrigger>();

		Poseidon = EntityManager::CreateEntity();
		data = Poseidon->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 3;
		Poseidon->AddComponent<Stats>()->name = "Poseidon_BuffPLHF20";
		abil = Poseidon->AddComponent<AbilityParams>();
		abil->dmgAdd = 2; abil->ability = ExecuteBuffAbility;
		Poseidon->AddComponent<PoolParams>()->target = TEAM;
		Poseidon->AddComponent<TargetSearch>()->search = ExecuteLowestHealthSearch;
		Poseidon->AddComponent<PostFaintTrigger>();

		Poseidon = EntityManager::CreateEntity();
		data = Poseidon->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 3;
		Poseidon->AddComponent<Stats>()->name = "Poseidon_DamageESLA1";
		abil = Poseidon->AddComponent<AbilityParams>();
		abil->dmg = 1;abil->ability = ExecuteDamageAbility;
		Poseidon->AddComponent<PoolParams>()->target = ENEMY;
		Poseidon->AddComponent<TargetSearch>()->search = ExecuteSecondInOfLineupSearch;
		Poseidon->AddComponent<PreAttackTrigger>();

		Poseidon = EntityManager::CreateEntity();
		data = Poseidon->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 3;
		Poseidon->AddComponent<Stats>()->name = "Poseidon_BuffPHHM20";
		abil = Poseidon->AddComponent<AbilityParams>();
		abil->dmgAdd = 2;abil->ability = ExecuteBuffAbility;
		Poseidon->AddComponent<PoolParams>()->target = TEAM;
		Poseidon->AddComponent<TargetSearch>()->search = ExecuteHighestHealthSearch;
		Poseidon->AddComponent<PreMatchTrigger>();
	#pragma endregion

	#pragma region Hermes_3
		// Hermes
		Entity* Hermes = EntityManager::CreateEntity();
		data = Hermes->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 1;
		Hermes->AddComponent<Stats>()->name = "Hermes_DamageERM1(T2)";
		abil = Hermes->AddComponent<AbilityParams>();
		abil->dmg = 1;abil->ability = ExecuteDamageAbility;
		Hermes->AddComponent<PoolParams>()->target = ENEMY;
		Hermes->AddComponent<TargetSearch>()->targetAmt = 2;
		Hermes->GetComponent<TargetSearch>()->search = ExecuteRandomSearch;
		Hermes->AddComponent<PreMatchTrigger>();

		Hermes = EntityManager::CreateEntity();
		data = Hermes->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 1;
		Hermes->AddComponent<Stats>()->name = "Hermes_DamageAAF2";
		abil = Hermes->AddComponent<AbilityParams>();
		abil->dmg = 2; abil->ability = ExecuteDamageAbility;
		Hermes->AddComponent<PoolParams>()->target = ENEMY;
		Hermes->AddComponent<TargetSearch>()->search = ExecuteAdjancentSearch;
		Hermes->AddComponent<PostFaintTrigger>();

		Hermes = EntityManager::CreateEntity();
		data = Hermes->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 1;
		Hermes->AddComponent<Stats>()->name = "Hermes_BuffERF-20(T1)";
		abil = Hermes->AddComponent<AbilityParams>();
		abil->dmgAdd = -2;abil->ability = ExecuteBuffAbility;
		Hermes->AddComponent<PoolParams>()->target = ENEMY;
		Hermes->AddComponent<TargetSearch>()->search = ExecuteRandomSearch;
		Hermes->AddComponent<PostFaintTrigger>();

		Hermes = EntityManager::CreateEntity();
		data = Hermes->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 1;
		Hermes->AddComponent<Stats>()->name = "Hermes_DamageEFLF1";
		abil = Hermes->AddComponent<AbilityParams>();
		abil->dmg = 1; abil->ability = ExecuteDamageAbility;
		Hermes->AddComponent<PoolParams>()->target = TEAM;
		Hermes->AddComponent<TargetSearch>()->search = ExecuteFrontOfLineupSearch;
		Hermes->AddComponent<PostFaintTrigger>();
	#pragma endregion

	#pragma region Hera_4
		// Hera
		Entity* Hera = EntityManager::CreateEntity();
		data = Hera->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 1;
		Hera->AddComponent<Stats>()->name = "Hera_DamageEHAM2";
		abil = Hera->AddComponent<AbilityParams>();
		abil->dmg = 2;abil->ability = ExecuteDamageAbility;
		Hera->AddComponent<PoolParams>()->target = ENEMY;
		Hera->AddComponent<TargetSearch>()->search = ExecuteHighestAttackSearch;
		Hera->AddComponent<PreMatchTrigger>();

		Hera = EntityManager::CreateEntity();
		data = Hera->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 1;
		Hera->AddComponent<Stats>()->name = "Hera_BuffAAM-10";
		Hera->AddComponent<AbilityParams>();
		abil->dmgAdd = -1;abil->ability = ExecuteBuffAbility;
		Hera->AddComponent<PoolParams>()->target = ALL;
		Hera->AddComponent<TargetSearch>()->search = ExecuteAdjancentSearch;
		Hera->AddComponent<PreMatchTrigger>();

		Hera = EntityManager::CreateEntity();
		data = Hera->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 1;
		Hera->AddComponent<Stats>()->name = "Hera_DamageAAM1";
		abil = Hera->AddComponent<AbilityParams>();
		abil->dmg = 1; abil->ability = ExecuteDamageAbility;
		Hera->AddComponent<PoolParams>()->target = ALL;
		Hera->AddComponent<TargetSearch>()->search = ExecuteAdjancentSearch;
		Hera->AddComponent<PreMatchTrigger>();

		Hera = EntityManager::CreateEntity();
		data = Hera->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 1;
		Hera->AddComponent<Stats>()->name = "Hera_BuffPAF01";
		abil = Hera->AddComponent<AbilityParams>();
		abil->hpAdd = 1;abil->ability = ExecuteBuffAbility;
		Hera->AddComponent<PoolParams>()->target = TEAM;
		Hera->AddComponent<TargetSearch>()->search = ExecuteAdjancentSearch;
		Hera->AddComponent<PostFaintTrigger>();
	#pragma endregion

	#pragma region Hephaestus_5
		// Hephaestus
		Entity* Hephaestus = EntityManager::CreateEntity();
		data = Hephaestus->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 4;
		Hephaestus->AddComponent<Stats>()->name = "Hephaestus_BuffPBLM-13";
		abil = Hephaestus->AddComponent<AbilityParams>();
		abil->dmgAdd = -1; abil->hpAdd = 3;abil->ability = ExecuteBuffAbility;
		Hephaestus->AddComponent<PoolParams>()->target = TEAM;
		Hephaestus->AddComponent<TargetSearch>()->search = ExecuteBackOfLineupSearch;
		Hephaestus->AddComponent<PreMatchTrigger>();

		Hephaestus = EntityManager::CreateEntity();
		data = Hephaestus->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 4;
		Hephaestus->AddComponent<Stats>()->name = "Hephaestus_DamagePBM1";
		abil = Hephaestus->AddComponent<AbilityParams>();
		abil->dmg = 1;abil->ability = ExecuteDamageAbility;
		Hephaestus->AddComponent<PoolParams>()->target = TEAM;
		Hephaestus->AddComponent<TargetSearch>()->search = ExecuteBehindSearch;
		Hephaestus->AddComponent<PreMatchTrigger>();

		Hephaestus = EntityManager::CreateEntity();
		data = Hephaestus->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 4;
		Hephaestus->AddComponent<Stats>()->name = "Hephaestus_BuffELHF-31";
		abil = Hephaestus->AddComponent<AbilityParams>();
		abil->dmgAdd = -3; abil->hpAdd = 1;abil->ability = ExecuteBuffAbility;
		Hephaestus->AddComponent<PoolParams>()->target = ENEMY;
		Hephaestus->AddComponent<TargetSearch>()->search = ExecuteLowestHealthSearch;
		Hephaestus->AddComponent<PostFaintTrigger>();

		Hephaestus = EntityManager::CreateEntity();
		data = Hephaestus->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 4;
		Hephaestus->AddComponent<Stats>()->name = "Hephaestus_DamageERU1(T2)";
		abil = Hephaestus->AddComponent<AbilityParams>();
		abil->dmg = 1;abil->ability = ExecuteDamageAbility;
		Hephaestus->AddComponent<PoolParams>()->target = ENEMY;
		Hephaestus->AddComponent<TargetSearch>()->targetAmt = 2;
		Hephaestus->GetComponent<TargetSearch>()->search = ExecuteBackOfLineupSearch;;
		Hephaestus->AddComponent<OnSummonTrigger>();
	#pragma endregion

	#pragma region Hades_6, cerberus_7
		// Hades
		Entity* Hades = EntityManager::CreateEntity();
		data = Hades->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 2;
		Hades->AddComponent<Stats>()->name = "Hades_SummonCerberus";
		Hades->AddComponent<Summoner>();

		abil = Hades->AddComponent<AbilityParams>();
		Entity* cerberus = EntityManager::CreateEntity();
		cerberus->AddComponent<Summon>()->ownerId = Hades->_id;
		cerberus->AddComponent<Stats>();
		data = cerberus->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 2;
		abil->summonId = cerberus->_id;
		abil->ability = ExecuteSummonAbility;

		Hades->AddComponent<PoolParams>()->target = TEAM;
		Hades->AddComponent<TargetSearch>()->search = ExecuteSelfSearch;
		Hades->AddComponent<PostFaintTrigger>();

		Hades = EntityManager::CreateEntity();
		data = Hades->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 2;
		Hades->AddComponent<Stats>()->name = "Hades_BuffAAM10";
		abil = Hades->AddComponent<AbilityParams>();
		abil->dmgAdd = 1; abil->ability = ExecuteDamageAbility;
		Hades->AddComponent<PoolParams>()->target = ALL;
		Hades->AddComponent<TargetSearch>()->search = ExecuteAdjancentSearch;
		Hades->AddComponent<PreMatchTrigger>();

		Hades = EntityManager::CreateEntity();
		data = Hades->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 2;
		Hades->AddComponent<Stats>()->name = "Hades_DamageAAF1";
		abil = Hades->AddComponent<AbilityParams>();
		abil->dmg = 1; abil->ability = ExecuteDamageAbility;
		Hades->AddComponent<PoolParams>()->target = ALL;
		Hades->AddComponent<TargetSearch>()->search = ExecuteAdjancentSearch;
		Hades->AddComponent<PostFaintTrigger>();

		Hades = EntityManager::CreateEntity();
		data = Hades->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 2;
		Hades->AddComponent<Stats>()->name = "Hades_DamageERF1(T2)";
		abil = Hades->AddComponent<AbilityParams>();
		abil->dmg = 1;abil->ability = ExecuteDamageAbility;
		Hades->AddComponent<PoolParams>()->target = ENEMY;
		Hades->AddComponent<TargetSearch>()->targetAmt = 2;
		Hades->GetComponent<TargetSearch>()->search = ExecuteRandomSearch;
		Hades->AddComponent<PostFaintTrigger>();
	#pragma endregion    

	#pragma region Dionysus_8
		// Dionysus
		Entity* Dionysus = EntityManager::CreateEntity();
		data = Dionysus->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 2;
		Dionysus->AddComponent<Stats>()->name = "Dionysus_DamageEHHF3";
		abil = Dionysus->AddComponent<AbilityParams>();
		abil->dmg = 3;abil->ability = ExecuteDamageAbility;
		Dionysus->AddComponent<PoolParams>()->target = ENEMY;
		Dionysus->AddComponent<TargetSearch>()->search = ExecuteHighestHealthSearch;
		Dionysus->AddComponent<PostFaintTrigger>();

		Dionysus = EntityManager::CreateEntity();
		data = Dionysus->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 2;
		Dionysus->AddComponent<Stats>()->name = "Dionysus_BuffPSM21";
		abil = Dionysus->AddComponent<AbilityParams>();
		abil->dmgAdd = 1; abil->hpAdd = 1;abil->ability = ExecuteBuffAbility;
		Dionysus->AddComponent<PoolParams>()->target = TEAM;
		Dionysus->AddComponent<TargetSearch>()->search = ExecuteSelfSearch;
		Dionysus->AddComponent<PreMatchTrigger>();

		Dionysus = EntityManager::CreateEntity();
		data = Dionysus->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 2;
		Dionysus->AddComponent<Stats>()->name = "Dionysus_BuffPSM21";
		abil = Dionysus->AddComponent<AbilityParams>();
		abil->dmgAdd = -2;abil->ability = ExecuteBuffAbility;
		Dionysus->AddComponent<PoolParams>()->target = ENEMY;
		Dionysus->AddComponent<TargetSearch>()->search = ExecuteHighestAttackSearch;
		Dionysus->AddComponent<PreMatchTrigger>();

		Dionysus = EntityManager::CreateEntity();
		data = Dionysus->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 2;
		Dionysus->AddComponent<Stats>()->name = "Dionysus_BuffPSM21";
		abil = Dionysus->AddComponent<AbilityParams>();
		abil->dmgAdd = 1; abil->hpAdd = 1; abil->ability = ExecuteBuffAbility;
		Dionysus->AddComponent<PoolParams>()->target = TEAM;
		Dionysus->AddComponent<TargetSearch>()->search = ExecuteSelfSearch;
		Dionysus->AddComponent<OnSummonTrigger>();
	#pragma endregion

	#pragma region Demeter_8, cornucopia_9
		// Demeter
		Entity* Demeter = EntityManager::CreateEntity();
		data = Demeter->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 3;
		Demeter->AddComponent<Stats>()->name = "Demeter_BuffPRF11(T3)";
		abil = Demeter->AddComponent<AbilityParams>();
		abil->dmgAdd = 1; abil->hpAdd = 1;abil->ability = ExecuteBuffAbility;
		Demeter->AddComponent<PoolParams>()->target = TEAM;
		Demeter->AddComponent<TargetSearch>()->targetAmt = 3;
		Demeter->GetComponent<TargetSearch>()->search = ExecuteRandomSearch;
		Demeter->AddComponent<PostFaintTrigger>();

		Demeter = EntityManager::CreateEntity();
		data = Demeter->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 3;
		Demeter->AddComponent<Stats>()->name = "Demeter_BuffPUS11";
		abil = Demeter->AddComponent<AbilityParams>();
		abil->dmgAdd = 1; abil->hpAdd = 1;abil->ability = ExecuteBuffAbility;
		Demeter->AddComponent<PoolParams>()->target = TEAM;
		Demeter->AddComponent<TargetSearch>()->targetAmt = 5;
		Demeter->GetComponent<TargetSearch>()->search = ExecuteSummonSearch;
		Demeter->AddComponent<OnSummonTrigger>();

		Demeter = EntityManager::CreateEntity();
		data = Demeter->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 3;
		Demeter->AddComponent<Stats>()->name = "Demeter_SummonCornucopia";
		Demeter->AddComponent<Summoner>();

		abil = Demeter->AddComponent<AbilityParams>();
		Entity* corncopia = EntityManager::CreateEntity();
		corncopia->AddComponent<Summon>()->ownerId = Demeter->_id;
		corncopia->AddComponent<Stats>();
		data = corncopia->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 3;
		abil->summonId = corncopia->_id;
		abil->ability = ExecuteSummonAbility;

		Demeter->AddComponent<PoolParams>()->target = TEAM;
		Demeter->AddComponent<TargetSearch>()->search = ExecuteSelfSearch;
		Demeter->AddComponent<PostFaintTrigger>();

		Demeter = EntityManager::CreateEntity();
		data = Demeter->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 3;
		Demeter->AddComponent<Stats>()->name = "Demeter_DamageEBLU1";
		abil = Demeter->AddComponent<AbilityParams>();
		abil->dmg = 1;abil->ability = ExecuteDamageAbility;
		Demeter->AddComponent<PoolParams>()->target = ENEMY;
		Demeter->AddComponent<TargetSearch>()->search = ExecuteBackOfLineupSearch;
		Demeter->AddComponent<OnSummonTrigger>();
	#pragma endregion

	#pragma region Athena_10,medusa_11, arachne_12
		// Athena
		Entity* Athena = EntityManager::CreateEntity();
		data = Athena->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 3;
		Athena->AddComponent<Stats>()->name = "Athena_SummonMedusa";
		Athena->AddComponent<Summoner>();

		abil = Athena->AddComponent<AbilityParams>();
		Entity* medusa = EntityManager::CreateEntity();
		medusa->AddComponent<Summon>()->ownerId = Athena->_id;
		medusa->AddComponent<Stats>();
		data = medusa->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 4;
		abil->summonId = medusa->_id;
		abil->ability = ExecuteSummonAbility;

		Athena->AddComponent<PoolParams>()->target = TEAM;
		Athena->AddComponent<TargetSearch>()->search = ExecuteSelfSearch;
		Athena->AddComponent<PostFaintTrigger>();

		Athena = EntityManager::CreateEntity();
		data = Athena->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 3;
		Athena->AddComponent<Stats>()->name = "Athena_DamageEHAF1";
		abil = Athena->AddComponent<AbilityParams>();
		abil->dmg = 1;abil->ability = ExecuteDamageAbility;
		Athena->AddComponent<PoolParams>()->target = ENEMY;
		Athena->AddComponent<TargetSearch>()->search = ExecuteLowestAttackSearch;
		Athena->AddComponent<PostFaintTrigger>();

		Athena = EntityManager::CreateEntity();
		data = Athena->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 3;
		Athena->AddComponent<Stats>()->name = "Athena_SummonArachne";
		Athena->AddComponent<Summoner>();

		abil = Athena->AddComponent<AbilityParams>();
		Entity* arachne = EntityManager::CreateEntity();
		arachne->AddComponent<Summon>()->ownerId = Athena->_id;
		arachne->AddComponent<Stats>();
		data = arachne->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 3;
		abil->summonId = arachne->_id;
		abil->ability = ExecuteSummonAbility;

		Athena->AddComponent<PoolParams>()->target = ENEMY;
		Athena->AddComponent<TargetSearch>()->search = ExecuteRandomSearch;
		Athena->AddComponent<PostFaintTrigger>();

		Athena = EntityManager::CreateEntity();
		data = Athena->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 3;
		Athena->AddComponent<Stats>()->name = "Athena_BuffPBD10";
		abil = Athena->AddComponent<AbilityParams>();
		abil->dmgAdd = 1;abil->ability = ExecuteBuffAbility;
		Athena->AddComponent<PoolParams>()->target = TEAM;
		Athena->AddComponent<TargetSearch>()->search = ExecuteBehindSearch;
		Athena->AddComponent<OnDamagedTrigger>();
	#pragma endregion

	#pragma region Artemis_13, manticore_14
		// Artemis
		Entity* Artemis = EntityManager::CreateEntity();
		data = Artemis->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 2;
		Artemis->AddComponent<Stats>()->name = "Artemis_BuffPBF02(T2)";
		abil = Artemis->AddComponent<AbilityParams>();
		abil->hpAdd = 2; abil->ability = ExecuteBuffAbility;
		Artemis->AddComponent<PoolParams>()->target = TEAM;
		Artemis->AddComponent<TargetSearch>()->search = ExecuteBehindSearch;
		Artemis->AddComponent<PostFaintTrigger>();

		Artemis = EntityManager::CreateEntity();
		data = Artemis->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 2;
		Artemis->AddComponent<Stats>()->name = "Artemis_DamageEFLM2";
		abil = Artemis->AddComponent<AbilityParams>();
		abil->dmg = 2;abil->ability = ExecuteDamageAbility;
		Artemis->AddComponent<PoolParams>()->target = ENEMY;
		Artemis->AddComponent<TargetSearch>()->search = ExecuteFrontOfLineupSearch;
		Artemis->AddComponent<PreMatchTrigger>();

		Artemis = EntityManager::CreateEntity();
		data = Artemis->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 2;
		Artemis->AddComponent<Stats>()->name = "Artemis_DamageERS1(T1)";
		abil = Artemis->AddComponent<AbilityParams>();
		abil->dmg = 1;abil->ability = ExecuteDamageAbility;
		Artemis->AddComponent<PoolParams>()->target = ENEMY;
		Artemis->AddComponent<TargetSearch>()->search = ExecuteRandomSearch;
		Artemis->AddComponent<OnSummonTrigger>();

		Artemis = EntityManager::CreateEntity();
		data = Artemis->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 2;
		Artemis->AddComponent<Stats>()->name = "Artemis_SummonManticore";
		Artemis->AddComponent<Summoner>();

		abil = Artemis->AddComponent<AbilityParams>();
		Entity* manticore = EntityManager::CreateEntity();
		data = manticore->AddComponent<DemigodData>();
		manticore->AddComponent<Summon>()->ownerId = Artemis->_id;
		manticore->AddComponent<Stats>();
		data->atk = 1; data->hp = 1;
		abil->summonId = manticore->_id;
		abil->ability = ExecuteSummonAbility;

		Artemis->AddComponent<PoolParams>()->target = TEAM;
		Artemis->AddComponent<TargetSearch>()->search = ExecuteSelfSearch;
		Artemis->AddComponent<PostFaintTrigger>();
	#pragma endregion

	#pragma region Ares_15
		// Ares
		Entity* Ares = EntityManager::CreateEntity();
		data = Ares->AddComponent<DemigodData>();
		data->atk = 5; data->hp = 2;
		Ares->AddComponent<Stats>()->name = "Ares_DamageEBLM1";
		abil = Ares->AddComponent<AbilityParams>();
		abil->dmg = 1;abil->ability = ExecuteDamageAbility;
		Ares->AddComponent<PoolParams>()->target = ENEMY;
		Ares->AddComponent<TargetSearch>()->search = ExecuteBackOfLineupSearch;
		Ares->AddComponent<PreMatchTrigger>();

		Ares = EntityManager::CreateEntity();
		data = Ares->AddComponent<DemigodData>();
		data->atk = 5; data->hp = 2;
		Ares->AddComponent<Stats>()->name = "Ares_BuffEBLD-10";
		abil = Ares->AddComponent<AbilityParams>();
		abil->dmgAdd = -1;abil->ability = ExecuteBuffAbility;
		Ares->AddComponent<PoolParams>()->target = ENEMY;
		Ares->AddComponent<TargetSearch>()->search = ExecuteBackOfLineupSearch;
		Ares->AddComponent<OnDamagedTrigger>();

		Ares = EntityManager::CreateEntity();
		data = Ares->AddComponent<DemigodData>();
		data->atk = 5; data->hp = 2;
		Ares->AddComponent<Stats>()->name = "Ares_DamageEFLF2";
		abil = Ares->AddComponent<AbilityParams>();
		abil->dmg = 2;abil->ability = ExecuteDamageAbility;
		Ares->AddComponent<PoolParams>()->target = ENEMY;
		Ares->AddComponent<TargetSearch>()->search = ExecuteFrontOfLineupSearch;
		Ares->AddComponent<PostFaintTrigger>();

		Ares = EntityManager::CreateEntity();
		data = Ares->AddComponent<DemigodData>();
		data->atk = 5; data->hp = 2;
		Ares->AddComponent<Stats>()->name = "Ares_BuffPBD01";
		abil = Ares->AddComponent<AbilityParams>();
		abil->hpAdd = 1;abil->ability = ExecuteBuffAbility;
		Ares->AddComponent<PoolParams>()->target = TEAM;
		Ares->AddComponent<TargetSearch>()->search = ExecuteBehindSearch;
		Ares->AddComponent<OnDamagedTrigger>();
	#pragma endregion

	#pragma region Apollo_16, chariot_17
		// Apollo
		Entity* Apollo = EntityManager::CreateEntity();
		data = Apollo->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 5;
		Apollo->AddComponent<Stats>()->name = "Apollo_DamageELHM1";
		Apollo->AddComponent<AbilityParams>();
		abil->dmg = 1;abil->ability = ExecuteDamageAbility;
		Apollo->AddComponent<PoolParams>()->target = ENEMY;
		Apollo->AddComponent<TargetSearch>()->search = ExecuteHighestHealthSearch;
		Apollo->AddComponent<PreMatchTrigger>();

		Apollo = EntityManager::CreateEntity();
		data = Apollo->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 5;
		Apollo->AddComponent<Stats>()->name = "Apollo_SummonChariot";
		Apollo->AddComponent<Summoner>();

		abil = Apollo->AddComponent<AbilityParams>();
		Entity* chariot = EntityManager::CreateEntity();
		chariot->AddComponent<Summon>()->ownerId = Apollo->_id;
		chariot->AddComponent<Stats>();
		data = chariot->AddComponent<DemigodData>();
		data->atk = 3; data->hp = 1;
		abil->summonId = chariot->_id;
		abil->ability = ExecuteSummonAbility;

		Apollo->AddComponent<PoolParams>()->target = TEAM;
		Apollo->AddComponent<TargetSearch>()->search = ExecuteSelfSearch;
		Apollo->AddComponent<PostFaintTrigger>();

		Apollo = EntityManager::CreateEntity();
		data = Apollo->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 5;
		Apollo->AddComponent<Stats>()->name = "Apollo_BuffAAM01";
		abil = Apollo->AddComponent<AbilityParams>();
		abil->hpAdd = 1;abil->ability = ExecuteBuffAbility;
		Apollo->AddComponent<PoolParams>()->target = ALL;
		Apollo->AddComponent<TargetSearch>()->search = ExecuteAdjancentSearch;
		Apollo->AddComponent<PreMatchTrigger>();

		Apollo = EntityManager::CreateEntity();
		data = Apollo->AddComponent<DemigodData>();
		data->atk = 2; data->hp = 5;
		Apollo->AddComponent<Stats>()->name = "Apollo_BuffPBD01";
		abil = Apollo->AddComponent<AbilityParams>();
		abil->hpAdd = 1; abil->ability = ExecuteBuffAbility;
		Apollo->AddComponent<PoolParams>()->target = TEAM;
		Apollo->AddComponent<TargetSearch>()->search = ExecuteBehindSearch;
		Apollo->AddComponent<OnDamagedTrigger>();
	#pragma endregion

	#pragma region Aphrodite_18
		// Hephaestus
		Entity* Aphrodite = EntityManager::CreateEntity();
		data = Aphrodite->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 1;
		Aphrodite->AddComponent<Stats>()->name = "Aphrodite_BuffPSA20";
		abil = Aphrodite->AddComponent<AbilityParams>();
		abil->dmgAdd = 1; abil->ability = ExecuteBuffAbility;
		Aphrodite->AddComponent<PoolParams>()->target = TEAM;
		Aphrodite->AddComponent<TargetSearch>()->search = ExecuteSelfSearch;
		Aphrodite->AddComponent<PreAttackTrigger>();

		Aphrodite = EntityManager::CreateEntity();
		data = Aphrodite->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 1;
		Aphrodite->AddComponent<Stats>()->name = "Aphrodite_BuffERS-10(T1)";
		abil = Aphrodite->AddComponent<AbilityParams>();
		abil->dmgAdd = -1;abil->ability = ExecuteBuffAbility;
		Aphrodite->AddComponent<PoolParams>()->target = ENEMY;
		Aphrodite->AddComponent<TargetSearch>()->search = ExecuteRandomSearch;
		Aphrodite->AddComponent<OnSummonTrigger>();

		Aphrodite = EntityManager::CreateEntity();
		data = Aphrodite->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 1;
		Aphrodite->AddComponent<Stats>()->name = "Aphrodite_BuffEFLD1-1";
		abil = Aphrodite->AddComponent<AbilityParams>();
		abil->dmgAdd = 1; abil->hpAdd = -1; abil->ability = ExecuteBuffAbility;
		Aphrodite->AddComponent<PoolParams>()->target = ENEMY;
		Aphrodite->AddComponent<TargetSearch>()->search = ExecuteFrontOfLineupSearch;
		Aphrodite->AddComponent<OnDamagedTrigger>();

		Aphrodite = EntityManager::CreateEntity();
		data = Aphrodite->AddComponent<DemigodData>();
		data->atk = 1; data->hp = 1;
		Aphrodite->AddComponent<Stats>()->name = "Aphrodite_BuffERF-10(T2)";
		abil = Aphrodite->AddComponent<AbilityParams>();
		abil->dmgAdd = -1;abil->ability = ExecuteBuffAbility;
		Aphrodite->AddComponent<PoolParams>()->target = ENEMY;
		Aphrodite->AddComponent<TargetSearch>()->targetAmt = 2;
		Aphrodite->GetComponent<TargetSearch>()->search = ExecuteRandomSearch;
		Aphrodite->AddComponent<PostFaintTrigger>();
	#pragma endregion
	
}

uint8_t Battle(Entity* units, uint8_t size) {
	uint8_t teamSize = size / 2;
	BattleData battleData;
	battleData.unitsData = (DemigodData*)malloc(sizeof(DemigodData) * size);
	battleData.poolData = (PoolParams*)malloc(sizeof(PoolParams) * size);
	battleData.deadList = (bool*)malloc(sizeof(bool) * size);

	for (uint8_t i = 0; i < size; i++) { 
		battleData.unitsData[i] = *(units[i].GetComponent<DemigodData>());
		battleData.poolData[i] = *(units[i].GetComponent<PoolParams>());
		battleData.deadList[i] = false;
		switch (battleData.poolData[i].target)
		{
		case ALL:
			battleData.poolData[i].poolSize = size;
			battleData.poolData[i].begin = 0;
			break;
		case TEAM:
			if (i < teamSize) {
				battleData.poolData[i].poolSize = teamSize;
				battleData.poolData[i].begin = 0;
			}
			else {
				battleData.poolData[i].poolSize = size;
				battleData.poolData[i].begin = teamSize;
			}
			break;
		case ENEMY:
			if (i < teamSize) {
				battleData.poolData[i].poolSize = size;
				battleData.poolData[i].begin = teamSize;
			}
			else {
				battleData.poolData[i].poolSize = teamSize;
				battleData.poolData[i].begin = 0;
			}
			break;
		}
	}

	for (uint8_t i = 0; i < size; i++)
	{
		if (units[i].HasComponent<PreMatchTrigger>()) {
			Ability(&units[i], units, &battleData, battleData.poolData[i],i);
		}
	}
	uint8_t leftIndex = 0;
	uint8_t rightIndex = teamSize;
	bool isTeamOneLoose = false;
	bool isTeamTwoLoose = false;
	
	while (!isTeamTwoLoose && !isTeamOneLoose)
	{
		// check pre attack
		if (units[leftIndex].HasComponent<PreAttackTrigger>()) { 
			Ability(&units[leftIndex],units, &battleData, battleData.poolData[leftIndex], leftIndex);
		};
		if (units[rightIndex].HasComponent<PreAttackTrigger>()) { 
			Ability(&units[rightIndex], units, &battleData, battleData.poolData[rightIndex], rightIndex);
		};

		std::cout << (int)battleData.unitsData[leftIndex].atk << " "<< (int)battleData.unitsData[leftIndex].hp << " vs " << (int)battleData.unitsData[rightIndex].atk << " " << (int)battleData.unitsData[rightIndex].hp << "\n";// << units[leftIndex].GetComponent<Stats>()->name << " " << units[rightIndex].GetComponent<Stats>()->name << "\n";
		
		//Damage
		DealDamage(rightIndex, battleData.unitsData[leftIndex].atk, &battleData, units);
		DealDamage(leftIndex, battleData.unitsData[rightIndex].atk, &battleData, units);

		if (battleData.deadList[leftIndex] && units[leftIndex].HasComponent<PostFaintTrigger>()) {
			Ability(&units[leftIndex], units, &battleData, battleData.poolData[leftIndex], leftIndex);
		};
		if (battleData.deadList[rightIndex] && units[rightIndex].HasComponent<PostFaintTrigger>()) {
			Ability(&units[rightIndex], units, &battleData, battleData.poolData[rightIndex], rightIndex);
		};

		while (battleData.deadList[leftIndex] && leftIndex + 1 <= teamSize) {
			leftIndex++;
		}
		while (battleData.deadList[rightIndex] && rightIndex + 1 <= size) {
			rightIndex++;
		}
		isTeamOneLoose = leftIndex + 1 > teamSize;
		isTeamTwoLoose = rightIndex + 1 > size;
	} 
	//free(battleData.unitsData);
	//free(battleData.poolData);
	return isTeamOneLoose << isTeamTwoLoose;
}

int main()
{
	std::srand(time(0));
	std::cout << "Genereate Babies\n";
	SetupComponents();
	std::cout << "Component Count: " << ComponentManager::componentCount << std::endl;
	SetupDemigods();
	std::cout << "Entity Count " << EntityManager::entityCount << std::endl;
	Entity testTeam[8] = { Entity(0),Entity(8),Entity(24),Entity(24),
						   Entity(0),Entity(8),Entity(16),Entity(30)};

	std::cout << "\nBattle: " << (int) Battle(testTeam, 6) << std::endl;
}







// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
