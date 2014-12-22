#include "../Private/BluPrivatePCH.h"
#include "../classes/BluUIComponent.h"

UBluUIComponent::UBluUIComponent(const class FObjectInitializer& PCIP)
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
	TextureParameterName = TEXT("BluTexture");

	info.width = Width;
	info.height = Height;

}

void UBluUIComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Load up the browser

	// Modify a few settings to allow local file loading
	browserSettings.universal_access_from_file_urls = STATE_ENABLED;
	browserSettings.file_access_from_file_urls = STATE_ENABLED;

	// Set transparant option
	info.SetAsWindowless(NULL, bIsTransparent);

	renderer = new RenderHandler(Width, Height, this);
	g_handler = new BrowserClient(renderer);
	browser = CefBrowserHost::CreateBrowserSync(info, g_handler.get(), "about:blank", browserSettings, NULL);

	// Setup JS event emitter
	g_handler->SetEventEmitter(&ScriptEventEmitter);

	UE_LOG(LogBlu, Log, TEXT("Component Initialized"));
	CefString str = *DefaultURL;
	UE_LOG(LogBlu, Log, TEXT("Loading URL: %s"), *DefaultURL);

	// Load the URL
	browser->GetMainFrame()->LoadURL(*DefaultURL);
	ResetTexture();
}

void UBluUIComponent::ResetTexture()
{

	// Here we init the texture to its initial state
	DestroyTexture();
	
	// init the new Texture2D
	Texture = UTexture2D::CreateTransient(Width, Height);
	Texture->AddToRoot();
	Texture->UpdateResource();

	ResetMaterialInstance();

}

void UBluUIComponent::DestroyTexture()
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

void UBluUIComponent::ResetMaterialInstance()
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
			UE_LOG(LogBlu, Warning, TEXT("Material instance could not be created, trying once again!"));
			return;
		}

		// Try once more
		if (!MaterialInstance)
		{
			UE_LOG(LogBlu, Error, TEXT("Failed to create the material instance, fail!"));
			return;
		}

		// Ensure we have the correct parameter name
		UTexture* Tex = nullptr;
		if (!MaterialInstance->GetTextureParameterValue(TextureParameterName, Tex))
		{
			UE_LOG(LogBlu, Warning, TEXT("Material instance Texture parameter not found"));
			return;
		}

		// Give them our Blu painted texture
		MaterialInstance->SetTextureParameterValue(TextureParameterName, Texture);

	}

}

UMaterialInstanceDynamic* UBluUIComponent::GetMaterialInstance() const
{
	return MaterialInstance;
}

void UBluUIComponent::BeginDestroy()
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

void UBluUIComponent::TextureUpdate()
{
	if (!browser || !bIsEnabled)
	{
		UE_LOG(LogBlu, Warning, TEXT("NO BROWSER ACCESS OR NOT ENABLED"))
		// UE_LOG(LogBlu, Warning, TEXT("No browesr, or component is not enabled"));
		return;
	}

	if (browser->IsLoading())
	{
		UE_LOG(LogBlu, Warning, TEXT("BROWSER LOADING"))
		// The browser is not ready yet
		return;
	}

	if (Texture && Texture->Resource)
	{

		// Is our texture ready?
		auto ref = static_cast<FTexture2DResource*>(Texture->Resource)->GetTexture2DRHI();
		if (!ref)
		{
			UE_LOG(LogBlu, Warning, TEXT("NO REF"))
			return;
		}

		// Get the view from the browser
		const void* texData = g_handler->GetRenderHandlerCustom()->buffer_data;

		if (texData == NULL)
		{
			UE_LOG(LogBlu, Warning, TEXT("NO TEXTDATA"))
			return;
		}

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
	else {
		UE_LOG(LogBlu, Warning, TEXT("no Texture or Texture->resource"))
	}
	
}

void UBluUIComponent::ExecuteJS(FString code)
{
	CefString codeStr = *code;
	UE_LOG(LogBlu, Log, TEXT("Execute JS: %s"), *code)
	browser->GetMainFrame()->ExecuteJavaScript(codeStr, "", 0);
	// create an event function that can be called from JS
}

UTexture2D* UBluUIComponent::GetTexture() const
{
	check(Texture);
	return Texture;
}

void UBluUIComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// Super tick
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

