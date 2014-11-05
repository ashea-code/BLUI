#pragma once
#include "../Classes/RenderHandler.h"
#include "AllowWindowsPlatformTypes.h"
#include "include/cef_client.h"
#include "include/cef_app.h"
#include "HideWindowsPlatformTypes.h"

class CEF_API CEFInstance
{
	private:
		CefRefPtr<BrowserClient> g_handler;
		CefRefPtr<CefBrowser> browser;
		CefBrowserSettings browserSettings;
		CefSettings settings;
		CefWindowInfo info;
		RenderHandler* renderer = new RenderHandler();
	
	public:
		CEFInstance();
		~CEFInstance();

};