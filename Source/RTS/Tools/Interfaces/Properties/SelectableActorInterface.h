// 

#pragma once

// Core:
//#include "CoreMinimal.h"

// Base:
#include "UObject/Interface.h"

// Structs:
#include "RTS/Tools/Structs/Properties/ComponentRendering.h"

// Generated:
#include "SelectableActorInterface.generated.h"
//--------------------------------------------------------------------------------------



// This class does not need to be modified.
UINTERFACE(Blueprintable)
class RTS_API USelectableActorInterface : public UInterface
{
    GENERATED_BODY()
};



/* Интерфейс Выбираемого Актора */
class RTS_API ISelectableActorInterface
{
    GENERATED_BODY()

public:

    /* ---   Base   --- */

    /** Бфстрая Проверка Объекта на реализацию данного Интерфейса
    @note   Вызов методов 'Execute_*' в Объекте без реализованного Интерфейса вызовет фатальную ошибку */
    FORCEINLINE static bool CheckImplementation(const UObject* O)
    {
        return O ? O->GetClass()->ImplementsInterface(USelectableActorInterface::StaticClass()) : false;
    };
    //-------------------------------------------



    /* ---   Selection   --- */

    /** Установить состояние "Выбранный" */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable,
        Category = "Selectable Actor")
    void SetSelectedByPlayer(bool bIsSelected);

    /** Установить состояние "Выбранный" */
    virtual void SetSelectedByPlayer_Implementation(bool bIsSelected) = 0;


    /** Является ли "Выбранным"? */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable,
        Category = "Selectable Actor",
        meta = (DisplayName = "Is Selected?"))
    bool IsSelectedByPlayer() const;

    /** Является ли "Выбранным"? */
    virtual bool IsSelectedByPlayer_Implementation() const = 0;
    //-------------------------------------------
};

