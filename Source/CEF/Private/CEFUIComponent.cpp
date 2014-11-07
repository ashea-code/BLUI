#include "../Private/CEFPrivatePCH.h"
#include "../classes/CEFUIComponent.h"

UCEFUIComponent::UCEFUIComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	g_handler = new BrowserClient(renderer);
	browser = CefBrowserHost::CreateBrowserSync(CEFManager::info, g_handler.get(), "about:blank", browserSettings, NULL);
}

void UCEFUIComponent::initComponent()
{
	CefString str = *DefaultURL;
	UE_LOG(LogCEF, Warning, TEXT("TEST: %s"), *DefaultURL);
}