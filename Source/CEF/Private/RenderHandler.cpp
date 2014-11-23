#include "../Private/BluPrivatePCH.h"

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	rect = CefRect(0, 0, 800, 600);
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
}