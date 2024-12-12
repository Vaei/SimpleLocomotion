// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleAnimComponent.h"

#include "Engine/World.h"

#if WITH_EDITOR
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Misc/UObjectToken.h"
#include "Logging/MessageLog.h"
#endif

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
		
#if WITH_EDITORONLY_DATA
		ConsumedDebugFunctions.Reset();
#endif
	}
}

void USimpleAnimComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetUpdatedCharacter();
	
#if WITH_EDITORONLY_DATA
	ConsumedDebugFunctions.Reset();
#endif
}

void USimpleAnimComponent::PostLoad()
{
	Super::PostLoad();

	// Not in blueprint, can't call event
	// SetUpdatedCharacter();

#if WITH_EDITORONLY_DATA
	ConsumedDebugFunctions.Reset();
#endif
}

void USimpleAnimComponent::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITORONLY_DATA
	ConsumedDebugFunctions.Reset();
#endif
}

void USimpleAnimComponent::NotImplemented(const FString& FunctionName) const
{
#if WITH_EDITORONLY_DATA
	if (!ConsumedDebugFunctions.Contains(FunctionName))  // Avoid spamming the user
	{
		USimpleAnimComponent* MutableThis = const_cast<USimpleAnimComponent*>(this);
		MutableThis->ConsumedDebugFunctions.Add(FunctionName);
		
		const bool bGameWorld = GetWorld() && GetWorld()->IsGameWorld();
		if (bGameWorld)
		{
			// Showing a notification will let the user see something is wrong before they end play
			FString FuncName = FunctionName.RightChop(FunctionName.Find(TEXT("::")) + 2);
			FuncName = FuncName.Left(FuncName.Find(TEXT("_Implementation")));
			FString Message = FString::Printf(TEXT("%s is not implemented"), *FuncName);
			FNotificationInfo Info(FText::FromString(Message));
			Info.ExpireDuration = 3.0f; // Duration in seconds
			Info.bFireAndForget = true;
			FSlateNotificationManager::Get().AddNotification(Info);
		}

		// Log to the message log
		const FName LogCat = bGameWorld ? TEXT("PIE") : TEXT("AssetCheck");
		FMessageLog Log { LogCat };
		Log.Error(FText::FromString(FunctionName + " is not implemented for "))->AddToken(FUObjectToken::Create(GetClass()))->AddToken(FUObjectToken::Create(GetOwner()->GetClass()));
		Log.Open(EMessageSeverity::Error);
	}
#endif
}
