#include "BluPrivatePCH.h"

UBluHUD::UBluHUD(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

	// Default properties
	bIsEnabled = true;
	bIsTransparent = true;

	// Initial screen size
	Width = 800;
	Height = 600;

	DefaultURL = "about:blank";

	TextureParameterName = TEXT("BluTexture");

}

void UBluHUD::InitializeComponent()
{
	Super::InitializeComponent();

	SetScreenPosition(0.f, 0.f);
	SetScreenSize(Width, Height);

	// Register page with Viewport Client to receive input
	if (GEngine && GEngine->GameViewport)
	{
		UBluViewPort* ViewportClient = Cast<UBluViewPort>(GEngine->GameViewport);
		if (ViewportClient)
		{
			ViewportClient->RegisterHudUI(this);
		}
	}
}

void UBluHUD::BeginDestroy()
{
	// Unregister page from Viewport
	if (GEngine && GEngine->GameViewport)
	{
		UBluViewPort* ViewportClient = Cast<UBluViewPort>(GEngine->GameViewport);
		if (ViewportClient)
		{
			ViewportClient->UnregisterHudUI(this);
		}
	}

	Super::BeginDestroy();
}

void UBluHUD::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// Here we will ppdate mouse position
	FVector2D MousePosition = FVector2D::ZeroVector;
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//////////////////////////////////////////////////////////////////////////
// View control

FVector2D UBluHUD::GetScreenPosition() const
{
	return ScreenPosition;
}

void UBluHUD::SetScreenPosition(float ScreenX, float ScreenY)
{
	ScreenPosition = FVector2D(ScreenX, ScreenY);
}

FVector2D UBluHUD::GetScreenSize() const
{
	return ScreenSize;
}

void UBluHUD::SetScreenSize(float ScreenWidth, float ScreenHeight)
{
	ScreenSize = FVector2D(ScreenWidth, ScreenHeight);
}