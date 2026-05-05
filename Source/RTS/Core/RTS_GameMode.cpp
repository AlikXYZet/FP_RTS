//

// Base:
#include "RTS_GameMode.h"

// Interaction:
#include "RTS_GameState.h"
//--------------------------------------------------------------------------------------



/* ---   Statics   --- */

// Общедоступный указатель на текущий экземпляр класса 'ARTS_GameMode'
ARTS_GameModeBase* ARTS_GameModeBase::CurrentGameMode = nullptr;
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

ARTS_GameModeBase::ARTS_GameModeBase()
{
    // Установка вызова функции Tick() в каждом кадре
    PrimaryActorTick.bCanEverTick = false; // Предварительно
    SetActorTickInterval(1.f); // 1 раз/сек.

    // Настройка репликации
    bReplicates = false;
    SetReplicateMovement(false);
    //-------------------------------------------


    /* ---   Statics   --- */

    // Общедоступный указатель на текущий экземпляр класса 'ARTS_GameMode'
    CurrentGameMode = this;
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void ARTS_GameModeBase::BeginPlay()
{
    Super::BeginPlay();
}

void ARTS_GameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ARTS_GameModeBase::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    /* ---   Statics   --- */

    CurrentGameMode = this;
    //-------------------------------------------
}

void ARTS_GameModeBase::Destroyed()
{
    /* ---   Statics   --- */

    CurrentGameMode = nullptr;
    //-------------------------------------------

    Super::Destroyed();
}
//--------------------------------------------------------------------------------------
