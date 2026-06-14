//

#pragma once

// Core:
#include "CoreMinimal.h"

// Generated:
#include "ComponentRendering.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Interaction:
//class ;
//--------------------------------------------------------------------------------------



/** Рендеринг (Визуализация) Компонентов */
USTRUCT(BlueprintType)
struct FComponentRendering
{
    GENERATED_BODY()

    /* ---   Constructors   --- */

    FComponentRendering() {};
    FComponentRendering(UPrimitiveComponent* UsedComponent, uint8 Depth = 0)
        : Component(UsedComponent), DepthStencilValue(Depth) {};
    //-------------------------------------------


    /* ---   Data   --- */

    // Используемый Компонент
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPrimitiveComponent* Component = nullptr;

    // Значение глубины Выделения
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 DepthStencilValue = 0;
    //-------------------------------------------


    /* ---   Operators | ==   --- */

    FORCEINLINE bool operator==(const FComponentRendering& Second) const
    {
        return Component == Second.Component;
    }

    FORCEINLINE bool operator==(const UPrimitiveComponent* SecondComponent) const
    {
        return Component == SecondComponent;
    }
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
