// SurfaceModeler.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include <objidl.h>
#include <iostream>

#include "framework.h"
#include "SurfaceModelerMain.h"
#include "ThreadedRenderer.h"
#include "BoxSurface.h"
#include "Scene.h"
#include "SampleScenes.h"
#include "SceneReader.h"

using namespace SurfaceModeler;

#define WM_USER_RENDER_COMPLETE (WM_USER + 100)

constexpr auto WIDTH = 1024;
constexpr auto HEIGHT = 768;

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hWnd;                                      // main window
LPCWSTR className = L"SURFACEMODELER";
LPCWSTR windowTitle = L"Surface Modeler";
HBITMAP hbitmap = NULL;

std::unique_ptr<ThreadedRenderer> renderer = nullptr;
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

    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
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
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszClassName  = className;

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

    SceneReader reader;
    auto surfaces = reader.surfacesForScene(SampleScenes::roundedCube());
    renderer = std::unique_ptr<ThreadedRenderer>(new ThreadedRenderer(WIDTH, HEIGHT, surfaces[0], onRender));

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

   hWnd = CreateWindowW(className, windowTitle, WS_OVERLAPPEDWINDOW,
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
