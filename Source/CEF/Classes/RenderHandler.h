#pragma once
#include "AllowWindowsPlatformTypes.h"
#include "include/cef_client.h"
#include "include/cef_app.h"
#include "HideWindowsPlatformTypes.h"
//#include "../Private/BluPrivatePCH.h"

class RenderHandler : public CefRenderHandler
{
public:

	void* buffer_data = NULL;

	// CefRenderHandler interface
	bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;

	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

	// CefBase interface
	// NOTE: Must be at bottom
public:
	IMPLEMENT_REFCOUNTING(RenderHandler)
};

// for manual render handler
class BrowserClient : public CefClient
{
public:
	BrowserClient(RenderHandler* renderHandler) : m_renderHandler(renderHandler)
	{
		
	};

	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
		return m_renderHandler;
	};

	CefRefPtr<RenderHandler> m_renderHandler;

	virtual CefRefPtr<RenderHandler> GetRenderHandlerCustom() {
		return m_renderHandler;
	};

	// NOTE: Must be at bottom
public:
	IMPLEMENT_REFCOUNTING(BrowserClient)
};