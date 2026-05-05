//

// Base:
#include "RTS_GameState.h"

// Interaction:
#include "RTS_GameMode.h"
//--------------------------------------------------------------------------------------



/* ---   Statics   --- */

// Общедоступный указатель на текущий экземпляр класса 'ARTS_GameState'
ARTS_GameStateBase* ARTS_GameStateBase::CurrentGameState = nullptr;
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

ARTS_GameStateBase::ARTS_GameStateBase()
{
    // Установка вызова функции Tick() в каждом кадре
    PrimaryActorTick.bCanEverTick = false; // Предварительно
    SetActorTickInterval(1.f); // 1 раз/сек.

    // Настройка репликации
    bReplicates = true;
    SetReplicateMovement(false);
    //-------------------------------------------


    /* ---   Statics   --- */

    // Общедоступный указатель на текущий экземпляр класса 'ARTS_GameState'
    CurrentGameState = this;
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void ARTS_GameStateBase::BeginPlay()
{
    Super::BeginPlay();
}

//void ARTS_GameStateBase::Tick(float DeltaSeconds)
//{
//    Super::Tick(DeltaSeconds);
//}

void ARTS_GameStateBase::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    /* ---   Statics   --- */

    CurrentGameState = this;
    //-------------------------------------------
}

//void ARTS_GameStateBase::PostInitializeComponents()
//{
//    Super::PostInitializeComponents();
//}

void ARTS_GameStateBase::Destroyed()
{
    /* ---   Statics   --- */

    CurrentGameState = nullptr;
    //-------------------------------------------

    Super::Destroyed();
}
//--------------------------------------------------------------------------------------
