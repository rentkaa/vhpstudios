// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroPlayerController.h"
#include "titan/HUD/HeroHUD.h"
#include "titan/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
void AHeroPlayerController::BeginPlay() {

	Super::BeginPlay();

	HeroHUD = Cast<AHeroHUD>(GetHUD());
}

void AHeroPlayerController::SetHUDHealth(float Health, float MaxHealth) {

	HeroHUD = HeroHUD == nullptr ? Cast<AHeroHUD>(GetHUD()) : HeroHUD;

	bool bHUDValid = HeroHUD &&
		HeroHUD->CharacterOverlay &&
		HeroHUD->CharacterOverlay->HealthBar &&
		HeroHUD->CharacterOverlay->HealthText;

	if (bHUDValid) {
		const float HealthPercent = Health / MaxHealth;
		HeroHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		HeroHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}