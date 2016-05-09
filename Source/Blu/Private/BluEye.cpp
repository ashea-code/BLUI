#include "BluPrivatePCH.h"

UBluEye::UBluEye(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

	Width = 800;
	Height = 600;

	bIsTransparent = false;
	bEnableWebGL = false;

}

void UBluEye::init(UObject* WorldContextObject)
{

	/**
	* We don't want this running in editor unless it's PIE
	* If we don't check this, CEF will spawn infinit processes with widget components
	**/
	const UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject);
	if (GEngine)
	{
		if (!world->IsGameWorld() && !world->IsPlayInEditor())
		{
			UE_LOG(LogBlu, Log, TEXT("Notice: not playing - Component Will Not Initialize"));
			return;
		}
	}
	
	if (Width <= 0 || Height <= 0)
	{
		UE_LOG(LogBlu, Log, TEXT("Can't initialize when Width or Height are <= 0"));
		return;
	}

	browserSettings.universal_access_from_file_urls = STATE_ENABLED;
	browserSettings.file_access_from_file_urls = STATE_ENABLED;

	info.width = Width;
	info.height = Height;

	// Set transparant option
	info.SetAsWindowless(0, bIsTransparent);

	// Figure out if we want to turn on WebGL support
	if (bEnableWebGL)
	{
		if (BluManager::CPURenderSettings)
		{
			UE_LOG(LogBlu, Error, TEXT("You have enabled WebGL for this browser, but CPU Saver is enabled in BluManager.cpp - WebGL will not work!"));
		}
		browserSettings.webgl = STATE_ENABLED;
	}

	renderer = new RenderHandler(Width, Height, this);
	g_handler = new BrowserClient(renderer);
	browser = CefBrowserHost::CreateBrowserSync(info, g_handler.get(), "about:blank", browserSettings, NULL);

	// Setup JS event emitter
	g_handler->SetEventEmitter(&ScriptEventEmitter);

	UE_LOG(LogBlu, Log, TEXT("Component Initialized"));
	CefString str = *DefaultURL;
	UE_LOG(LogBlu, Log, TEXT("Loading URL: %s"), *DefaultURL);

	// Load the default URL
	LoadURL(DefaultURL);
	ResetTexture();

}

void UBluEye::ResetTexture()
{

	// Here we init the texture to its initial state
	DestroyTexture();

	// init the new Texture2D
	Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
	Texture->AddToRoot();
	Texture->UpdateResource();

	RenderParams.Texture2DResource = (FTexture2DResource*)Texture->Resource;

	ResetMatInstance();

}

void UBluEye::DestroyTexture()
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

void UBluEye::TextureUpdate(const void *buffer, FUpdateTextureRegion2D *updateRegions, uint32  regionCount)
{
	if (!browser || !bEnabled)
	{
		UE_LOG(LogBlu, Warning, TEXT("NO BROWSER ACCESS OR NOT ENABLED"))
		return;
	}

	if (Texture && Texture->Resource)
	{

		if (buffer == nullptr)
		{
			UE_LOG(LogBlu, Warning, TEXT("NO TEXTDATA"))
				return;
		}	
	 
		FUpdateTextureRegionsData * RegionData = new FUpdateTextureRegionsData;
		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->NumRegions = regionCount;
		RegionData->SrcBpp = 4;
		RegionData->SrcPitch = Width * 4;
		RegionData->SrcData = (uint8*)buffer;
		RegionData->Regions = updateRegions;


		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
			void,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; RegionIndex++)
				{
					
					RHIUpdateTexture2D(RegionData->Texture2DResource->GetTexture2DRHI(), 0, RegionData->Regions[RegionIndex], RegionData->SrcPitch, RegionData->SrcData
						+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
						+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp);
				}

				FMemory::Free(RegionData->Regions);
				delete RegionData;
			});

	}
	else {
		UE_LOG(LogBlu, Warning, TEXT("no Texture or Texture->resource"))
	}

}

void UBluEye::ExecuteJS(const FString& code)
{
	CefString codeStr = *code;
	browser->GetMainFrame()->ExecuteJavaScript(codeStr, "", 0);
}

void UBluEye::ExecuteJSMethodWithParams(const FString& methodName, const TArray<FString> params)
{

	// Empty param string
	FString paramString = "(";

	// Build the param string
	for (FString param : params)
	{
		paramString += param;
		paramString += ",";
	}
		
	// Remove the last , it's not needed
	paramString.RemoveFromEnd(",");
	paramString += ");";

	// time to call the function
	ExecuteJS(methodName + paramString);

}

void UBluEye::LoadURL(const FString& newURL)
{

	// Check if we want to load a local file

	if (newURL.Contains(TEXT("blui://"), ESearchCase::IgnoreCase, ESearchDir::FromStart))
	{

		// Get the current working directory
		FString GameDir = FPaths::ConvertRelativePathToFull(FPaths::GameDir());

		// We're loading a local file, so replace the proto with our game directory path
		FString LocalFile = newURL.Replace(TEXT("blui://"), *GameDir, ESearchCase::IgnoreCase);

		// Now we use the file proto
		LocalFile = FString(TEXT("file:///")) + LocalFile;

		UE_LOG(LogBlu, Log, TEXT("Load Local File: %s"), *LocalFile)

		// Load it up 
		browser->GetMainFrame()->LoadURL(*LocalFile);

		return;

	}

	// Load as usual
	browser->GetMainFrame()->LoadURL(*newURL);

}

