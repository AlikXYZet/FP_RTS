// 

#pragma once

// Core:
//#include "CoreMinimal.h"

// Base:
#include "UObject/Interface.h"

// Structs:
#include "RTS/Tools/Structs/Properties/UnitCharacterData.h"

// Generated:
#include "UnitCharacterDataInterface.generated.h"
//--------------------------------------------------------------------------------------



// This class does not need to be modified.
UINTERFACE(Blueprintable)
class RTS_API UUnitCharacterDataInterface : public UInterface
{
    GENERATED_BODY()
};



/* Интерфейс Выбираемого Актора */
class RTS_API IUnitCharacterDataInterface
{
    GENERATED_BODY()

public:

    /* ---   Base   --- */

    /** Быстрая Проверка Объекта на реализацию данного Интерфейса
    @note   Вызов методов 'Execute_*' в Объекте без реализованного Интерфейса вызовет фатальную ошибку */
    FORCEINLINE static bool CheckImplementation(const UObject* O)
    {
        return O ? O->GetClass()->ImplementsInterface(UUnitCharacterDataInterface::StaticClass()) : false;
    };
    //-------------------------------------------



    /* ---   Interface: Test  --- */

    /** Test */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable,
        Category = "Test")
    void GetTest(FUnitCharacterData& Value);

    virtual void GetTest_Implementation(FUnitCharacterData& Value) = 0;
    //-------------------------------------------
};

