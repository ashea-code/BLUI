#pragma once
#include "../Private/BluPrivatePCH.h"

class RenderHandler : public CefRenderHandler
{
	public:

		void* buffer_data = NULL;

		int32 width;
		int32 height;

		// CefRenderHandler interface
		bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;

		void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

		RenderHandler::RenderHandler(int32 width, int32 height);

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