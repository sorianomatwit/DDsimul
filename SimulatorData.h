#pragma once

enum TargetPosition
{
    Self,
    Behind,
    Random,
    Summoned,
    Adjancent,
    BackOfLineup,
    FrontOfLineup,
    HighestAttack,
    HighestHealth,
    LowestAttack,
    LowestHealth,
    SecondInLineup
};
enum AbilityTrigger
{
    PreAttack,
    OnDamaged,
    PostFaint,
    PreMatch,
    OnSummon
};
enum PoolTeam
{
    Player,
    Enemy,
    Both
};
enum AbilityType
{
    Damage,
    Buff,
    Summon
};

struct AbilityData
{
    TargetPosition targetPos;
    AbilityTrigger trigger;
    PoolTeam targetTeam;
    AbilityType type;
};

struct DemigodData
{
    unsigned short _Id;
    int atk;
    int hp;
};