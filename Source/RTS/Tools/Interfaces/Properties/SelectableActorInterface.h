// 

#pragma once

// Core:
//#include "CoreMinimal.h"

// Base:
#include "UObject/Interface.h"

// Enums:
#include "RTS/Tools/Enums/ActorSelectionMode.h"

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

    /** Быстрая Проверка Объекта на реализацию данного Интерфейса
    @note   Вызов методов 'Execute_*' в Объекте без реализованного Интерфейса вызовет фатальную ошибку */
    FORCEINLINE static bool CheckImplementation(const UObject* O)
    {
        return O ? O->GetClass()->ImplementsInterface(USelectableActorInterface::StaticClass()) : false;
    };
    //-------------------------------------------



    /* ---   Interface: Selectable Actor   --- */

    /** Установить состояние "Выбранный" */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable,
        Category = "Selectable Actor")
    void SetSelectionMode(EActorSelectionMode Mode);

    /** Установить состояние "Выбранный" */
    virtual void SetSelectionMode_Implementation(EActorSelectionMode Mode) = 0;


    /** Является ли "Выбранным"? */
    virtual EActorSelectionMode GetSelectionMode() const = 0;
    //-------------------------------------------
};

