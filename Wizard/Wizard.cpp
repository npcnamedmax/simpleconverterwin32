// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "converter.h"

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("Application");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND textbox; //handle to text box
HWND hButton; //handle to button
static HWND hMenu;
static HWND outputbox;

WCHAR textinput[1000] = L"TEST"; //text from input box
TCHAR strText[255] = TEXT("\0"); //bin/hex/dec selected from combobox
bool selected = false; //tests if value has been selected from combobox
const TCHAR* items[] = { TEXT("hex"), TEXT("bin"), TEXT("dec")};

result res;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1010, 500,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, Windows desktop!");

    switch (message)
    {
    case WM_CREATE:
        textbox = CreateWindowEx(WS_EX_LEFT, L"EDIT", L"", WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 10, 700, 20, hWnd, (HMENU)2, NULL, NULL);
        hButton = CreateWindowEx(WS_EX_LEFT, L"BUTTON", L"ENTER", WS_CHILD | WS_BORDER | WS_VISIBLE, 920, 10, 60, 20, hWnd, (HMENU)1, NULL, NULL);
        hMenu = CreateWindowEx(0, L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWN, 720, 10, 100, 200, hWnd, NULL, hInst, NULL);
        outputbox = CreateWindowEx(WS_EX_LEFT, L"EDIT", L"", WS_CHILD | WS_BORDER | WS_VISIBLE | ES_READONLY | ES_MULTILINE, 10, 40, 700, 100, hWnd, NULL, NULL, NULL);
        int i;
        for (i = 0; i < 3; i++)
        {
            //populate combo box
            SendMessage(hMenu, CB_ADDSTRING, 0, (LPARAM)items[i]);
        }
        break;
    case WM_KEYDOWN:
    {
        switch (wParam) {
        case VK_RETURN:
            SendMessageA(hButton, BM_CLICK, 0, 0);
        }
        break;

    }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, Windows desktop!"
        // in the top left corner.

        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1: //button, text input box is 2
            GetWindowText(textbox, textinput, 1000);
            if (selected == true) {
                //to convert tchar to string, note that the below method only works with single byte characters
                wstring ws1(textinput);
                wstring ws2(strText);
                string str1(ws1.begin(), ws1.end());
                string str2(ws2.begin(), ws2.end());
                res = engine(str1, str2);
                if (res.invInput == false) { //tests input 
                    wstring output = res.concatenate();
                    //convert back to LPCWSTR
                    LPCWSTR str = output.c_str();
                    SetWindowText(outputbox, str);
                }
                else if (res.invInput == true) {
                    MessageBox(hWnd, L"Invalid input", L"Error", MB_OK);
                }
            }
            else MessageBox(hWnd, L"Please select type", L"Error", MB_OK);
            break;

        }
        if (HIWORD(wParam) == CBN_SELCHANGE) {
            selected = true;
            //get position of selected item
            LRESULT sel = SendMessage(hMenu, CB_GETCURSEL, 0, 0);
            //get selected item text
            SendMessage(hMenu, CB_GETLBTEXT, sel, (LPARAM)strText);
            SetFocus(hWnd);
        } break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;


        return 0;
    }
}

LRESULT CALLBACK SubclassedWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        if (wParam == VK_RETURN)
        {
            // Get the handle of the control that has focus
            HWND hFocus = GetFocus();
            // Get the ID of the control that has focus
            int id = GetDlgCtrlID(hFocus);
            if (id == 2) // ID matches text input box
            {
                SendMessageA(hButton, BM_CLICK, 0, 0);
            }
        }
        break;

    default:
        // Pass the message on to the original WndProc
        return CallWindowProc(WndProc, hWnd, message, wParam, lParam);
    }

    return 0;
}

//error handling for invalid value 










