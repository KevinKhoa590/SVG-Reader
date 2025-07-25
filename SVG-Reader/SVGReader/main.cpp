#include "stdafx.h"
#include "rapidxml.hpp"
#include "SVGParser.h"
#include "SVGRenderer.h"
#include "SVGElement.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")

// Global: parsed elements and color pool container
SVGParseResult svgResult;
vector<unique_ptr<SVGElement>>& elements = svgResult.elements;

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    static SVGRenderer renderer;
    renderer.render(svgResult, graphics);
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND hWnd;
    MSG msg;
    WNDCLASS wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    // Initialize GDI+
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // === Parse the SVG file ===
    svgResult = SVGParser::parse("sample.svg");

    // Register window class
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("SVGWindow");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("SVGWindow"), TEXT(""),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    // Message loop
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
