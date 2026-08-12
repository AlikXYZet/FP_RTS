//

#pragma once

// Core:
#include "CoreMinimal.h"

// Global:
#include "GlobalMacros.h"

// UE:
#include "Engine/DataTable.h"

// Generated:
#include "FactionData.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Interaction:
//class ;
//--------------------------------------------------------------------------------------



/** Данные Фракции */
USTRUCT(BlueprintType)
struct FFactionData : public FTableRowBase
{
    GENERATED_BODY()

    /* ---   Data   --- */

    /* Наименование Фракции */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Faction Data")
    FText Name;

    /* Цвет Фракции */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Faction Data")
    FLinearColor Color = FLinearColor::White;

    /* Количество Юнитов в Фракции на данный момент
    @note   Расчитывается на старте игры */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Faction Data")
    int32 Number = 0;
    //-------------------------------------------


    /* ---   Statics   --- */

    /* Пустые Данные Фракции */
    static const FFactionData Empty;
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
