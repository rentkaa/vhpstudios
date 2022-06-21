
#include "CombatComponent.h"
#include "titan/Weapon/Weapon.h"


UCombatComponent::UCombatComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Hero == nullptr) || WeaponToEquip == nullptr) return;
}

