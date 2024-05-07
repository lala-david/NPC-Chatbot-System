#include "FlaskCommunicator.h"
#include "HttpModule.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "MyNPC.h"
#include "Kismet/GameplayStatics.h"

void AFlaskCommunicator::SendRequest(const FString &InputText)
{
    // HTTP ��û ����
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

    FString ServerUrl = TEXT("localhost/chat");

    Request->SetURL(ServerUrl);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    // JSON ��ü ����
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    JsonObject->SetStringField(TEXT("message"), InputText);

    // JSON�� ���ڿ��� ����ȭ
    FString RequestContent;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestContent);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    Request->SetContentAsString(RequestContent);

    // ���� ó�� �ݹ� ����
    Request->OnProcessRequestComplete().BindUObject(this, &AFlaskCommunicator::OnResponseReceived);

    // ��û ����
    Request->ProcessRequest();
}

void AFlaskCommunicator::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        FString ResponseContent = Response->GetContentAsString();

        TSharedPtr<FJsonObject> JsonResponse;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);

        if (FJsonSerializer::Deserialize(Reader, JsonResponse))
        {
            FString ResponseMessage = JsonResponse->GetStringField(TEXT("response"));

            // ���� �޽����� NPC�� ����
            AMyNPC *NPC = Cast<AMyNPC>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyNPC::StaticClass()));
            if (NPC)
            {
                NPC->UpdateText(ResponseMessage);
            }

            // �α׿��� ���
            UE_LOG(LogTemp, Log, TEXT("Response: %s"), *ResponseMessage);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP request failed"));
    }
}