// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TitanGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TITAN_API ATitanGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	//elimination function that recieves the character that was eliminated from the game, and the player controller's from both of the players in question
	virtual void PlayerEliminated(class AHero* EliminatedCharacter, class AHeroPlayerController* VictimController, AHeroPlayerController* AttackerController);

	virtual void RequestRespawn(class ACharacter* ElimmedCharacter, AController* ElimmedController);
};
