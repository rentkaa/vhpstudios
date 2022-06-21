// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAnimInstance.h"
#include "Hero.h"
#include "GameFramework/CharacterMovementComponent.h"

void UHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Hero = Cast<AHero>(TryGetPawnOwner());
}
void UHeroAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Hero == nullptr)
	{
		Hero = Cast<AHero>(TryGetPawnOwner());
	}
	if (Hero == nullptr) return;

	FVector Velocity = Hero->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = Hero->GetCharacterMovement()->IsFalling();
	bIsAccelerating = Hero->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

	bWeaponEquipped = Hero->IsWeaponEquipped();

	bIsCrouched = Hero->bIsCrouched;

	bAiming = Hero->IsAiming();

}

