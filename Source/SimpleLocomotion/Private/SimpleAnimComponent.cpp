// Copyright (c) Jared Taylor


#include "SimpleAnimComponent.h"

#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SimpleAnimComponent)

USimpleAnimComponent::USimpleAnimComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void USimpleAnimComponent::OnRegister()
{
	Super::OnRegister();

	if (GetWorld() && GetWorld()->IsGameWorld())
	{
		SetUpdatedCharacter();
	}
}

void USimpleAnimComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetUpdatedCharacter();
}

void USimpleAnimComponent::PostLoad()
{
	Super::PostLoad();

	SetUpdatedCharacter();
}