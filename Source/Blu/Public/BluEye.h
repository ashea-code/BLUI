#pragma once
#include "BluEye.generated.h"

class BrowserClient;
class RenderHandler;

/*
* Struct for Texture data
* Based on code from VaQuoleUI by Vladimir Alyamkin
*/
struct TextureData
{
	TextureData(uint32 InWidth = 0, uint32 InHeight = 0, uint32 InStride = 0, const TArray<uint32>& InBytes = TArray<uint32>())
	: Bytes(InBytes)
	, Width(InWidth)
	, Height(InHeight)
	, StrideBytes(InStride)
	{

	}

	TextureData(const TextureData &Other)
		: Bytes(Other.Bytes)
		, Width(Other.Width)
		, Height(Other.Height)
		, StrideBytes(Other.StrideBytes)
	{

	}

	TextureData& operator=(const TextureData& Other)
	{
		if (this != &Other)
		{
			SetRawData(Other.Width, Other.Height, Other.StrideBytes, Other.Bytes);
		}
		return *this;
	}

	void SetRawData(uint32 InWidth, uint32 InHeight, uint32 InStride, const TArray<uint32>& InBytes)
	{
		Width = InWidth;
		Height = InHeight;
		StrideBytes = InStride;
		Bytes = InBytes;
	}

	void Empty()
	{
		Bytes.Empty();
	}

	uint32 GetWidth() const { return Width; }
	uint32 GetHeight() const { return Height; }
	uint32 GetStride() const { return StrideBytes; }
	uint32 GetDataSize() const { return Width * Height * StrideBytes; }
	const TArray<uint32>& GetRawBytes() const { return Bytes; }

	/** Accesses the raw bytes of already sized texture data */
	uint32* GetRawBytesPtr() { return Bytes.GetData(); }

private:
	/** Raw uncompressed texture data */
	TArray<uint32> Bytes;

	/** Width of the texture */
	uint32 Width;

	/** Height of the texture */
	uint32 Height;

	/** The number of bytes of each pixel */
	uint32 StrideBytes;

};

typedef TSharedPtr<TextureData, ESPMode::ThreadSafe> TextureDataPtr;
typedef TSharedRef<TextureData, ESPMode::ThreadSafe> TextureDataRef;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FScriptEvent, const FString&, EventName, const FString&, EventMessage);

UCLASS(ClassGroup = Blu, Blueprintable)
class BLU_API UBluEye : public UObject
{

	GENERATED_UCLASS_BODY()

	/* Initialize function, should be called after properties are set */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void init();

	/* The default URL this UI component will load */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		FString DefaultURL;

	/* Is this UI component current active? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		bool bEnabled;

	/* Should this be rendered in game to be transparent? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		bool bIsTransparent;

	/* Width of the view resolution */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		int32 Width;

	/* Height of the view resolution */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		int32 Height;

	/* Material that will be instanced to load UI texture into it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blu")
		UMaterialInterface* BaseMaterial;

	/* Name of parameter to load UI texture into material */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blu")
		FName TextureParameterName = "BluTexture";

	/* Get the texture data from our UI component */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		UTexture2D* GetTexture() const;

	/** Material instance that contains texture inside it */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		UMaterialInstanceDynamic* GetMaterialInstance() const;

	/* Execute JS code inside the browser */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void ExecuteJS(const FString& code);

	UFUNCTION(BlueprintCallable, Category = "Blu")
		void LoadURL(const FString& newURL);

	/* Trigger a mouse-down LEFT click in the browser via a Vector2D */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void TriggerLeftClick(const FVector2D& pos, const float scale = 1);

	/* Trigger a mouse-down RIGHT click in the browser via a Vector2D */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void TriggerRightClick(const FVector2D& pos, const float scale = 1);

	/* Move the mouse in the browser */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void TriggerMouseMove(const FVector2D& pos, const float scale = 1);

	/* Javascript event emitter */
	UPROPERTY(BlueprintAssignable)
		FScriptEvent ScriptEventEmitter;

	/* Trigger a key down event */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void KeyDown(FKeyEvent InKey);

	/* Trigger a key down event */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void KeyUp(FKeyEvent InKey);

	/* Trigger a key press event */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void KeyPress(FKeyEvent InKey);

	/* Trigger a character key event */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void CharKeyPress(FCharacterEvent CharEvent);

	/* Close the browser */
	UFUNCTION(BlueprintCallable, Category = "Blu")
		void CloseBrowser();

	CefRefPtr<CefBrowser> browser;

	void TextureUpdate(const void* buffer);

	void BeginDestroy() override;

	protected:
		CefWindowInfo info;
		CefRefPtr<BrowserClient> g_handler;
		CefBrowserSettings browserSettings;
		RenderHandler* renderer;

		void ResetTexture();
		void DestroyTexture();
		void ResetMatInstance();
		
		// Parse UE4 key events, helper
		void processKeyCode(FKeyEvent InKey);

		// Helper for processing key modifiers
		void processKeyMods(FInputEvent InKey);

		// Store UI state in this UTexture2D
		UTexture2D* Texture;
		UMaterialInstanceDynamic* MaterialInstance;

		CefMouseEvent mouse_event;
		CefKeyEvent key_event;

};