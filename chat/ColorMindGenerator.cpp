// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorMindGenerator.h"
#include "Http.h"

// Sets default values
AColorMindGenerator::AColorMindGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AColorMindGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AColorMindGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AColorMindGenerator::GetColorMind()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AColorMindGenerator::HandleGetColorMindCompleted);

	FString URL = FString::Printf(TEXT("http://colormind.io/api/"));
	FString ContentBody = FString::Printf(TEXT("{\"model\": \"default\"}"));

	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb("POST");
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(ContentBody);
	HttpRequest->ProcessRequest();


}


void AColorMindGenerator::HandleGetColorMindCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccesful)
{
	// UE_LOG(LogTemp, Warning, TEXT("The fuction was called"));

	if (!Response || !bWasSuccesful)
	{
		UE_LOG(LogTemp, Warning, TEXT("COLORMINDGENERATOR: The request has failed."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("COLORMINDGENERATOR: %s"), *Response->GetContentAsString());

}