

#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "titan/Character/Hero.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"


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
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}
	//start with the widgets hidden
	if (Pickupwidget) {
		Pickupwidget->SetVisibility(false);
	}
	
	
}
void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
}
void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHero* Hero = Cast<AHero>(OtherActor);
	if (Hero)
	{
		Hero->SetOverlappingWeapon(this);
	}
}
void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AHero* Hero = Cast<AHero>(OtherActor);
	if (Hero)
	{
		Hero->SetOverlappingWeapon(nullptr);
	}
}
void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	if (Pickupwidget)
	{
		Pickupwidget->SetVisibility(bShowWidget);
	}
}
void AWeapon::Fire(const FVector& HitTarget)
{
	if (FireAnimation) {
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}
}
void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		break;
	}
}
void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

