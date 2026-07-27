//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "GameFramework/Actor.h"

// Plugins:
#include "NiagaraComponent.h"

// Generated:
#include "Projectile.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// UE:
class USphereComponent;
class UProjectileMovementComponent;

// GAS
class UAbilitySystemComponent;
class UGameplayEffect;
//--------------------------------------------------------------------------------------



UCLASS(Abstract)
class RTS_API AProjectile : public AActor
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this character's properties
    AProjectile();
    //-------------------------------------------



    /* ---   Components   --- */

    // Сферическая коллизии Снаряда
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    USphereComponent* SphereComponent = nullptr;

    // Компонент статического Меша визуализации Снаряда
    // @note    Эффективнее будет заменить на Particle или Niagara System
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
    //    Category = "Components",
    //    meta = (AllowPrivateAccess = "true"))
    //UStaticMeshComponent* ProjectileMesh = nullptr;

    // Компонент Партикла визуализации Снаряда
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
    //    Category = "Components",
    //    meta = (AllowPrivateAccess = "true"))
    //UParticleSystemComponent* FXComponent = nullptr;

    // Компонент Партикла визуализации Снаряда
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    UNiagaraComponent* NiagaraFXComponent = nullptr;
    //-------------------------------------------



    /* ---   Non-scene Components   --- */

    // Компонент перемещения Снаряда
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* ProjectileMovement = nullptr;
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    /** Called when the game starts or when spawned */
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Ballistics   --- */

    /** Дистанция прямолинейного Движения
    @note   Использууется имитация прицельной баллистической стрельбы:
            снаряд летит по прямой в пределах радиуса стрельбы ('Straight Line Time'),
            затем под действием гравитации падает вниз (запускается отсчёт времени жизни Актора) */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite,
        Category = "Projectile|Ballistics",
        meta = (ExposeOnSpawn = true))
    float StraightLineDistance = 1.f;

    //

    /** Вызывается по истечении срока службы актера (если он у него есть). */
    virtual void LifeSpanExpired() override;
    //-------------------------------------------



private:

};
