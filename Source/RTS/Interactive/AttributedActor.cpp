// 

// Base:
#include "AttributedActor.h"

// Global:
#include "GlobalMacros.h"

// GAS:
#include "RTS/GAS/RTS_AbilitySystemComponent.h"
#include "RTS/GAS/RTS_AttributeSet.h"

// UE:
#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

// Interaction:
#include "RTS/ActorComponents/Properties/InteractiveComponent.h"
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
    StaticMesh->SetCustomDepthStencilValue(1);
    StaticMesh->bReceivesDecals = false;

    // Декаль выделения данного Персонажа
    Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
    Decal->SetupAttachment(RootComponent);
    //Decal->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
    Decal->SetUsingAbsoluteRotation(true);
    Decal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
    Decal->SetRelativeScale3D(FVector(0.3f));
    Decal->SetHiddenInGame(true);

    /* Виджет отображения Атрибутов данного Актора */
    AttributesWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Attributes Widget"));
    AttributesWidget->SetupAttachment(RootComponent);
    AttributesWidget->SetHiddenInGame(true);
    //-------------------------------------------


    /* ---   Non-scene Components   --- */

    // Компонент Интерактивности с данным Юнитом
    InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("Interactive Component"));

    // Компонент "стимуляции" Сенсорики
    AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerception: Stimuli Source"));
    // PS: 'UAIPerceptionStimuliSourceComponent' не предоставляет изменение его параметров (отмечены как 'protected')
    // Рекомендация: Перейти в среду 'Blueprint' или Создать дочерний класс-оболочку с методами изменения необходимых параметров

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
    if (GetAbilitySystemComponent())
    {
        GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

        if (GetRTSAttributeSet())
        {
            GAMEPLAYATTRIBUTE_VALUE_Delegating(Health);
            GAMEPLAYATTRIBUTE_VALUE_Delegating(MaxHealth);
            GAMEPLAYATTRIBUTE_VALUE_Delegating(Armor);
            GAMEPLAYATTRIBUTE_VALUE_Delegating(MaxArmor);

            GAMEPLAYATTRIBUTE_ZERO_Delegating(OnZeroHealth);
            GAMEPLAYATTRIBUTE_ZERO_Delegating(OnZeroArmor);
        }
    }
    else
    {
        M_Error("AbilitySystemComp is NOT");
    }
}
//--------------------------------------------------------------------------------------



/* ---   Interface: Interactive   --- */

TArray<FComponentRendering> AAttributedActor::GetUsedComponents_Implementation()
{
    return TArray<FComponentRendering>{ FComponentRendering(StaticMesh, TeamID ? 1 : 2) };
}
//--------------------------------------------------------------------------------------



/* ---   Interactive   --- */
//--------------------------------------------------------------------------------------



/* ---   Interface: Selectable Actor   --- */

void AAttributedActor::SetSelectionMode_Implementation(EActorSelectionMode Mode)
{
    if (Decal && GetSelectionMode() != Mode)
    {
        CurrentSelectionMode = Mode;
        Decal->SetHiddenInGame(Mode == EActorSelectionMode::NotSelected);
    }
}

EActorSelectionMode AAttributedActor::GetSelectionMode() const
{
    return CurrentSelectionMode;
}
//--------------------------------------------------------------------------------------
