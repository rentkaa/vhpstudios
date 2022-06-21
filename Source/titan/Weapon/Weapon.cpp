

#include "Weapon.h"
#include "Components/SphereComponent.h"

AWeapon::AWeapon()
{

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//pawn can ignore when walking over or interacting
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//start with no collision enabled
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//AREA SPHERE
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	//its like the area sphere doesnt exist
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//set collisions on the server
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
	
}
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

