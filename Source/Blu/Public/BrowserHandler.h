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
class BrowserClient : public CefClient, CefDownloadHandler, CefLifeSpanHandler, CefDisplayHandler
{

	private:
		FScriptEvent* event_emitter;
		UBluEye* blu;

	public:
		BrowserClient(RenderHandler* renderHandler, UBluEye* bluEye) : m_renderHandler(renderHandler)
		{
			blu = bluEye;
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

		virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() override {
			return this;
		}

		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
			return this;
		}

		virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override {
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
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override {
			// Browser window created successfully...
		}
		virtual bool DoClose(CefRefPtr<CefBrowser> browser) override {
			// Allow or block browser window close...
			return true;
		}
		virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override {
			// Browser window is closed, perform cleanup...
		}

		//Fullscreen handling - doesn't work on this patch
		/*virtual void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser, bool fullscreen) override{
			return;
		}*/


		// NOTE: Must be at bottom
	public:
		IMPLEMENT_REFCOUNTING(BrowserClient)
};