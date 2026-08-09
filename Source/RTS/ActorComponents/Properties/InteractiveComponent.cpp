//

// Base:
#include "InteractiveComponent.h"

// Global:
#include "GlobalFunctions.h"
#include "GlobalMacros.h"

// UE:
#include "GameFramework/InputSettings.h"

// Interface:
#include "RTS/Tools/Interfaces/Properties/InteractiveInterface.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

// Sets default values for this component's properties
UInteractiveComponent::UInteractiveComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.
    // You can turn these features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false; // Предварительно

    // Используем InitializeComponent()
    bWantsInitializeComponent = true;

    // Автоматическая активация по умолчанию
    bAutoActivate = true;
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void UInteractiveComponent::InitializeComponent()
{
    Super::InitializeComponent();
}

void UInteractiveComponent::BeginPlay()
{
    Super::BeginPlay();

    InitHighlightedComponents();
    InitActionGroup();
}

void UInteractiveComponent::Deactivate()
{
    CursorWasEndFromOwner();

    Super::Deactivate();
}

void UInteractiveComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
    CursorWasEndFromOwner();

    Super::OnComponentDestroyed(bDestroyingHierarchy);
}
//--------------------------------------------------------------------------------------



/* ---   Highlighting   --- */

void UInteractiveComponent::CursorWasBeginOverOwner(AActor* TouchedActor)
{
    if (IsActive())
    {
        if (IInteractiveInterface::CheckImplementation(GetOwner())
            && IInteractiveInterface::Execute_CheckHighlightCondition(GetOwner()))
        {
            for (FComponentRendering& Data : HighlightedComponents)
            {
                if (Data.Component)
                {
                    Data.Component->SetRenderCustomDepth(true);
                }
            }
        }
    }
}

void UInteractiveComponent::CursorWasEndFromOwner(AActor* TouchedActor)
{
    if (IsActive())
    {
        for (FComponentRendering& Data : HighlightedComponents)
        {
            if (Data.Component)
            {
                Data.Component->SetRenderCustomDepth(false);
            }
        }
    }
}

void UInteractiveComponent::InitHighlightedComponents()
{
    if (GetOwner())
    {
        if (IInteractiveInterface::CheckImplementation(GetOwner()))
        {
            TArray<FComponentRendering> lPrimComp = IInteractiveInterface::Execute_GetUsedComponents(GetOwner());
            if (lPrimComp.Num())
            {
                HighlightedComponents.Reserve(HighlightedComponents.Num() + lPrimComp.Num());

                for (FComponentRendering PrimComp : lPrimComp)
                {
                    if (IsValid(PrimComp.Component))
                    {
                        HighlightedComponents.AddUnique(PrimComp);
                    }
                }
            }
        }

        for (FComponentRendering& Data : HighlightedComponents)
        {
            if (IsValid(Data.Component))
            {
                // Настройка канала (цвета) контура подсветки для выделяемого компонента
                Data.Component->SetCustomDepthStencilValue(Data.DepthStencilValue);
            }
        }

        GetOwner()->OnBeginCursorOver.AddDynamic(this, &UInteractiveComponent::CursorWasBeginOverOwner);
        GetOwner()->OnEndCursorOver.AddDynamic(this, &UInteractiveComponent::CursorWasEndFromOwner);
    }
}
//--------------------------------------------------------------------------------------



/* ---   Actions   --- */

void UInteractiveComponent::InitActionGroup()
{
    if (ActionKeys.Num())
    {
        GetOwner()->OnClicked.AddDynamic(this, &UInteractiveComponent::OwnerWasClicked);
    }
}

void UInteractiveComponent::OwnerWasClicked(AActor* TouchedActor, FKey ButtonReleased)
{
    if (IsActive())
    {
        if (ActionKeys.Find(ButtonReleased)
            && IInteractiveInterface::CheckImplementation(GetOwner())
            && IInteractiveInterface::Execute_CheckActionConditions(GetOwner(), ButtonReleased))
        {
            OnOwnerWasClicked.Broadcast(ButtonReleased);
        }
    }
}
//--------------------------------------------------------------------------------------



/* ===   For EDITOR only   === */

#if WITH_EDITOR

/* ---   Base: Debugs   --- */

void UInteractiveComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.Property
        && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UInteractiveComponent, SelectedActionGroups))
    {
        ReInitActionKeys();
    }
};

//void UInteractiveComponent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
//{
//    Super::PostEditChangeChainProperty(PropertyChangedEvent);
//}
//--------------------------------------------------------------------------------------



/* ---   Actions   --- */

void UInteractiveComponent::ReInitActionKeys()
{
    CheckActionGroups({ SelectedActionGroups });

    ActionKeys.Empty();

    if (SelectedActionGroups != NAME_None)
    {
        if (UInputSettings* InputSettings = UInputSettings::GetInputSettings())
        {
            TArray<FInputActionKeyMapping> lArray;

            InputSettings->GetActionMappingByName(SelectedActionGroups, lArray);

            if (lArray.Num())
            {
                ActionKeys.Reserve(lArray.Num());

                for (FInputActionKeyMapping& Data : lArray)
                {
                    ActionKeys.Add(Data.Key);
                }
            }
        }
    }
}
//--------------------------------------------------------------------------------------

#endif // WITH_EDITOR
//======================================================================================
