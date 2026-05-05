#pragma once

// Core:
#include "CoreMinimal.h"

// Global:
#include "RTS/Tools/GlobalMacros.h"

// UE:
#include "EngineUtils.h"
#include "GameFramework/Actor.h"

// C++:
#include <random>
//--------------------------------------------------------------------------------------



/* ---   constexpr: Шаблоны проверки соответствия типа на уровне компилятора   --- */

/** Является ли классом типа 'AActor' */
template<class T>
constexpr bool is_Actor = std::is_convertible<T*, AActor*>::value;


/** Является ля контейнером */
template<typename, typename = void>
constexpr bool is_container = false;

template<typename T>
constexpr bool is_container<
    T,
    std::void_t<    // Проверки на наличие функций и переменных
    decltype(std::declval<T>().begin()),
    decltype(std::declval<T>().end()),
    typename T::value_type
    >
> = true;
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
template <class ActorType,
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
        RTS_LOG_Static(Error,
            "At Level there is NO Actor of '%s' class",
            *ActorType::StaticClass()->GetName());
        return nullptr;
    }
}
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