bool UBluEye::IsBrowserLoading()
{

	return browser->IsLoading();

}

void UBluEye::ReloadBrowser(bool IgnoreCache)
{

	if (IgnoreCache)
	{
		return browser->ReloadIgnoreCache();
	}

	browser->Reload();

}

void UBluEye::NavBack()
{

	if (browser->CanGoBack())
	{
		browser->GoBack();
	}

}

void UBluEye::NavForward()
{

	if (browser->CanGoForward())
	{
		browser->GoForward();
	}

}

UTexture2D* UBluEye::ResizeBrowser(const int32 NewWidth, const int32 NewHeight)
{
	// Do we even have a texture to try and resize?
	verifyf(Texture, TEXT("Can't resize when there isn't a texture. Did you forget to call init?"));

	if (NewWidth <= 0 || NewHeight <= 0)
	{
		// We can't do this, just do nothing.
		UE_LOG(LogBlu, Log, TEXT("Can't resize when one or both of the sizes are <= 0!"));
		return Texture;
	}

	// Disable the web view while we resize
	bEnabled = false;

	// Set our new Width and Height
	Width = NewWidth;
	Height = NewHeight;
	
	// Update our render handler
	renderer->Width = NewWidth;
	renderer->Height = NewHeight;

	// We need to reset the texture
	ResetTexture();

	// Let the browser's host know we resized it
	browser->GetHost()->WasResized();

	// Now we can keep going
	bEnabled = true;

	UE_LOG(LogBlu, Log, TEXT("BluEye was resized!"))

	return Texture;

}

UBluEye* UBluEye::SetProperties(const int32 SetWidth,
	const int32 SetHeight,
	const bool SetIsTransparent,
	const bool SetEnabled,
	const bool SetWebGL,
	const FString& SetDefaultURL,
	const FName& SetTextureParameterName,
	UMaterialInterface* SetBaseMaterial)
{
	Width = SetWidth;
	Height = SetHeight;

	bEnabled = SetEnabled;

	bIsTransparent = SetIsTransparent;
	bEnableWebGL = SetWebGL;
	BaseMaterial = SetBaseMaterial;

	DefaultURL = SetDefaultURL;
	TextureParameterName = SetTextureParameterName;

	return this;
}

void UBluEye::TriggerMouseMove(const FVector2D& pos, const float scale)
{

	mouse_event.x = pos.X / scale;
	mouse_event.y = pos.Y / scale;

	browser->GetHost()->SendFocusEvent(true);
	browser->GetHost()->SendMouseMoveEvent(mouse_event, false);

}

void UBluEye::TriggerLeftClick(const FVector2D& pos, const float scale)
{
	TriggerLeftMouseDown(pos, scale);
	TriggerLeftMouseUp(pos, scale);
}

void UBluEye::TriggerRightClick(const FVector2D& pos, const float scale)
{
	TriggerRightMouseDown(pos, scale);
	TriggerRightMouseUp(pos, scale);
}

void UBluEye::TriggerLeftMouseDown(const FVector2D& pos, const float scale)
{
	mouse_event.x = pos.X / scale;
	mouse_event.y = pos.Y / scale;

	browser->GetHost()->SendMouseClickEvent(mouse_event, MBT_LEFT, false, 1);
}

void UBluEye::TriggerRightMouseDown(const FVector2D& pos, const float scale)
{
	mouse_event.x = pos.X / scale;
	mouse_event.y = pos.Y / scale;

	browser->GetHost()->SendMouseClickEvent(mouse_event, MBT_RIGHT, false, 1);
}

void UBluEye::TriggerLeftMouseUp(const FVector2D& pos, const float scale)
{
	mouse_event.x = pos.X / scale;
	mouse_event.y = pos.Y / scale;

	browser->GetHost()->SendMouseClickEvent(mouse_event, MBT_LEFT, true, 1);
}

void UBluEye::TriggerRightMouseUp(const FVector2D& pos, const float scale)
{
	mouse_event.x = pos.X / scale;
	mouse_event.y = pos.Y / scale;

	browser->GetHost()->SendMouseClickEvent(mouse_event, MBT_RIGHT, true, 1);
}

void UBluEye::TriggerMouseWheel(const float MouseWheelDelta, const FVector2D& pos, const float scale)
{
	mouse_event.x = pos.X / scale;
	mouse_event.y = pos.Y / scale;

	browser->GetHost()->SendMouseWheelEvent(mouse_event, MouseWheelDelta * 10, MouseWheelDelta * 10);
}

void UBluEye::KeyDown(FKeyEvent InKey)
{

	processKeyMods(InKey);
	processKeyCode(InKey);

	key_event.type = KEYEVENT_KEYDOWN;
	browser->GetHost()->SendKeyEvent(key_event);

}

