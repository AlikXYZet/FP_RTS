// 

// Base:
#include "AttributedActor.h"

// Macros:
#include "RTS/Tools/GlobalMacros.h"

// Net:
#include "Net/UnrealNetwork.h"

// GAS:
#include "RTS/GAS/RTS_AttributeSet.h"

// UE:
#include "Engine/Classes/Components/DecalComponent.h"

// Interaction:
#include "RTS/Core/RTS_PlayerController.h"
//--------------------------------------------------------------------------------------



/* ---   Macros   --- */

/** Макрос: Подписка функции к делегату для передачи значения атрибутов GAS через Событие BP */
#define GAMEPLAYATTRIBUTE_VALUE_Delegating_AAttributedActor(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_Delegating(AAttributedActor, PropertyName)
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

AAttributedActor::AAttributedActor()
{
    // Set this pawn to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Предварительно

    // Настройка репликации
    bReplicates = true;
    SetReplicateMovement(true);
    //-------------------------------------------


    /* ---   Components   --- */

    // Корневой компонент
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Меш визуализации
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
    StaticMesh->SetupAttachment(RootComponent);
    StaticMesh->SetCollisionProfileName(ProfileName_Destructible);

    // Декаль выделения данного Персонажа
    Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
    Decal->SetupAttachment(RootComponent);
    Decal->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
    Decal->SetUsingAbsoluteRotation(true);
    Decal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
    Decal->SetRelativeScale3D(FVector(0.3f));
    Decal->SetHiddenInGame(true);
    //-------------------------------------------


    /* ---   Non-scene Components   --- */

    // Компонент Системы Способностей (GAS)
    AbilitySystemComp = CreateDefaultSubobject<URTS_AbilitySystemComponent>(TEXT("Ability System Comp"));
    AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void AAttributedActor::BeginPlay()
{
    Super::BeginPlay();

    InitAbilitySystemComp();
}

void AAttributedActor::PreInitializeComponents()
{
    Super::PreInitializeComponents();

    // Скрытый Набор Атрибутов (для GAS)
    if (HasAuthority() && !AttributeSet)
    {
        AttributeSet = NewObject<URTS_AttributeSet>(this, TEXT("Attributes"));
        AbilitySystemComp->AddAttributeSetSubobject(AttributeSet);
    }
    // PS: Создание здесь экземпляра AttributeSet через NewObject<T>(*),
    // а не в конструкторе через CreateDefaultSubobject<T>(*),
    // является решением ошибки, описанной в UE-81109:
    // "уничтожение сборщиком AttributeSet у дубликатов актора-владельца"
}
//--------------------------------------------------------------------------------------



/* ---   GAS   --- */

void AAttributedActor::InitAbilitySystemComp()
{
    if (AbilitySystemComp)
    {
        AbilitySystemComp->InitAbilityActorInfo(this, this);

        if (AttributeSet)
        {
            GAMEPLAYATTRIBUTE_VALUE_Delegating_AAttributedActor(Health);
            GAMEPLAYATTRIBUTE_VALUE_Delegating_AAttributedActor(MaxHealth);
            GAMEPLAYATTRIBUTE_VALUE_Delegating_AAttributedActor(Armor);
            GAMEPLAYATTRIBUTE_VALUE_Delegating_AAttributedActor(MaxArmor);

            AttributeSet->OnZeroHealth.AddDynamic(this, &AAttributedActor::Event_OnZeroHealth);
            AttributeSet->OnZeroArmor.AddDynamic(this, &AAttributedActor::Event_OnZeroArmor);
        }
    }
    else
    {
        RTS_Error("AbilitySystemComp is NOT");
    }
}
//--------------------------------------------------------------------------------------



/* ---   Interface: Interactive   --- */

TArray<FComponentRendering> AAttributedActor::GetUsedComponents_Implementation()
{
    return TArray<FComponentRendering>{ FComponentRendering(StaticMesh) };
}
//--------------------------------------------------------------------------------------



/* ---   Interactive   --- */
//--------------------------------------------------------------------------------------



/* ---   Interface: Selectable Actor   --- */

void AAttributedActor::SetSelectedByPlayer_Implementation(bool bIsSelected)
{
    if (Execute_IsSelectedByPlayer(this) != bIsSelected
        && Decal)
    {
        Decal->SetHiddenInGame(!bIsSelected);
    }
}

bool AAttributedActor::IsSelectedByPlayer_Implementation() const
{
    return Decal ? !Decal->bHiddenInGame : false;
}
//--------------------------------------------------------------------------------------
