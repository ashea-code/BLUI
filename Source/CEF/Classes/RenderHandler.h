#include "AllowWindowsPlatformTypes.h"
#include "include/cef_client.h"
#include "include/cef_app.h"
#include "HideWindowsPlatformTypes.h"

class RenderHandler : public CefRenderHandler
{
public:

	UCHAR* buffer_data;

	// CefRenderHandler interface
	bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
	{
		rect = CefRect(0, 0, 800, 600);
		return true;
	};

	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
	{
		FMemory::Memcpy(buffer_data, buffer, width*height*sizeof(uint32));
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

	CefRefPtr<RenderHandler> m_renderHandler;

	virtual CefRefPtr<RenderHandler> GetRenderHandlerCustom() {
		return m_renderHandler;
	};

	// NOTE: Must be at bottom
public:
	IMPLEMENT_REFCOUNTING(BrowserClient)
};