// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstanceProxy.h"
#include "SimpleAnimInstanceProxy.generated.h"

USTRUCT(meta = (DisplayName = "Native Variables"))
struct FSimpleAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

	FSimpleAnimInstanceProxy(UAnimInstance* Instance)
		: Super(Instance)
		, PendingMessageLogs({})
	{}
	
	FSimpleAnimInstanceProxy()
		: PendingMessageLogs({})
	{}

	UPROPERTY()
	TMap<FString, FString> PendingMessageLogs;  // Log category : Log message
};