// 

#pragma once

// Core:
//#include "CoreMinimal.h"

// Base:
#include "UObject/Interface.h"

// Structs:
#include "RTS/Tools/Structs/Properties/ComponentRendering.h"

// Generated:
#include "InteractiveInterface.generated.h"
//--------------------------------------------------------------------------------------



// This class does not need to be modified.
UINTERFACE(Blueprintable)
class RTS_API UInteractiveInterface : public UInterface
{
    GENERATED_BODY()
};



/* Интерфейс Интерактивности Актора
* @note Используется как шаблон для взаимодействия с "UInteractiveComponent" */
class RTS_API IInteractiveInterface
{
    GENERATED_BODY()

public:

    /* ---   Base   --- */

    /** Бфстрая Проверка Объекта на реализацию данного Интерфейса
    @note   Вызов методов 'Execute_*' в Объекте без реализованного Интерфейса вызовет фатальную ошибку */
    FORCEINLINE static bool CheckImplementation(const UObject* O)
    {
        return O->GetClass()->ImplementsInterface(UInteractiveInterface::StaticClass());
    };
    //-------------------------------------------



    /* ---   Highlighting   --- */

    /** Получить компоненты, которые требуется подсветить */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable,
        Category = "Interactive|Highlighting")
    TArray<FComponentRendering> GetUsedComponents();

    virtual TArray<FComponentRendering> GetUsedComponents_Implementation() { return TArray<FComponentRendering>(); };

    /** Предикат условия выделения (подсвечивания) Компонента */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable,
        Category = "Interactive|Highlighting")
    bool CheckHighlightCondition();

    virtual bool CheckHighlightCondition_Implementation() { return true; };
    //-------------------------------------------



    /* ---   Actions   --- */

    /** Предикат условия взаимодействия с Актором */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable,
        Category = "Interactive|Actions")
    bool CheckActionConditions(const FKey& ButtonReleased);

    virtual bool CheckActionConditions_Implementation(const FKey& ButtonReleased)
    {
        return true;
        // PS:  Вызов метода 'CheckHighlightCondition' из среды 'Blueprint' на данном этапе НЕ возможен,
        //      поэтому 'CheckActionConditions' не может дублировать его по умолчанию.
        //      В других случаях - вызов осуществляется через 'Execute_CheckHighlightCondition(UObject* O)'
    };
    //-------------------------------------------
};

