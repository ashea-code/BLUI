#include "BluPrivatePCH.h"

UBluWidget::UBluWidget(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

	Width = 800;
	Height = 600;

	bIsTransparent = false;

}

void UBluWidget::init()
{

	browserSettings.universal_access_from_file_urls = STATE_ENABLED;
	browserSettings.file_access_from_file_urls = STATE_ENABLED;

	info.width = Width;
	info.height = Height;

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

void UBluWidget::ResetTexture()
{

	// Here we init the texture to its initial state
	DestroyTexture();

	// init the new Texture2D
	Texture = UTexture2D::CreateTransient(Width, Height);
	Texture->AddToRoot();
	Texture->UpdateResource();
}

void UBluWidget::DestroyTexture()
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

void UBluWidget::TextureUpdate(const void *buffer)
{
	if (!browser || !bEnabled)
	{
		UE_LOG(LogBlu, Warning, TEXT("NO BROWSER ACCESS OR NOT ENABLED"))
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

		if (buffer == nullptr)
		{
			UE_LOG(LogBlu, Warning, TEXT("NO TEXTDATA"))
				return;
		}

		const size_t size = Width * Height * sizeof(uint32);

		// @TODO This is a bit heavy to keep reallocating/deallocating, but not a big deal. Maybe we can ping pong between buffers instead.
		TArray<uint32> ViewBuffer;
		ViewBuffer.Init(Width * Height);
		FMemory::Memcpy(ViewBuffer.GetData(), buffer, size);

		TextureDataPtr dataPtr = MakeShareable(new TextureData);

		dataPtr->SetRawData(Width, Height, sizeof(uint32), ViewBuffer);

		// Clean up from the per-render
		ViewBuffer.Empty();
		buffer = 0;

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

void UBluWidget::ExecuteJS(FString code)
{
	CefString codeStr = *code;
	UE_LOG(LogBlu, Log, TEXT("Execute JS: %s"), *code)
	browser->GetMainFrame()->ExecuteJavaScript(codeStr, "", 0);
	// create an event function that can be called from JS
}

void UBluWidget::TriggerMouseMove(FVector2D pos)
{

	mouse_event.x = pos.X;
	mouse_event.y = pos.Y;

	UE_LOG(LogBlu, Warning, TEXT("Mouse Update pos: %s"), *pos.ToString())

	browser->GetHost()->SendFocusEvent(true);
	browser->GetHost()->SendMouseMoveEvent(mouse_event, false);
}

void UBluWidget::TriggerLeftClick(FVector2D pos)
{

	mouse_event.x = pos.X;
	mouse_event.y = pos.Y;

	//browser->GetHost()->SendFocusEvent(true);
	browser->GetHost()->SendMouseClickEvent(mouse_event, MBT_LEFT, false, 1);
	browser->GetHost()->SendMouseClickEvent(mouse_event, MBT_LEFT, true, 1);

	UE_LOG(LogBlu, Warning, TEXT("Left Click %s"), *pos.ToString())
}

void UBluWidget::TriggerRightClick(FVector2D pos)
{

	mouse_event.x = pos.X;
	mouse_event.y = pos.Y;

	//browser->GetHost()->SendFocusEvent(true);
	browser->GetHost()->SendMouseClickEvent(mouse_event, MBT_RIGHT, false, 1);
	browser->GetHost()->SendMouseClickEvent(mouse_event, MBT_RIGHT, true, 1);

	UE_LOG(LogBlu, Warning, TEXT("Right click %s"), *pos.ToString())
}

void UBluWidget::KeyDown(FGeometry Geometry, FKeyEvent InKeyEvent)
{


	CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("key_event");
	msg->GetArgumentList()->SetInt(0, InKeyEvent.GetKeyCode());
	msg->GetArgumentList()->SetInt(1, InKeyEvent.GetCharacter());


	// Check key mod flags!
	// We'll check alt first
	if (InKeyEvent.IsAltDown())
	{
		msg->GetArgumentList()->SetBool(0, true);
	} else {
		msg->GetArgumentList()->SetBool(0, false);
	}

	// Then control takes priority
	if (InKeyEvent.IsControlDown())
	{
		msg->GetArgumentList()->SetBool(1, true);
	} else {
		msg->GetArgumentList()->SetBool(1, false);
	}

	// Shift should be checked last, more important!
	if (InKeyEvent.IsShiftDown())
	{
		msg->GetArgumentList()->SetBool(2, true);
	} else {
		msg->GetArgumentList()->SetBool(2, false);
	}

	browser->SendProcessMessage(PID_RENDERER, msg);

	UE_LOG(LogBlu, Warning, TEXT("SEND KEY EVENT"))
}

UTexture2D* UBluWidget::GetTexture() const
{
	check(Texture)
	return Texture;
}