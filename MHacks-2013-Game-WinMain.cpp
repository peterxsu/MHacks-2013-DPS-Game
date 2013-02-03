#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <wincodec.h>
#include <d2d1.h>

#include "Display.h"
#include "EasyBMP.h"
#include "backendMain.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "WindowsCodecs.lib")

#include <ShObjIdl.h>

PCWSTR FilePath;

template <class DERIVED_TYPE>
class BaseWindow {
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	DERIVED_TYPE *pThis = NULL;

	if (uMsg == WM_CREATE)
	{
		CREATESTRUCT *pCreate = (CREATESTRUCT*)lParam;
		pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

		pThis->m_hwnd = hwnd;
	}
	else {
		pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}

	if (pThis){
		return pThis->HandleMessage(uMsg, wParam, lParam);
	}
	else {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
	BaseWindow() : m_hwnd(NULL) { }

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0) {

	WNDCLASS wc = {0};

	wc.lpfnWndProc	= DERIVED_TYPE::WindowProc;
	wc.hInstance	= GetModuleHandle(NULL);
	wc.lpszClassName= ClassName();

	RegisterClass(&wc);

	m_hwnd = CreateWindowEx(
		dwExStyle,
		ClassName(),
		lpWindowName,
		dwStyle,
		x, y,
		nWidth, nHeight,
		hWndParent,
		hMenu,
		GetModuleHandle(NULL),
		this);
	
	return (m_hwnd ? TRUE : FALSE);
}

	HWND Window() const { return m_hwnd; }

protected:
	virtual PCWSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
};

class MainWindow : public BaseWindow<MainWindow> {
	ID2D1Factory			*pFactory;
	IWICImagingFactory		*pIWICFactory;
	ID2D1HwndRenderTarget	*pRenderTarget;

	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();
	void OnPaint();

public:
	MainWindow() : pFactory(NULL), pIWICFactory(NULL), pRenderTarget(NULL) { }

	PCWSTR ClassName() const { return L"MHacks Game"; }

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

HRESULT LoadBitmapFromFile(
	ID2D1RenderTarget *pRenderTarget,
	IWICImagingFactory *pIWICFactory,
	PCWSTR FileName,
	ID2D1Bitmap **ppBitmap);

template <class T> void SafeRelease(T **ppT);

long GetDesktopSizeHorizontal();
long GetDesktopSizeVertical();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	try {
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (FAILED(hr)) { throw 1; };

		MainWindow win;

		if (!win.Create(
			L"MHacks Game",
			WS_OVERLAPPED,
			0,
			(GetDesktopSizeHorizontal()/2) - 350,
			(GetDesktopSizeVertical()/2) - 250,
			700,
			500,
			NULL,
			NULL)) {

			return 0;
		}

		ShowWindow(win.Window(), nCmdShow);

		FilePath = (PWSTR)L"display.bmp";

		initialize();

		MSG msg = { };
		while (GetMessage(&msg, NULL, 0, 0)) {
			gameActions();

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	catch(int) { };

	return 0;
}

HRESULT MainWindow::CreateGraphicsResources() {
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, size), &pRenderTarget);

		if (FAILED(hr)) { throw 1; }
	}

	return hr;
}

void MainWindow::DiscardGraphicsResources() {
	SafeRelease(&pRenderTarget);
}

void MainWindow::OnPaint() {
	ID2D1Bitmap *pBitmap;

	LoadBitmapFromFile(
		pRenderTarget,
		pIWICFactory,
		FilePath,
		&pBitmap);

	pRenderTarget->BeginDraw();

	pRenderTarget->DrawBitmap(
		pBitmap,
		D2D1::Rect(0, 0, 700, 500));

	pRenderTarget->EndDraw();

	SafeRelease(&pBitmap);
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HRESULT hr;
	
	switch (uMsg)
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) {
			return -1;
		}
		CreateGraphicsResources();

		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID*)&pIWICFactory);

		if (FAILED(hr)) { return -1; }

		return 0;

	case WM_DESTROY:
		DiscardGraphicsResources();

		SafeRelease(&pFactory);
		SafeRelease(&pIWICFactory);

		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		{
			OnPaint();
			return 0;
		}

		return 0;
	}

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

HRESULT LoadBitmapFromFile(
	ID2D1RenderTarget *pRenderTarget,
	IWICImagingFactory *pIWICFactory,
	PCWSTR FileName,
	ID2D1Bitmap **ppBitmap) {

	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICFormatConverter *pConverter = NULL;

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
		FileName,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder);
	while(FilePath == NULL) { }

	if (FAILED(hr)) { throw 'a'; }
	hr = pDecoder->GetFrame(0, &pSource);

	if (FAILED(hr)) { throw 'b'; }
	hr = pIWICFactory->CreateFormatConverter(&pConverter);

	if (FAILED(hr)) { throw 'c'; }
	hr = pConverter->Initialize(
		pSource,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut);

	if (FAILED(hr)) { throw 'd'; }
	hr = pRenderTarget->CreateBitmapFromWicBitmap(
		pConverter,
		NULL,
		ppBitmap);

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pConverter);
}

template <class T> void SafeRelease(T **ppT) {
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

long GetDesktopSizeHorizontal() {
	RECT desktop;

	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	return desktop.right;
}

long GetDesktopSizeVertical() {
	RECT desktop;

	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	return desktop.bottom;
}