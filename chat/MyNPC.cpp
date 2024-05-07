#include "MyNPC.h"

AMyNPC::AMyNPC() {
    PrimaryActorTick.bCanEverTick = false;

    // 텍스트 렌더링 컴포넌트 생성 및 초기화
    TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
    TextRenderComponent->SetupAttachment(RootComponent);

    TextRenderComponent->SetText(FText::FromString("NPC"));
    TextRenderComponent->SetTextRenderColor(FColor::White);
    TextRenderComponent->SetHorizontalAlignment(EHTA_Center);
    TextRenderComponent->SetVerticalAlignment(EVRTA_TextCenter);
}

void AMyNPC::BeginPlay() {
    Super::BeginPlay();
}

void AMyNPC::UpdateText(const FString& Text) {
    if (TextRenderComponent) {
        TextRenderComponent->SetText(FText::FromString(Text));
    }
}
