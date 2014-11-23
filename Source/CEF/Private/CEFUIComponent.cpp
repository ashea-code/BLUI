#include "../Private/CEFPrivatePCH.h"
#include "../classes/CEFUIComponent.h"

UCEFUIComponent::UCEFUIComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	// Ensure the component will tick
	PrimaryComponentTick.bCanEverTick = true;

	// Make sure this compoenent will Initialize
	bWantsInitializeComponent = true;

	// Tick with the pre phyics group
	PrimaryComponentTick.TickGroup = TG_PrePhysics;

	// Initial w/h
	Width = 800;
	Height = 600;

	// Initial Texture Name
	TextureParameterName = TEXT("UITexture");

	info.width = Width;
	info.height = Height;
	info.SetAsWindowless(NULL, true);

	g_handler = new BrowserClient(renderer);
	browser = CefBrowserHost::CreateBrowserSync(info, g_handler.get(), "about:blank", browserSettings, NULL);

}

void UCEFUIComponent::InitializeComponent()
{
	Super::InitializeComponent();
	UE_LOG(LogCEF, Log, TEXT("Component Initialized"));
	CefString str = *DefaultURL;
	UE_LOG(LogCEF, Log, TEXT("Loading URL: %s"), *DefaultURL);
	browser->GetMainFrame()->LoadURL(*DefaultURL);
	ResetTexture();
}

void UCEFUIComponent::ResetTexture()
{

	// Here we init the texture to its initial state
	DestroyTexture();
	
	// init the new Texture2D
	Texture = UTexture2D::CreateTransient(Width, Height);
	Texture->AddToRoot();
	Texture->UpdateResource();

	ResetMaterialInstance();

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

void UCEFUIComponent::ResetMaterialInstance()
{

	// Check we have all the requirements
	if (!Texture || !BaseMaterial || TextureParameterName.IsNone())
	{
		return;
	}

	if (!MaterialInstance)
	{
		// we need to create a new one
		MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, NULL);


		// Do some checking to ensure the material instance was actually created! We need this!
		if (!MaterialInstance)
		{
			UE_LOG(LogCEF, Warning, TEXT("Material instance could not be created, trying once again!"));
			return;
		}

		// Try once more
		if (!MaterialInstance)
		{
			UE_LOG(LogCEF, Error, TEXT("Failed to create the material instance, fail!"));
			return;
		}

		// Ensure we have the correct parameter name
		UTexture* Tex = nullptr;
		if (!MaterialInstance->GetTextureParameterValue(TextureParameterName, Tex))
		{
			UE_LOG(LogCEF, Warning, TEXT("Material instance Texture parameter not found"));
			return;
		}

		// Give them our CEF painted texture
		MaterialInstance->SetTextureParameterValue(TextureParameterName, Texture);

	}

}

UMaterialInstanceDynamic* UCEFUIComponent::GetMaterialInstance() const
{
	return MaterialInstance;
}

void UCEFUIComponent::BeginDestroy()
{
	// Destroy our texture
	DestroyTexture();

	// Close our browser for this component if it had a running browser
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
		// UE_LOG(LogCEF, Warning, TEXT("No browesr, or component is not enabled"));
		return;
	}

	if (browser->IsLoading())
	{
		// The browser is not ready yet
		return;
	}

	if (Texture)
	{

		// Is our texture ready?
		auto ref = static_cast<FTexture2DResource*>(Texture->Resource)->GetTexture2DRHI();
		if (!ref)
		{
			return;
		}

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

	}
	
}

void UCEFUIComponent::ExecuteJS(FString code)
{
	CefString codeStr = *code;
	UE_LOG(LogCEF, Log, TEXT("Execute JS: %s"), *code)
	browser->GetMainFrame()->ExecuteJavaScript(codeStr, "", 0);
}

void UCEFUIComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// Super tick
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Redraw our Texture for the UI
	TextureUpdate();

}