/*
*    This file is used to facilitate the linking process of the
*    windows loader to the default entry point WinMain which is
*    specified in the Microsoft's documentation for the windowed app.
*/

#ifdef NDEBUG // Only compile this file in `Release` configuration cause it's not necessary when using `Windows` subsystem

#define SDL_MAIN_HANDLED        // Manually handle SDL main function

/*   This macro got from https://github.com/libsdl-org/SDL/blob/SDL2/src/core/windows/SDL_windows.h line 98
*    Necessary for main_getcmdline() to work properly
*/
#define WIN_StringToUTF8W(S) SDL_iconv_string("UTF-8", "UTF-16LE", (const char *)(S), (SDL_wcslen(S) + 1) * sizeof(WCHAR))

#include <SDL2/SDL.h>
#include <windows.h>


int main(int argc, char* argv[]);
static BOOL OutOfMemory(void);
int main_getcmdline(void);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


/* entry point for windowed build, used when switch to `Release` configuration */
int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrev, _In_ LPWSTR szCmdLine, _In_ int sw)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInst;
	RegisterClass(&wc);
	return main_getcmdline();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HBRUSH hBrush = CreateSolidBrush(RGB(50, 25, 50));
    if (uMsg == WM_DESTROY) PostQuitMessage(0);
    else if (uMsg == WM_PAINT) {
        PAINTSTRUCT pStructure;
        HDC hdc = BeginPaint(hwnd, &pStructure);
        FillRect(hdc, &pStructure.rcPaint, hBrush);
        TRIVERTEX v[] = { {0, 0, 0x5000, 0x5000, 0x8000, 0x0000}, {pStructure.rcPaint.right, pStructure.rcPaint.bottom, 0x0000, 0x5000, 0x8000, 0x0000} };
        GRADIENT_RECT g = { 0, 1 };
    }
    else if (uMsg == WM_SETFOCUS || uMsg == WM_KILLFOCUS) {
        hBrush = CreateSolidBrush(uMsg == WM_SETFOCUS ? RGB(50, 50, 50) : RGB(30, 30, 30));
        InvalidateRect(hwnd, NULL, TRUE);
    }
    else return DefWindowProc(hwnd, uMsg, wParam, lParam);
    return 0;
}

/*
    The code below was grabbed from https://github.com/libsdl-org/SDL/blob/SDL2/src/main/windows/SDL_windows_main.c
	for the implementation of OutOfMemory() and main_getcmdline() and other WIN32 platform specific functions
*/

/* Pop up an out of memory message, returns to Windows */
static BOOL OutOfMemory(void)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", "Out of memory - aborting", NULL);
    return FALSE;
}

#if defined(_MSC_VER)
/* The VC++ compiler needs main/wmain defined */
#define console_ansi_main main
#if UNICODE
#define console_wmain wmain
#endif
#endif

/* Gets the arguments with GetCommandLine, converts them to argc and argv
    and calls SDL_main */
static int main_getcmdline(void)
{
    LPWSTR* argvw;
    char** argv;
    int i, argc, result;

    argvw = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!argvw) {
        return OutOfMemory();
    }

    /* Note that we need to be careful about how we allocate/free memory here.
        * If the application calls SDL_SetMemoryFunctions(), we can't rely on
        * SDL_free() to use the same allocator after SDL_main() returns.
        */

        /* Parse it into argv and argc */
    argv = (char**)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (argc + 1) * sizeof(*argv));
    if (!argv) {
        return OutOfMemory();
    }
    for (i = 0; i < argc; ++i) {
        DWORD len;
        char* arg = WIN_StringToUTF8W(argvw[i]);
        if (!arg) {
            return OutOfMemory();
        }
        len = (DWORD)SDL_strlen(arg);
        argv[i] = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (size_t)len + 1);
        if (!argv[i]) {
            return OutOfMemory();
        }
        SDL_memcpy(argv[i], arg, len);
        SDL_free(arg);
    }
    argv[i] = NULL;
    LocalFree(argvw);

    SDL_SetMainReady();

    /* Run the application main() code */
    result = main(argc, argv);

    /* Free argv, to avoid memory leak */
    for (i = 0; i < argc; ++i) {
        HeapFree(GetProcessHeap(), 0, argv[i]);
    }
    HeapFree(GetProcessHeap(), 0, argv);

    return result;
}

