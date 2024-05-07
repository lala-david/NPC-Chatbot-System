#include "FlaskCommunicator.h"
#include "HttpModule.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "MyNPC.h"
#include "Kismet/GameplayStatics.h"

void AFlaskCommunicator::SendRequest(const FString& InputText) {
    // HTTP 요청 생성
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

    FString ServerUrl = TEXT("203.234.55.134:8900/chat");

    Request->SetURL(ServerUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    // JSON 객체 생성
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    JsonObject->SetStringField(TEXT("message"), InputText);

    // JSON을 문자열로 직렬화
    FString RequestContent;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestContent);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    Request->SetContentAsString(RequestContent);

    // 응답 처리 콜백 설정
    Request->OnProcessRequestComplete().BindUObject(this, &AFlaskCommunicator::OnResponseReceived);

    // 요청 실행
    Request->ProcessRequest();
}

void AFlaskCommunicator::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
    if (bWasSuccessful && Response.IsValid()) {
        FString ResponseContent = Response->GetContentAsString();

        TSharedPtr<FJsonObject> JsonResponse;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);

        if (FJsonSerializer::Deserialize(Reader, JsonResponse)) {
            FString ResponseMessage = JsonResponse->GetStringField(TEXT("response"));

            // 응답 메시지를 NPC에 전달
            AMyNPC* NPC = Cast<AMyNPC>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyNPC::StaticClass()));
            if (NPC) {
                NPC->UpdateText(ResponseMessage);
            }

            // 로그에도 출력
            UE_LOG(LogTemp, Log, TEXT("Response: %s"), *ResponseMessage);
        }
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("HTTP request failed"));
    }
}