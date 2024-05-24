// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleAnimComponent.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(SimpleAnimComponent)

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