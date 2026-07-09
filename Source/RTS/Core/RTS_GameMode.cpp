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
    if (AllFractions.Num())
    {
        uint8 FracID = Unit->GetGenericTeamId().GetId();

        if (FracID >= AllFractions.Num())
        {
            FracID = AllFractions.Num() - 1;
        }

        // Увеличение Счётчика Юнитов соответствующей Фракции
        AllFractions[FracID].Number += 1;
        OnChangingFactionUnitsNumber.Broadcast(FracID, AllFractions[FracID]);
    }
    else
    {
        M_Error("AllFractions is EMPTY");
    }
}

bool ARTS_GameModeBase::RegisteringUnitDestruction(const AUnitCharacter* Unit)
{
    if (AllFractions.Num())
    {
        uint8 FracID = Unit->GetGenericTeamId().GetId();

        if (FracID >= AllFractions.Num())
        {
            FracID = AllFractions.Num() - 1;
        }

        // Уменьшение Счётчика Юнитов соответствующей Фракции
        AllFractions[FracID].Number -= 1;
        OnChangingFactionUnitsNumber.Broadcast(FracID, AllFractions[FracID]);

        if (AllFractions[FracID].Number <= 0)
        {
            OnFactionDestruction.Broadcast(FracID, AllFractions[FracID]);
        }

        return true;
    }
    else
    {
        M_Error("AllFractions is EMPTY");
    }

    return false;
}

const FFractionData& ARTS_GameModeBase::GetFractionData(const AUnitCharacter* Unit) const
{
    if (Unit)
    {
        return GetFractionDataByID(Unit->GetGenericTeamId().GetId());
    }

    return FFractionData::Empty;
}

const FFractionData& ARTS_GameModeBase::GetFractionDataByID(uint8 TeamID) const
{
    if (AllFractions.Num())
    {
        if (TeamID >= AllFractions.Num())
        {
            TeamID = AllFractions.Num() - 1;
        }

        return AllFractions[TeamID];
    }
    else if (FactionsData)
    {
        TArray<FFractionData*> lAllRows;
        FactionsData->GetAllRows<FFractionData>(__FUNCTION__, lAllRows);

        if (lAllRows.IsValidIndex(TeamID))
        {
            return *lAllRows[TeamID];
        }
    }

    return FFractionData::Empty;
}

const int64 ARTS_GameModeBase::GetNumberOfOtherUnits(uint8 IgnoredTeam)
{
    int64 lCount = 0;

    if (AllFractions.Num())
    {
        for (FFractionData& Data : AllFractions)
        {
            lCount += Data.Number;
        }

        if (AllFractions.IsValidIndex(IgnoredTeam))
        {
            lCount -= AllFractions[IgnoredTeam].Number;
        }
        else
        {
            lCount -= AllFractions.Last().Number;
        }
    }

    return lCount;
}

FORCEINLINE void ARTS_GameModeBase::InitStatistics()
{
    // @note    'FORCEINLINE' действует в пределах данного '.cpp'

    // Получение данных о фракциях
    TArray<FFractionData*> lAllRows;
    FactionsData->GetAllRows<FFractionData>(__FUNCTION__, lAllRows);

    // Резервирование и заполнение памяти текущими + неучтёнными Фракциями
    // @note    ID Фракции совпадает с Номером элемента Массива
    AllFractions.Empty(lAllRows.Num() + 1);
    for (const FFractionData* Data : lAllRows)
    {
        AllFractions.Add(*Data);
    }

    AllFractions.Add(FFractionData());
}
//--------------------------------------------------------------------------------------
