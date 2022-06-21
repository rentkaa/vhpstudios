#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TITAN_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class AHero;
	void EquipWeapon(class AWeapon* WeaponToEquip);

protected:
	virtual void BeginPlay() override;

public:	
	
private:
	class AHero* Hero;
	class AWeapon* EquippedWeapon;
		
};
