#include "../Private/CEFPrivatePCH.h"
#include "../classes/CEFUIComponent.h"

UCEFUIComponent::UCEFUIComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	g_handler = new BrowserClient(renderer);
	browser = CefBrowserHost::CreateBrowserSync(CEFManager::info, g_handler.get(), "about:blank", browserSettings, NULL);

	// Ensure the component will tick
	PrimaryComponentTick.bCanEverTick = true;
}

void UCEFUIComponent::initComponent()
{
	CefString str = *DefaultURL;
	UE_LOG(LogCEF, Warning, TEXT("TEST: %s"), *DefaultURL);
	browser->GetMainFrame()->LoadURL(*DefaultURL);
	ResetTexture();
}

void UCEFUIComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UCEFUIComponent::ResetTexture()
{
	// Here we init the texture to its initial state
	DestroyTexture();

	// init the new Texture2D
	Texture = UTexture2D::CreateTransient(Width, Height);
	Texture->AddToRoot();
	Texture->UpdateResourceW();

	// ResetMaterialInstance();

}

void UCEFUIComponent::DestroyTexture()
{
	// Here we destory the texture and its resource
	if (Texture)
	{
		Texture->RemoveFromRoot();

		if (Texture->Resource)
		{
			BeginReleaseResource(Texture->Resource);
			FlushRenderingCommands();
		}

		Texture->MarkPendingKill();
		Texture = nullptr;

	}
}

void UCEFUIComponent::BeginDestroy()
{
	// Destroy our texture
	DestroyTexture();

	// Close our browser for this component
	if (browser)
	{
		browser->GetHost()->CloseBrowser(true);
	}

	Super::BeginDestroy();
}

void UCEFUIComponent::TextureUpdate()
{
	if (!browser || !bIsEnabled)
	{
		UE_LOG(LogCEF, Warning, TEXT("No browesr, or component is not enabled"));
		return;
	}

	if (browser->IsLoading())
	{
		// The browser is not ready yet
		UE_LOG(LogCEF, Warning, TEXT("Texture Browser Loading"));
		return;
	}

	if (Texture && Texture->Resource)
	{

		UE_LOG(LogCEF, Warning, TEXT("Texture & Resource"));

		// Is our texture ready?
		auto ref = static_cast<FTexture2DResource*>(Texture->Resource)->GetTexture2DRHI();
		if (!ref)
		{
			return;
		}

		UE_LOG(LogCEF, Warning, TEXT("Texture Ready"));

		// Get the view from the browser
		const void* texData = g_handler->GetRenderHandlerCustom()->buffer_data;
		const size_t size = Width * Height * sizeof(uint32);

		// @TODO This is a bit heavy to keep reallocating/deallocating, but not a big deal. Maybe we can ping pong between buffers instead.
		TArray<uint32> ViewBuffer;
		ViewBuffer.Init(Width * Height);
		FMemory::Memcpy(ViewBuffer.GetData(), texData, size);

		TextureDataPtr dataPtr = MakeShareable(new TextureData);
		dataPtr->SetRawData(Width, Height, sizeof(uint32), ViewBuffer);

		// Clean up from the per-render
		ViewBuffer.Empty();
		texData = 0;

		UE_LOG(LogCEF, Warning, TEXT("Texture Render Cleanup"));

		ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(
			TextureData,
			TextureDataPtr, ImageData, dataPtr,
			FTexture2DRHIRef, TargetTexture, ref,
			const size_t, DataSize, size,
			{
			uint32 stride = 0;
			void* MipData = GDynamicRHI->RHILockTexture2D(TargetTexture, 0, RLM_WriteOnly, stride, false);

			if (MipData)
			{
				FMemory::Memcpy(MipData, ImageData->GetRawBytesPtr(), ImageData->GetDataSize());
				GDynamicRHI->RHIUnlockTexture2D(TargetTexture, 0, false);
			}

			ImageData.Reset();
		});

	} else {
		UE_LOG(LogCEF, Warning, TEXT("Texture && Texture->Resource fail"));
	}
	
}

void UCEFUIComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// Super tick
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Redraw our Texture for the UI
	TextureUpdate();

}