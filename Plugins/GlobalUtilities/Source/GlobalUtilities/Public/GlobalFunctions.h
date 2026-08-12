//

#pragma once

// Core:
#include "CoreMinimal.h"

// Global:
#include "GlobalMacros.h"

// UE:
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "GameFramework/InputSettings.h"

// C++:
#include <random>

// Generated:
//#include "GlobalFunctions.generated.h"
//--------------------------------------------------------------------------------------



/* ---   constexpr: Шаблоны проверки соответствия типа на уровне компилятора   --- */

/* Используются следующим способом:
    @param  is_check    -- Требуемая проверка соответствия
    @param  T           -- Класс, требующий выбранной проверки

    template<class T, class = std::enable_if_t<is_check<T>>>
*/

// Warning: Вероятно, может вызвать проблемы с мультиплатформенностью проекта -- требуется отдельная проверка
//
///** Является ли классом типа 'AActor' */
//template<class T>
//constexpr bool is_Actor = std::is_convertible<T*, AActor*>::value;
//
//
///** Является ля контейнером */
//template<typename, typename = void>
//constexpr bool is_container = false;
//
//template<typename T>
//constexpr bool is_container<
//    T,
//    std::void_t<    // Проверки на наличие функций и переменных
//    decltype(std::declval<T>().begin()),
//    decltype(std::declval<T>().end()),
//    typename T::value_type
//    >
//> = true;
//----------------------------------------------------------------------------------------------------



/* ---   Statics   --- */

/** Получить Ряд Чисел в диапазоне [Begin; End)
@warning    Предполагается, что 'End > Begin' */
static TArray<int32> GetNumbersSeries(int32 Begin, int32 End)
{
    TArray<int32> Result;
    Result.Empty(End - Begin);

    while (Begin != End)
    {
        Result.Add(Begin);
        ++Begin;
    }

    return Result;
};

/** Получить первый элемент мира соответствующего класса
@note   Возможно, можно сильнее упростить получение "первого" элемента на карте по его типу.
        Однако, в местах текущего использования это не столь важно... */
/*template <class ActorType,
    class = std::enable_if_t<is_Actor<ActorType>>>
static ActorType* GetFirstActorOfType()
{
    TActorIterator<ActorType> It(GEngine->GameViewport->GetWorld(), ActorType::StaticClass());
    if (It)
    {
        return *It;
    }
    else
    {
        M_Error("At Level there is NO Actor of '%s' class",
            *ActorType::StaticClass()->GetName());

        return nullptr;
    }
}*/
//--------------------------------------------------------------------------------------



/* ---   Random   --- */

// Получение рандомного значения [min, max]
FORCEINLINE int32 GetRandom(int32 max, int32 min = 0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32> distrib(min, max);

    return distrib(gen);
};

// Получение рандомного bool значения
FORCEINLINE bool GetRandom(float Chance = 0.5f)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(Chance);

    return d(gen);
};

// Получение рандомного float значения
FORCEINLINE float GetRandomFloat(float max = 1.f, float min = 0.f)
{
    return GetRandom(max * 256, min * 256) / 256.f;
};
//--------------------------------------------------------------------------------------



/* ---   Inputs | Get   --- */

/* Получить имена зарегистрированных Групп Действий */
static TArray<FName> GetActionGroups()
{
    TArray<FName> ActionNames;

    if (UInputSettings* InputSettings = UInputSettings::GetInputSettings())
    {
        InputSettings->GetActionNames(ActionNames);
        ActionNames.AddUnique(NAME_None);
    }

    return ActionNames;
};

/* Получить имена зарегистрированных Групп Осей */
static TArray<FName> GetAxisGroups()
{
    TArray<FName> AxisNames;

    if (UInputSettings* InputSettings = UInputSettings::GetInputSettings())
    {
        InputSettings->GetAxisNames(AxisNames);
        AxisNames.AddUnique(NAME_None);
    }

    return AxisNames;
};
//--------------------------------------------------------------------------------------



/* ---   Inputs | Check   --- */

