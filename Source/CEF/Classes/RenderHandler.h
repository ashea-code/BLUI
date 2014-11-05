#include "AllowWindowsPlatformTypes.h"
#include "include/cef_client.h"
#include "include/cef_app.h"
#include "HideWindowsPlatformTypes.h"

class RenderHandler : public CefRenderHandler
{
public:
	// CefRenderHandler interface
	bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
	{
		rect = CefRect(0, 0, 800, 600);
		return true;
	};

	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
	{
		//memcpy(texBuf->getCurrentLock().data, buffer, width*height*4);
	};

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
		// BOOST_LOG_TRIVIAL(info) << "here 1b";
	};

	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
		return m_renderHandler;
	};

	CefRefPtr<CefRenderHandler> m_renderHandler;

	// NOTE: Must be at bottom
public:
	IMPLEMENT_REFCOUNTING(BrowserClient)
};