#include "../Private/BluPrivatePCH.h"

RenderHandler::RenderHandler(int32 width, int32 height, UBluWidget* ui)
{
	this->width = width;
	this->height = height;
	this->parentUI = ui;
}

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	rect = CefRect(0, 0, width, height);
	return true;
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	// Bitmap header information
	BITMAPINFOHEADER info;
	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = width;
	info.biHeight = -height;  // minus means top-down bitmap
	info.biPlanes = 1;
	info.biBitCount = 32;
	info.biCompression = BI_RGB;  // no compression
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 1;
	info.biYPelsPerMeter = 1;
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	// Reset buffer data
	buffer_data = NULL;

	// Start creating a bitmap here
	HDC screen_dc = GetDC(NULL);
	HBITMAP bitmap = CreateDIBSection(screen_dc, reinterpret_cast<BITMAPINFO*>(&info), DIB_RGB_COLORS, &buffer_data, NULL, 0);
	ReleaseDC(NULL, screen_dc);

	if (bitmap == NULL)
		return;

	// Copy buffer into new bitmap in buffer_data
	memcpy(buffer_data, buffer, width * height * 4);

	// Trigger our parent UIs Texture to update
	parentUI->TextureUpdate();

}

bool BrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{

	FString name = message->GetArgumentList()->GetString(0).c_str();
	FString data = message->GetArgumentList()->GetString(1).c_str();
	FString type = message->GetArgumentList()->GetString(2).c_str();

	if (type == "js_event")
	{
		event_emitter->Broadcast(name, data);
	}

	return true;

}

void BrowserClient::SetEventEmitter(FScriptEvent* emitter)
{
	this->event_emitter = emitter;
}