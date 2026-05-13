//

// Base:
#include "UnitCharacter.h"

// Macros:
#include "RTS/Tools/GlobalMacros.h"

// GAS:
#include "RTS/GAS/RTS_AttributeSet.h"

// UE:
#include "Components/CapsuleComponent.h"
#include "Engine/Classes/Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Interaction:
#include "RTS/ActorComponents/Properties/InteractiveComponent.h"
#include "RTS/Core/RTS_PlayerController.h"
//--------------------------------------------------------------------------------------



/* ---   Macros   --- */

/** Макрос: Подписка функции к делегату для передачи значения атрибутов GAS через Событие BP */
#define GAMEPLAYATTRIBUTE_VALUE_Delegating_AUnitCharacter(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_Delegating(AUnitCharacter, PropertyName)
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

AUnitCharacter::AUnitCharacter()
{
    // Set this pawn to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Принудительно
    SetActorTickInterval(1.f); // 1 раз/сек.
    //-------------------------------------------


    /* ---   Components   --- */

    // Главный Меш образа
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    GetMesh()->bReceivesDecals = false;

    // Декаль выделения данного Персонажа
    Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
    Decal->SetupAttachment(GetCapsuleComponent());
    Decal->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
    Decal->SetUsingAbsoluteRotation(true);
    Decal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
    Decal->SetRelativeScale3D(FVector(0.3f));
    Decal->SetHiddenInGame(true);
    //-------------------------------------------


    /* ---   Non-scene Components   --- */

    // Компонент Интерактивности с данным Юнитом
    InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("Interactive Component"));

    // Компонент Системы Способностей (GAS)
    AbilitySystemComp = CreateDefaultSubobject<URTS_AbilitySystemComponent>(TEXT("Ability System Comp"));
    AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // Скрытый Набор Атрибутов (для GAS)
    //AttributeSet = CreateDefaultSubobject<UFPS_AttributeSet>(TEXT("Attributes"));
    // PS: Создание перенесено в `PreInitializeComponents()`, см. в обсуждении ошибки UE-81109
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void AUnitCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (InteractiveComponent)
    {
        InteractiveComponent->OnOwnerWasClicked.AddDynamic(this, &AUnitCharacter::OnInteractiveAction);;
    }
}

//void AUnitCharacter::Tick(float DeltaTime)
//{
//    Super::Tick(DeltaTime);
//}

void AUnitCharacter::Destroyed()
{
    if (AttributeSet)
    {
        // Вызываем уничтожение компонента, созданного через 'NewObject<*>(*)'
        AttributeSet->ConditionalBeginDestroy();
    }

    Super::Destroyed();
}

void AUnitCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
    AttributeSet->SetHealth(0);
}

void AUnitCharacter::PreInitializeComponents()
{
    Super::PreInitializeComponents();

    // Скрытый Набор Атрибутов (для GAS)
    if (HasAuthority() && !AttributeSet)
    {
        AttributeSet = NewObject<URTS_AttributeSet>(this, TEXT("Attributes"));
        AbilitySystemComp->AddAttributeSetSubobject(AttributeSet);
        // PS: Warning! Данный метод автоматически Реплицирует "AttributeSet" на стороне Клиентов
    }
    // PS: Создание здесь экземпляра AttributeSet через NewObject<T>(*),
    // а не в конструкторе через CreateDefaultSubobject<T>(*),
    // является решением ошибки, описанной в UE-81109:
    // "уничтожение сборщиком AttributeSet у дубликатов актора-владельца"
}
//--------------------------------------------------------------------------------------



/* ---   Interface: Interactive   --- */

TArray<FComponentRendering> AUnitCharacter::GetUsedComponents_Implementation()
{
    return TArray<FComponentRendering>{ FComponentRendering(GetMesh()) };
}
//--------------------------------------------------------------------------------------



/* ---   Interactive   --- */

void AUnitCharacter::OnInteractiveAction(const FKey& ButtonReleased)
{
    if (Execute_IsSelectedByPlayer(this))
    {
        Execute_SetSelectedByPlayer(this, false);
        GetRTSLocalController()->RemoveUnitFromSelectedUnits(this);
    }
    else
    {
        Execute_SetSelectedByPlayer(this, true);
        GetRTSLocalController()->AddUnitToSelectedUnits(this);
    }
}
//--------------------------------------------------------------------------------------



/* ---   GAS   --- */

void AUnitCharacter::InitAbilitySystemComp()
{
    if (AbilitySystemComp)
    {
        if (AttributeSet)
        {
            GAMEPLAYATTRIBUTE_VALUE_Delegating_AUnitCharacter(Health);
            GAMEPLAYATTRIBUTE_VALUE_Delegating_AUnitCharacter(MaxHealth);
            GAMEPLAYATTRIBUTE_VALUE_Delegating_AUnitCharacter(Armor);
            GAMEPLAYATTRIBUTE_VALUE_Delegating_AUnitCharacter(MaxArmor);

            AttributeSet->OnZeroHealth.AddDynamic(this, &AUnitCharacter::OnZeroHealth);
            AttributeSet->OnZeroArmor.AddDynamic(this, &AUnitCharacter::Event_OnZeroArmor);
        }
    }
    else
    {
        RTS_Error("AbilitySystemComp is NOT");
    }
}
//--------------------------------------------------------------------------------------



/* ---   GAS Events   --- */

void AUnitCharacter::OnZeroHealth()
{
    // @note    Убираем всё лишнее

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->DisableMovement();
    }

    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    Event_OnZeroHealth();

    SetLifeSpan(10.f);
}
//--------------------------------------------------------------------------------------



/* ---   Interface: Selectable Actor   --- */

void AUnitCharacter::SetSelectedByPlayer_Implementation(bool bIsSelected)
{
    if (Execute_IsSelectedByPlayer(this) != bIsSelected
        && Decal)
    {
        Decal->SetHiddenInGame(!bIsSelected);
    }
}

bool AUnitCharacter::IsSelectedByPlayer_Implementation() const
{
    return Decal ? !Decal->bHiddenInGame : false;
}
//--------------------------------------------------------------------------------------
