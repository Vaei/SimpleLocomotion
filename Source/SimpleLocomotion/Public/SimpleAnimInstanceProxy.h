// Copyright (c) Jared Taylor

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
		, UniqueMessageLogs({})
	{}
	
	FSimpleAnimInstanceProxy()
		: PendingMessageLogs({})
		, UniqueMessageLogs({})
	{}
	
	void AddPendingMessage(const FString& LogMessage, const FString& LogCategory = "PIE")
	{
		if (!UniqueMessageLogs.Contains(LogMessage))
		{
			PendingMessageLogs.Add(LogCategory, LogMessage);
			UniqueMessageLogs.Add(LogMessage);
		}
	}

	const TMap<FString, FString>& GetPendingMessageLogs() const { return PendingMessageLogs; }
	
	void ResetPendingMessageLogs() { PendingMessageLogs.Reset(); }
	void EmptyPendingMessageLogs() { PendingMessageLogs.Empty(); }

private:
	UPROPERTY()
	TMap<FString, FString> PendingMessageLogs;  // Log category : Log message

	UPROPERTY()
	TArray<FString> UniqueMessageLogs;  // Don't repeat already printed messages

};