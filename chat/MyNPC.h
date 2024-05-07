#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "MyNPC.generated.h"

UCLASS()
class LASTLASTTEST_API AMyNPC : public AActor {
    GENERATED_BODY()

public:
    // 생성자
    AMyNPC();

    // 텍스트 업데이트 메서드
    UFUNCTION(BlueprintCallable, Category = "NPC")
    void UpdateText(const FString& Text);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* TextRenderComponent;

    virtual void BeginPlay() override;
};
