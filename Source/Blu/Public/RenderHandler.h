#pragma once

#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#endif
#include "include/cef_client.h"
#include "include/cef_app.h"
#if PLATFORM_WINDOWS
#include "HideWindowsPlatformTypes.h"
#endif

#include "../Public/BluEye.h"


class RenderHandler : public CefRenderHandler
{
	private:
		UBluEye* parentUI;

	public:

		int32 Width;
		int32 Height;

		// CefRenderHandler interface
		bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;

		void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

		RenderHandler(int32 width, int32 height, UBluEye* ui);

		// CefBase interface
		// NOTE: Must be at bottom
	public:
		IMPLEMENT_REFCOUNTING(RenderHandler)
};

// for manual render handler
class BrowserClient : public CefClient
{

	private:
		FScriptEvent* event_emitter;

	public:
		BrowserClient(RenderHandler* renderHandler) : m_renderHandler(renderHandler)
		{
		
		};

		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() 
		{
			return m_renderHandler;
		};

		CefRefPtr<RenderHandler> m_renderHandler;

		virtual CefRefPtr<RenderHandler> GetRenderHandlerCustom()
		{
			return m_renderHandler;
		};

		virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;
		void SetEventEmitter(FScriptEvent* emitter);

		// NOTE: Must be at bottom
	public:
		IMPLEMENT_REFCOUNTING(BrowserClient)
};