// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "CharacterBase.h"





ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