/* Проверить Массив Групп входных данных только для Действий ('Action') */
static void CheckActionGroups(const TArray<FName>& Actions)
{
    if (UInputSettings* InputSettings = UInputSettings::GetInputSettings())
    {
        TArray<FName> lArray_AllNames;

        InputSettings->GetActionNames(lArray_AllNames);

        for (const FName& Data : Actions)
        {
            if (Data == NAME_None)
            {
                M_LOG_Static(Warning, "Not used at least one of the Actions ('%s')",
                    *Data.ToString());
            }
            else if (lArray_AllNames.Find(Data) == INDEX_NONE)
            {
                M_Error_Static("'%s' is NOT an Action",
                    *Data.ToString());
            }
            else
            {
                TArray<FInputActionKeyMapping> lKeysArray;
                InputSettings->GetActionMappingByName(Data, lKeysArray);

                bool lCheck = false;

                for (FInputActionKeyMapping& Key : lKeysArray)
                {
                    if (Key.ActionName != NAME_None)
                    {
                        lCheck = true;
                        break;
                    }
                }

                if (!lCheck)
                {
                    M_LOG_Static(Warning, "Action '%s' is Empty",
                        *Data.ToString());
                }
            }
        }
    }
}

/* Проверить Массив Групп входных данных только для Осей ('Axis') */
static void CheckAxisGroups(const TArray<FName>& Axis)
{
    if (UInputSettings* InputSettings = UInputSettings::GetInputSettings())
    {
        TArray<FName> lArray_AllNames;

        InputSettings->GetAxisNames(lArray_AllNames);

        for (const FName& Data : Axis)
        {
            if (Data == NAME_None)
            {
                M_LOG_Static(Warning, "Not used at least one of the Axes ('%s')",
                    *Data.ToString());
            }
            else if (lArray_AllNames.Find(Data) == INDEX_NONE)
            {
                M_Error_Static("'%s' is NOT an Axis",
                    *Data.ToString());
            }
            else
            {
                TArray<FInputAxisKeyMapping> lKeysArray;
                InputSettings->GetAxisMappingByName(Data, lKeysArray);

                bool lCheck = false;

                for (FInputAxisKeyMapping& Key : lKeysArray)
                {
                    if (Key.AxisName != NAME_None)
                    {
                        lCheck = true;
                        break;
                    }
                }

                if (!lCheck)
                {
                    M_LOG_Static(Warning, "Axis '%s' is Empty",
                        *Data.ToString());
                }
            }
        }
    }
}

/* Проверить Массивы Групп входных данных */
static void CheckInputGroupArray(const TArray<FName>& Actions, const TArray<FName>& Axis)
{
    if (UInputSettings* InputSettings = UInputSettings::GetInputSettings())
    {
        TArray<FName> lArray_AllNames;


        InputSettings->GetActionNames(lArray_AllNames);

        for (const FName& Data : Actions)
        {
            if (Data == NAME_None)
            {
                M_LOG_Static(Warning, "Not used at least one of the Actions ('%s')",
                    *Data.ToString());
            }
            else if (lArray_AllNames.Find(Data) == INDEX_NONE)
            {
                M_Error_Static("'%s' is NOT an Action",
                    *Data.ToString());
            }
            else
            {
                TArray<FInputActionKeyMapping> lKeysArray;
                InputSettings->GetActionMappingByName(Data, lKeysArray);

                bool lCheck = false;

                for (FInputActionKeyMapping& Key : lKeysArray)
                {
                    if (Key.ActionName != NAME_None)
                    {
                        lCheck = true;
                        break;
                    }
                }

                if (!lCheck)
                {
                    M_LOG_Static(Warning, "Action '%s' is Empty",
                        *Data.ToString());
                }
            }
        }


        InputSettings->GetAxisNames(lArray_AllNames);

        for (const FName& Data : Axis)
        {
            if (Data == NAME_None)
            {
                M_LOG_Static(Warning, "Not used at least one of the Axes ('%s')",
                    *Data.ToString());
            }
            else if (lArray_AllNames.Find(Data) == INDEX_NONE)
            {
                M_Error_Static("'%s' is NOT an Axis",
                    *Data.ToString());
            }
            else
            {
                TArray<FInputAxisKeyMapping> lKeysArray;
                InputSettings->GetAxisMappingByName(Data, lKeysArray);

                bool lCheck = false;

                for (FInputAxisKeyMapping& Key : lKeysArray)
                {
                    if (Key.AxisName != NAME_None)
                    {
                        lCheck = true;
                        break;
                    }
                }

                if (!lCheck)
                {
                    M_LOG_Static(Warning, "Axis '%s' is Empty",
                        *Data.ToString());
                }
            }
        }
    }
}
//--------------------------------------------------------------------------------------
