// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlaskCommunicator.generated.h"

class IHttpRequest;
class IHttpResponse;

typedef TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> FHttpRequestPtr;
typedef TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> FHttpResponsePtr;

UCLASS()
class LASTLASTTEST_API AFlaskCommunicator : public AGameModeBase {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "HTTP")
    void SendRequest(const FString& InputText);
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
