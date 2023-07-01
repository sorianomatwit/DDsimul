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

struct Dead {};
struct Summon { unsigned short ownerId; }; // @note when calling the summon, you only have the id not the entity

//happen looking at the entity
struct OnDamagedTrigger {};
struct PostFaintTrigger {};
//happen look at the game
struct PreAttackTrigger {};
struct PreMatchTrigger {};
struct OnSummonTrigger {};



enum PRIORITY {
    ONE = 0b00,
    TWO = 0b10,
    THREE = 0b11
};

typedef enum TPOOL {
    TEAM,
    ENEMY,
    ALL
};

struct PoolParams { uint8_t begin = 0;uint8_t poolSize; TPOOL target; };


//struct DamageAbil { uint8_t dmg; PRIORITY priority = THREE;}; // priority 3
//struct BuffAbil { uint8_t dmgAdd; uint8_t hpAdd; PRIORITY priority = TWO;};// priority 2
//struct SummonAbil { unsigned short summonId; PRIORITY priority = ONE; };// priority 1

