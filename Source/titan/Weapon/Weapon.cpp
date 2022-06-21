

#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "titan/Character/Hero.h"


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

	Pickupwidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	Pickupwidget->SetupAttachment(RootComponent);

}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
	}
	if (Pickupwidget) {
		Pickupwidget->SetVisibility(false);
	}
	
	
}
void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHero* Hero = Cast<AHero>(OtherActor);
	if (Hero && Pickupwidget)
	{
		Pickupwidget->SetVisibility(true);
	}
}
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

