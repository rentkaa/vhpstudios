
#include "CombatComponent.h"
#include "titan/Weapon/Weapon.h"
#include "titan/Character/Hero.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UCombatComponent::UCombatComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 450.f;

}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	if (Hero)
	{
		Hero->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}

}
void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Hero)
	{
		Hero->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}
void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	if (Hero)
	{
		Hero->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Hero)
	{
		Hero->GetCharacterMovement()->bOrientRotationToMovement = false;
		Hero->bUseControllerRotationYaw = true;
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
}
void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, bAiming);
}
void UCombatComponent::FireButtonPressed(bool bPressed)
{	//client controlled boolean
	bFireButtonPressed = bPressed;

	if (bFireButtonPressed) {
		ServerFire();
	}

}
void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	//to trace we need to get the screen resulution
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld) {
		FVector Start = CrosshairWorldPosition;

		FVector End = Start + CrosshairWorldDirection * 80000.f;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);
		if (!TraceHitResult.bBlockingHit) {
			TraceHitResult.ImpactPoint = End;
		}
		else {
			DrawDebugSphere(
				GetWorld(),
				TraceHitResult.ImpactPoint,
				12.f,
				12,
				FColor::Red,
				true
			);
		}
	}
}

void UCombatComponent::ServerFire_Implementation() {

	MulticastFire();
}
void UCombatComponent::MulticastFire_Implementation()
{
	if (EquippedWeapon == nullptr) return;
	if (Hero)
	{
		Hero->PlayFireMontage(bAiming);
		EquippedWeapon->Fire();
	}
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Hero == nullptr || WeaponToEquip == nullptr) return;
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* HandSocket = Hero->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Hero->GetMesh());
	}
	EquippedWeapon->SetOwner(Hero);
	Hero->GetCharacterMovement()->bOrientRotationToMovement = false;
	Hero->bUseControllerRotationYaw = true;
}