/* This is where execution begins [console apps, ansi] */

int console_ansi_main_point(int argc, char* argv[])
{
    return main_getcmdline();
}

#if UNICODE

/* This is where execution begins [console apps, unicode] */

int console_wmain_point(int argc, wchar_t* wargv[], wchar_t* wenvp)
{
    return main_getcmdline();
}
#endif

/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2025 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifdef __WIN32_SDL_WRAPPER_CXX_


#if defined(__WIN32__) || defined(__WINRT__) || defined(__GDK__)

#if defined(HAVE_ROAPI_H)
typedef enum RO_INIT_TYPE
{
    RO_INIT_SINGLETHREADED = 0,
    RO_INIT_MULTITHREADED = 1
} RO_INIT_TYPE;
#endif

#ifndef _WIN32_WINNT_VISTA
#define _WIN32_WINNT_VISTA 0x0600
#endif
#ifndef _WIN32_WINNT_WIN7
#define _WIN32_WINNT_WIN7 0x0601
#endif
#ifndef _WIN32_WINNT_WIN8
#define _WIN32_WINNT_WIN8 0x0602
#endif

#ifndef LOAD_LIBRARY_SEARCH_SYSTEM32
#define LOAD_LIBRARY_SEARCH_SYSTEM32 0x00000800
#endif

/* Sets an error message based on an HRESULT */
int WIN_SetErrorFromHRESULT(const char* prefix, HRESULT hr)
{
    TCHAR buffer[1024];
    char* message;
    TCHAR* p = buffer;
    DWORD c = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, 0,
        buffer, SDL_arraysize(buffer), NULL);
    buffer[c] = 0;
    /* kill CR/LF that FormatMessage() sticks at the end */
    while (*p) {
        if (*p == '\r') {
            *p = 0;
            break;
        }
        ++p;
    }
    message = WIN_StringToUTF8(buffer);
    SDL_SetError("%s%s%s", prefix ? prefix : "", prefix ? ": " : "", message);
    SDL_free(message);
    return -1;
}

/* Sets an error message based on GetLastError() */
int WIN_SetError(const char* prefix)
{
    return WIN_SetErrorFromHRESULT(prefix, GetLastError());
}

HRESULT
WIN_CoInitialize(void)
{
    /* SDL handles any threading model, so initialize with the default, which
       is compatible with OLE and if that doesn't work, try multi-threaded mode.

       If you need multi-threaded mode, call CoInitializeEx() before SDL_Init()
    */
#ifdef __WINRT__
    /* DLudwig: On WinRT, it is assumed that COM was initialized in main().
       CoInitializeEx is available (not CoInitialize though), however
       on WinRT, main() is typically declared with the [MTAThread]
       attribute, which, AFAIK, should initialize COM.
    */
    return S_OK;
#elif defined(__XBOXONE__) || defined(__XBOXSERIES__)
    /* On Xbox, there's no need to call CoInitializeEx (and it's not implemented) */
    return S_OK;
#else
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (hr == RPC_E_CHANGED_MODE) {
        hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    }

    /* S_FALSE means success, but someone else already initialized. */
    /* You still need to call CoUninitialize in this case! */
    if (hr == S_FALSE) {
        return S_OK;
    }

    return hr;
#endif
}

void WIN_CoUninitialize(void)
{
#ifndef __WINRT__
    CoUninitialize();
#endif
}

