//

// Base:
#include "Projectile.h"

// Macros:
#include "RTS/Tools/Global/GlobalMacros.h"

// UE:
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

AProjectile::AProjectile()
{
    // Set this pawn to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false; // Предварительно
    //-------------------------------------------


    /* ---   Components   --- */

    // Капсула коллизии Снаряда (Корневой компонент)
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
    RootComponent = SphereComponent;
    SphereComponent->SetRelativeScale3D(FVector(0.1f));
    SphereComponent->SetCollisionProfileName(ProfileName_InvisibleWallDynamic);
    SphereComponent->SetUseCCD(true);

    // Компонент статического Меша визуализации Снаряда
    //ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
    //ProjectileMesh->SetupAttachment(RootComponent);
    //ProjectileMesh->SetRelativeScale3D(FVector(0.63f));
    //ProjectileMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
    //ProjectileMesh->SetGenerateOverlapEvents(false);
    //ProjectileMesh->SetCastShadow(false);

    // FX
    //FXComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX"));
    //FXComponent->SetupAttachment(RootComponent);

    // FX Niagara
    NiagaraFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara FX"));
    NiagaraFXComponent->SetupAttachment(RootComponent);

    // Компонент перемещения Снаряда
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
    ProjectileMovement->InitialSpeed = 2000;
    ProjectileMovement->MaxSpeed = 5000;
    ProjectileMovement->ProjectileGravityScale = 0.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->Velocity = FVector::XAxisVector;
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    // Расчёт Времени прямолинейного Движения
    SetLifeSpan(StraightLineDistance / ProjectileMovement->InitialSpeed);
}
//--------------------------------------------------------------------------------------



/* ---   Ballistics   --- */

void AProjectile::LifeSpanExpired()
{
    //Super::LifeSpanExpired();

    if (StraightLineDistance)
    {
        StraightLineDistance = 0.f;
        ProjectileMovement->ProjectileGravityScale = 1.f;
        SetLifeSpan(1.f);
    }
    else
    {
        Super::LifeSpanExpired();
    }
}
//--------------------------------------------------------------------------------------
