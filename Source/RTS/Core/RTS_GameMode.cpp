//

// Base:
#include "RTS_GameMode.h"

// Interaction:
#include "RTS/Units/UnitCharacter.h"
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

    InitStatistics();
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



/* ---   Statistics   --- */

void ARTS_GameModeBase::UnitRegistration(AUnitCharacter* Unit)
{
    if (AllFactions.Num())
    {
        uint8 FracID = Unit->GetGenericTeamId().GetId();

        if (FracID >= AllFactions.Num())
        {
            FracID = AllFactions.Num() - 1;
        }

        // Увеличение Счётчика Юнитов соответствующей Фракции
        AllFactions[FracID].Number += 1;
        OnChangingFactionUnitsNumber.Broadcast(FracID, AllFactions[FracID]);
    }
    else
    {
        M_Error("AllFactions is EMPTY");
    }
}

bool ARTS_GameModeBase::RegisteringUnitDestruction(const AUnitCharacter* Unit)
{
    if (AllFactions.Num())
    {
        uint8 FracID = Unit->GetGenericTeamId().GetId();

        if (FracID >= AllFactions.Num())
        {
            FracID = AllFactions.Num() - 1;
        }

        // Уменьшение Счётчика Юнитов соответствующей Фракции
        AllFactions[FracID].Number -= 1;
        OnChangingFactionUnitsNumber.Broadcast(FracID, AllFactions[FracID]);

        if (AllFactions[FracID].Number <= 0)
        {
            OnFactionDestruction.Broadcast(FracID, AllFactions[FracID]);
        }

        return true;
    }
    else
    {
        M_Error("AllFactions is EMPTY");
    }

    return false;
}

const FFactionData& ARTS_GameModeBase::GetFactionData(const AUnitCharacter* Unit) const
{
    if (Unit)
    {
        return GetFactionDataByID(Unit->GetGenericTeamId().GetId());
    }

    return FFactionData::Empty;
}

const FFactionData& ARTS_GameModeBase::GetFactionDataByID(uint8 TeamID) const
{
    if (AllFactions.Num())
    {
        if (TeamID >= AllFactions.Num())
        {
            TeamID = AllFactions.Num() - 1;
        }

        return AllFactions[TeamID];
    }
    else if (FactionsData)
    {
        TArray<FFactionData*> lAllRows;
        FactionsData->GetAllRows<FFactionData>(__FUNCTION__, lAllRows);

        if (lAllRows.IsValidIndex(TeamID))
        {
            return *lAllRows[TeamID];
        }
    }

    return FFactionData::Empty;
}

const int64 ARTS_GameModeBase::GetNumberOfOtherUnits(uint8 IgnoredTeam)
{
    int64 lCount = 0;

    if (AllFactions.Num())
    {
        for (FFactionData& Data : AllFactions)
        {
            lCount += Data.Number;
        }

        if (AllFactions.IsValidIndex(IgnoredTeam))
        {
            lCount -= AllFactions[IgnoredTeam].Number;
        }
        else
        {
            lCount -= AllFactions.Last().Number;
        }
    }

    return lCount;
}

FORCEINLINE void ARTS_GameModeBase::InitStatistics()
{
    // @note    'FORCEINLINE' действует в пределах данного '.cpp'

    // Получение данных о фракциях
    TArray<FFactionData*> lAllRows;
    FactionsData->GetAllRows<FFactionData>(__FUNCTION__, lAllRows);

    // Резервирование и заполнение памяти текущими + неучтёнными Фракциями
    // @note    ID Фракции совпадает с Номером элемента Массива
    AllFactions.Empty(lAllRows.Num() + 1);
    for (const FFactionData* Data : lAllRows)
    {
        AllFactions.Add(*Data);
    }

    AllFactions.Add(FFactionData());
}
//--------------------------------------------------------------------------------------