void UBluEye::KeyUp(FKeyEvent InKey)
{

	processKeyMods(InKey);
	processKeyCode(InKey);

	key_event.type = KEYEVENT_KEYUP;
	browser->GetHost()->SendKeyEvent(key_event);

}

void UBluEye::KeyPress(FKeyEvent InKey)
{

	// Simply trigger down, then up key events
	KeyDown(InKey);
	KeyUp(InKey);

}

void UBluEye::processKeyCode(FKeyEvent InKey)
{
	key_event.native_key_code = InKey.GetKeyCode();
	key_event.windows_key_code = InKey.GetKeyCode();
}

void UBluEye::CharKeyPress(FCharacterEvent CharEvent)
{

	// Process keymods like usual
	processKeyMods(CharEvent);

	// Below char input needs some special treatment, se we can't use the normal key down/up methods

#if PLATFORM_MAC
	key_event.character = CharEvent.GetCharacter();
#else
    key_event.windows_key_code = CharEvent.GetCharacter();
    key_event.native_key_code = CharEvent.GetCharacter();
#endif
	key_event.type = KEYEVENT_CHAR;
	browser->GetHost()->SendKeyEvent(key_event);
}

void UBluEye::RawCharKeyPress(const FString charToPress, bool isRepeat,
	bool LeftShiftDown,
	bool RightShiftDown,
	bool LeftControlDown,
	bool RightControlDown,
	bool LeftAltDown,
	bool RightAltDown,
	bool LeftCommandDown,
	bool RightCommandDown,
	bool CapsLocksOn)
{

	FModifierKeysState* KeyState = new FModifierKeysState(LeftShiftDown, RightShiftDown, LeftControlDown, 
		RightControlDown, LeftAltDown, RightAltDown, LeftCommandDown, RightCommandDown, CapsLocksOn);

	FCharacterEvent* CharEvent = new FCharacterEvent(charToPress.GetCharArray()[0], *KeyState, 0, isRepeat);

	CharKeyPress(*CharEvent);

}

void UBluEye::SpecialKeyPress(EBluSpecialKeys key, bool LeftShiftDown,
	bool RightShiftDown,
	bool LeftControlDown,
	bool RightControlDown,
	bool LeftAltDown,
	bool RightAltDown,
	bool LeftCommandDown,
	bool RightCommandDown,
	bool CapsLocksOn)
{

	int32 keyValue = key;

	key_event.windows_key_code = keyValue;
	key_event.native_key_code = keyValue;
	key_event.type = KEYEVENT_KEYDOWN;
	browser->GetHost()->SendKeyEvent(key_event);

	key_event.windows_key_code = keyValue;
	key_event.native_key_code = keyValue;
	// bits 30 and 31 should be always 1 for WM_KEYUP
	key_event.type = KEYEVENT_KEYUP;
	browser->GetHost()->SendKeyEvent(key_event);

}

void UBluEye::processKeyMods(FInputEvent InKey)
{

	int mods = 0;

	// Test alt
	if (InKey.IsAltDown())
	{
		mods |= cef_event_flags_t::EVENTFLAG_ALT_DOWN;
	}
	else
	// Test control
	if (InKey.IsControlDown())
	{
		mods |= cef_event_flags_t::EVENTFLAG_CONTROL_DOWN;
	} 
	else
	// Test shift
	if (InKey.IsShiftDown())
	{
		mods |= cef_event_flags_t::EVENTFLAG_SHIFT_DOWN;
	}

	key_event.modifiers = mods;

}

UTexture2D* UBluEye::GetTexture() const
{
	verifyf(Texture, TEXT("There is no texture to return! Did you forget to call init?"));
	return Texture;
}

void UBluEye::ResetMatInstance()
{
	if (!Texture || !BaseMaterial || TextureParameterName.IsNone())
	{
		return;
	}

	// Create material instance
	if (!MaterialInstance)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, NULL);
		if (!MaterialInstance)
		{
			UE_LOG(LogBlu, Warning, TEXT("UI Material instance can't be created"));
			return;
		}
	}

	// Check again, we must have material instance
	if (!MaterialInstance)
	{
		UE_LOG(LogBlu, Error, TEXT("UI Material instance wasn't created"));
		return;
	}

	// Check we have desired parameter
	UTexture* Tex = nullptr;
	if (!MaterialInstance->GetTextureParameterValue(TextureParameterName, Tex))
	{
		UE_LOG(LogBlu, Warning, TEXT("UI Material instance Texture parameter not found"));
		return;
	}

	MaterialInstance->SetTextureParameterValue(TextureParameterName, GetTexture());
}

void UBluEye::CloseBrowser()
{
	BeginDestroy();
}

void UBluEye::BeginDestroy()
{
	if (browser)
	{
		// Close up the browser
		browser->GetHost()->CloseDevTools();
		browser->GetHost()->CloseBrowser(true);

		UE_LOG(LogBlu, Warning, TEXT("Browser Closing"));
	}

	DestroyTexture();
	SetFlags(RF_BeginDestroyed);
	Super::BeginDestroy();
}
