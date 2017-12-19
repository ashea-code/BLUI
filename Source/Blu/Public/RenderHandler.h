#pragma once

#if PLATFORM_WINDOWS
#include "WindowsHWrapper.h"
#include "AllowWindowsPlatformTypes.h"
#include "AllowWindowsPlatformAtomics.h"
#endif
#pragma push_macro("OVERRIDE")
#undef OVERRIDE // cef headers provide their own OVERRIDE macro
THIRD_PARTY_INCLUDES_START
#include "include/cef_client.h"
#include "include/cef_app.h"
THIRD_PARTY_INCLUDES_END
#pragma pop_macro("OVERRIDE")
#if PLATFORM_WINDOWS
#include "HideWindowsPlatformAtomics.h"
#include "HideWindowsPlatformTypes.h"
#endif

#include "../Public/BluEye.h"


class RenderHandler : public CefRenderHandler
{
	public:
		UBluEye* parentUI;

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
class BrowserClient : public CefClient, public CefLifeSpanHandler, public CefDownloadHandler, public CefDisplayHandler
{

	private:
		FScriptEvent* event_emitter;
		CefRefPtr<RenderHandler> m_renderHandler;

		// For lifespan
		CefRefPtr<CefBrowser> m_Browser;
		int m_BrowserId;
		bool m_bIsClosing;

	public:
		BrowserClient(RenderHandler* renderHandler) : m_renderHandler(renderHandler)
		{
		
		};

		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() 
		{
			return m_renderHandler;
		};

		// Getter for renderer
		virtual CefRefPtr<RenderHandler> GetRenderHandlerCustom()
		{
			return m_renderHandler;
		};

		// Getter for lifespan
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override
		{
			return this;
		}

		virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;
		void SetEventEmitter(FScriptEvent* emitter);

		//CefDownloadHandler
		virtual void OnBeforeDownload(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDownloadItem> download_item,
			const CefString& suggested_name,
			CefRefPtr<CefBeforeDownloadCallback> callback) override;

		virtual void OnDownloadUpdated(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDownloadItem> download_item,
			CefRefPtr<CefDownloadItemCallback> callback) override;

		//CefLifeSpanHandler
		virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& target_url,
			const CefString& target_frame_name,
			WindowOpenDisposition target_disposition,
			bool user_gesture,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings,
			bool* no_javascript_access) override
		{
			return false;
		}

		// Lifespan methods
		void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
		void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

		// NOTE: Must be at bottom
	public:
		IMPLEMENT_REFCOUNTING(BrowserClient)
};