char* WIN_LookupAudioDeviceName(const WCHAR* name, const GUID* guid)
{
#if defined(__WINRT__) || defined(__XBOXONE__) || defined(__XBOXSERIES__)
    return WIN_StringToUTF8W(name); /* No registry access on WinRT/UWP and Xbox, go with what we've got. */
#else
    static const GUID nullguid = { 0 };
    const unsigned char* ptr;
    char keystr[128];
    WCHAR* strw = NULL;
    SDL_bool rc;
    HKEY hkey;
    DWORD len = 0;
    char* retval = NULL;

    if (WIN_IsEqualGUID(guid, &nullguid)) {
        return WIN_StringToUTF8(name); /* No GUID, go with what we've got. */
    }

    ptr = (const unsigned char*)guid;
    (void)SDL_snprintf(keystr, sizeof(keystr),
        "System\\CurrentControlSet\\Control\\MediaCategories\\{%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
        ptr[3], ptr[2], ptr[1], ptr[0], ptr[5], ptr[4], ptr[7], ptr[6],
        ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15]);

    strw = WIN_UTF8ToString(keystr);
    rc = (RegOpenKeyExW(HKEY_LOCAL_MACHINE, strw, 0, KEY_QUERY_VALUE, &hkey) == ERROR_SUCCESS);
    SDL_free(strw);
    if (!rc) {
        return WIN_StringToUTF8(name); /* oh well. */
    }

    rc = (RegQueryValueExW(hkey, L"Name", NULL, NULL, NULL, &len) == ERROR_SUCCESS);
    if (!rc) {
        RegCloseKey(hkey);
        return WIN_StringToUTF8(name); /* oh well. */
    }

    strw = (WCHAR*)SDL_malloc(len + sizeof(WCHAR));
    if (!strw) {
        RegCloseKey(hkey);
        return WIN_StringToUTF8(name); /* oh well. */
    }

    rc = (RegQueryValueExW(hkey, L"Name", NULL, NULL, (LPBYTE)strw, &len) == ERROR_SUCCESS);
    RegCloseKey(hkey);
    if (!rc) {
        SDL_free(strw);
        return WIN_StringToUTF8(name); /* oh well. */
    }

    strw[len / 2] = 0; /* make sure it's null-terminated. */

    retval = WIN_StringToUTF8(strw);
    SDL_free(strw);
    return retval ? retval : WIN_StringToUTF8(name);
#endif /* if __WINRT__ / else */
}

BOOL WIN_IsEqualGUID(const GUID* a, const GUID* b)
{
    return SDL_memcmp(a, b, sizeof(*a)) == 0;
}

BOOL WIN_IsEqualIID(REFIID a, REFIID b)
{
    return SDL_memcmp(a, b, sizeof(*a)) == 0;
}

void WIN_RECTToRect(const RECT* winrect, SDL_Rect* sdlrect)
{
    sdlrect->x = winrect->left;
    sdlrect->w = (winrect->right - winrect->left) + 1;
    sdlrect->y = winrect->top;
    sdlrect->h = (winrect->bottom - winrect->top) + 1;
}

void WIN_RectToRECT(const SDL_Rect* sdlrect, RECT* winrect)
{
    winrect->left = sdlrect->x;
    winrect->right = sdlrect->x + sdlrect->w - 1;
    winrect->top = sdlrect->y;
    winrect->bottom = sdlrect->y + sdlrect->h - 1;
}

BOOL WIN_IsRectEmpty(const RECT* rect)
{
    /* Calculating this manually because UWP and Xbox do not support Win32 IsRectEmpty. */
    return (rect->right <= rect->left) || (rect->bottom <= rect->top);
}
#endif /* defined(__WIN32__) || defined(__WINRT__) || defined(__GDK__) */

/*
 * Public APIs
 */
#if !defined(SDL_VIDEO_DRIVER_WINDOWS)

#if defined(__WIN32__) || defined(__GDK__)
int SDL_RegisterApp(const char* name, Uint32 style, void* hInst)
{
    (void)name;
    (void)style;
    (void)hInst;
    return 0;
}

void SDL_UnregisterApp(void)
{
}

void SDL_SetWindowsMessageHook(SDL_WindowsMessageHook callback, void* userdata)
{
}
#endif /* __WIN32__ || __GDK__ */

#if defined(__WIN32__) || defined(__WINGDK__)
int SDL_Direct3D9GetAdapterIndex(int displayIndex)
{
    (void)displayIndex;
    return 0; /* D3DADAPTER_DEFAULT */
}

SDL_bool SDL_DXGIGetOutputInfo(int displayIndex, int* adapterIndex, int* outputIndex)
{
    (void)displayIndex;
    if (adapterIndex) {
        *adapterIndex = -1;
    }
    if (outputIndex) {
        *outputIndex = -1;
    }
    return SDL_FALSE;
}
#endif /* __WIN32__ || __WINGDK__ */

#endif /* !SDL_VIDEO_DRIVER_WINDOWS */
#endif /* __WIN32_SDL_WRAPPER__CXX_ */

/* vi: set ts=4 sw=4 expandtab: */

#endif /* NDEBUG */
