// Surface Modeler.cpp : Defines the entry point for the application.
//


#include <windows.h>
#include <objidl.h>

#include "framework.h"
#include "SurfaceModelerMain.h"
#include "ThreadedRenderer.h"
#include "BoxSurface.h"

using namespace SurfaceModeler;

#define MAX_LOADSTRING 100
#define WM_USER_RENDER_COMPLETE (WM_USER + 100)

constexpr auto WIDTH = 1024;
constexpr auto HEIGHT = 768;

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hWnd;                                      // main window
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HBITMAP hbitmap = NULL;

std::unique_ptr<ThreadedRenderer> renderer = nullptr;
std::shared_ptr<Surface> surface = nullptr;
std::unique_ptr<BYTE[]> buffer = std::unique_ptr<BYTE[]>(new BYTE[WIDTH * HEIGHT * 3]);

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SURFACEMODELER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SURFACEMODELER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SURFACEMODELER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SURFACEMODELER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

void initializeRenderer() {

    auto onRender = [&] {
        int bufferIndex = 0;

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                Vector pixel = renderer->getBuffer(x, y);

                BYTE r = (BYTE)(pixel.x * 255.0f);
                BYTE g = (BYTE)(pixel.y * 255.0f);
                BYTE b = (BYTE)(pixel.z * 255.0f);

                buffer[bufferIndex++] = b;
                buffer[bufferIndex++] = g;
                buffer[bufferIndex++] = r;
            }
        }

        PostMessage(hWnd, WM_USER_RENDER_COMPLETE, NULL, NULL);
    };

    surface = std::shared_ptr<BoxSurface>(new BoxSurface(Vector(0.5f, 0.2f, 0.5f), 0.05f));
    renderer = std::unique_ptr<ThreadedRenderer>(new ThreadedRenderer(WIDTH, HEIGHT, surface, onRender));

    for (int i = 0; i < renderer->getWidth() * renderer->getHeight() * 3; i++) {
        buffer[i] = 0;
    }

    renderer->initialize();
    renderer->startRender();
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   initializeRenderer();

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYUP:
        {
            switch (wParam)
            {
            case VK_SPACE:
                renderer->startRender();
                break;
            }
            break;
        }
    case WM_USER_RENDER_COMPLETE:
        RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            BITMAPINFO info;
            ZeroMemory(&info, sizeof(BITMAPINFO));
            info.bmiHeader.biBitCount = 24;
            info.bmiHeader.biWidth = WIDTH;
            info.bmiHeader.biHeight = HEIGHT;
            info.bmiHeader.biPlanes = 1;
            info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            info.bmiHeader.biSizeImage = WIDTH * HEIGHT;
            info.bmiHeader.biCompression = BI_RGB;

            StretchDIBits(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, buffer.get(), &info, DIB_RGB_COLORS, SRCCOPY);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        renderer->exit();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
