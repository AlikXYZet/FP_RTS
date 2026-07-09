//

#pragma once

// Core:
#include "CoreMinimal.h"

// Global:
#include "RTS/Tools/Global/GlobalMacros.h"

// UE:
#include "Engine/DataTable.h"

// Generated:
#include "FractionData.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Interaction:
//class ;
//--------------------------------------------------------------------------------------



/** Данные Фракции */
USTRUCT(BlueprintType)
struct FFractionData : public FTableRowBase
{
    GENERATED_BODY()

    /* ---   Data   --- */

    /* Наименование Фракции */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Fraction Data")
    FText Name;

    /* Цвет Фракции */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Fraction Data")
    FLinearColor Color = FLinearColor::White;

    /* Количество Юнитов в Фракции на данный момент
    @note   Расчитывается на старте игры */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Fraction Data")
    int32 Number = 0;
    //-------------------------------------------


    /* ---   Statics   --- */

    // Пустые Данные Фракции
    static const FFractionData Empty;
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------



/* ---   Statics   --- */

// Пустые данные Фракции
SET_STATIC_EMPTY(FFractionData)
//--------------------------------------------------------------------------------------
