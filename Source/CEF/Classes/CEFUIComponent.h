#pragma once
#include "../Private/CEFPrivatePCH.h"
#include "CEFUIComponent.generated.h"

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
	uint32* GetRawBytesPtr() { return Bytes.GetTypedData(); }

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

UCLASS(ClassGroup = CEF, editinlinenew, meta = (BlueprintSpawnableComponent))
class UCEFUIComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	public:
		UCEFUIComponent();

		/* The default URL this UI component will load */
		UPROPERTY(EditAnywhere, Category = "View")
		FString DefaultURL;

		UFUNCTION(BlueprintCallable, Category = "UI|CEF")
		void initComponent();

	private:
		CefRefPtr<BrowserClient> g_handler;
		CefRefPtr<CefBrowser> browser;
		CefBrowserSettings browserSettings;
		RenderHandler* renderer = new RenderHandler();

	
};