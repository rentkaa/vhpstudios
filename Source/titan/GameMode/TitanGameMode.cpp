// Fill out your copyright notice in the Description page of Project Settings.


#include "TitanGameMode.h"
#include "titan/Character/Hero.h"
#include "titan/PlayerController/HeroPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

void ATitanGameMode::PlayerEliminated(class AHero* EliminatedCharacter, class AHeroPlayerController* VictimController, AHeroPlayerController* AttackerController) {
	if (EliminatedCharacter){
		EliminatedCharacter->Elim();
	}
}

void ATitanGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter) {
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController) {

		TArray <AActor*> PlayerStarts;
		//after this next function gets called, we will have a populated TArray of actors
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}