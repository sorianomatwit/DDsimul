#pragma once
#include <iostream>

struct DemigodData
{
    uint8_t atk = 0;
    uint8_t hp = 1;
};

struct Stats {
    const char* name;
    double winCount;
    double looseCount;
    double drawCount;
    double deathCount;
};

struct Summon { unsigned short ownerId; }; // @note when calling the summon, you only have the id not the entity
struct Summoner {};

//happen looking at the entity
struct OnDamagedTrigger {};
struct PostFaintTrigger {};
//happen look at the game
struct PreAttackTrigger {};
struct PreMatchTrigger {};
struct OnSummonTrigger {};



enum TPOOL {
    TEAM,
    ENEMY,
    ALL
};

struct PoolParams { uint8_t begin = 0;uint8_t poolSize; TPOOL target; };


//struct SelfSearch {};
//struct BehindSearch {};
//struct RandomSearch {};
//struct SummonedSearch {};
//struct AdjancentSearch{};
//struct BackOfLineupSearch{};
//struct FrontOfLineupSearch{};
//struct HighestAttackSearch{};
//struct HighestHealthSearch{};
//struct LowestAttackSearch{};
//struct LowestHealthSearch{};
//struct SecondInLineupSearch{};


//struct DamageAbil { uint8_t dmg;}; // priority 3
//struct BuffAbil { uint8_t dmgAdd = 0; uint8_t hpAdd = 0;};// priority 2
//struct SummonAbil { unsigned short summonId; };// priority 1

