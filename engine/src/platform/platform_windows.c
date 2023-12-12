#include "platform/platform.h"

#ifdef PLATFORM_WINDOWS

#include <windows.h>
#include <windowsx.h>

#include <stdlib.h>

#include "defines.h"
#include "core/logger.h"

typedef struct {
	HINSTANCE instanceHandle;
	HWND windowHandle;
} internal_state;

static f64 clockFrequency;
static LARGE_INTEGER startTime;

LRESULT CALLBACK win32_process_message(HWND windowHandle, u32 message, WPARAM wparam, LPARAM lparam);

//
// external
//

b8 platform_create(
		platform_state* state,
		const char* applicationName,
		i32 x,
		i32 y,
		i32 width,
		i32 height) {

	HINSTANCE instance = GetModuleHandleA(0);
	const char windowClassName[] = "arc_window_class";
	
	HICON icon = LoadIcon(instance, IDI_APPLICATION);
	WNDCLASSA windowClass;
	memset(&windowClass, 0, sizeof(windowClass));
	windowClass.style         = CS_DBLCLKS;
	windowClass.lpfnWndProc   = win32_process_message;
	windowClass.cbClsExtra    = 0;
	windowClass.cbWndExtra    = 0;
	windowClass.hInstance     = instance;
	windowClass.hIcon         = icon;
	windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszClassName = windowClassName;

	if (!RegisterClassA(&windowClass)) {
		log_error("window registration failed!");
		MessageBoxA(null, "window registration failed!", "error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	u32 clientX      = x;
	u32 clientY      = y;
	u32 clientWidth  = width;
	u32 clientHeight = height;

	u32 windowX      = clientX;
	u32 windowY      = clientY;
	u32 windowWidth  = clientWidth;
	u32 windowHeight = clientHeight;

	u32 windowStyle = 0;
	windowStyle |= WS_OVERLAPPED;
	windowStyle |= WS_SYSMENU;
	windowStyle |= WS_CAPTION;
	windowStyle |= WS_MAXIMIZEBOX;
	windowStyle |= WS_MINIMIZEBOX;
	windowStyle |= WS_THICKFRAME;

	u32 windowExStyle = 0;
	windowExStyle |= WS_EX_APPWINDOW;

	RECT borderRect = {0, 0, 0, 0};
	AdjustWindowRectEx(&borderRect, windowStyle, 0, windowExStyle);

	windowX += borderRect.left;
	windowY += borderRect.top;
	windowWidth  += borderRect.right  - borderRect.left;
	windowHeight += borderRect.bottom - borderRect.top;

	HWND windowHandle = CreateWindowExA(
		windowExStyle,
		windowClassName,
		applicationName,
		windowStyle,
		windowX,
		windowY,
		windowWidth,
		windowHeight,
		0,
		0,
		instance,
		0);

	if (windowHandle == null) {
		log_error("window registration failed!");
		MessageBoxA(null, "window registration failed!", "error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	b32 shouldActivate = true;
	i32 showWindowCommandFlags = shouldActivate ? SW_SHOW : SW_SHOWNOACTIVATE;
	ShowWindow(windowHandle, showWindowCommandFlags);

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	clockFrequency = 1.0 / (f64)frequency.QuadPart;
	QueryPerformanceCounter(&startTime);

	state->internal = malloc(sizeof(internal_state));
	internal_state* internal = (internal_state*)state->internal;
	memset(internal, 0, sizeof(internal_state));
	internal->instanceHandle = instance;
	internal->windowHandle = windowHandle;

	return true;

}

void platform_destroy(platform_state* state) {

	internal_state* internal = (internal_state*)state->internal;

	if (internal->windowHandle) {
		DestroyWindow(internal->windowHandle);
		internal->windowHandle = null;
	}

	// TODO(zgayford): free platform_state

}

b8 platform_poll_events(platform_state* state) {
	
	MSG message;
	while (PeekMessageA(&message, null, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessageA(&message);
	}

	return true;

}

// TODO(zgayford): implement
void* platform_memory_allocate(u64 length, b8 aligned) {
	return malloc(length);
}

// TODO(zgayford): implement
void platform_memory_free(void* block, b8 aligned) {
	free(block);
}

// TODO(zgayford): implement
void platform_memory_zero(void* dst, u64 length) {
	memset(dst, 0, length);
}

// TODO(zgayford): implement
void platform_memory_copy(void* dst, const void* src, u64 length) {
	memcpy(dst, src, length);
}

// TODO(zgayford): implement
void platform_memory_set(void* dst, i32 value, u64 length) {
	memset(dst, value, length);
}

void platform_console_stdout(const char* message) {

	/*
	HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);
	static u8 levels[] = {64, 4, 6, 2, 1, 8};
	SetConsoleTextAttribute(stdout, levels[color]);

	// NOTE(zgayford): idk if i need this
	u64 length = strlen(message);
	LPDWORD bytesWritten = 0;
	OutputDebugStringA(message);
	WriteConsoleA(stdout, message, (DWORD)length, bytesWritten, 0);
	*/

	HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);
	u64 length = strlen(message);
	LPDWORD bytesWritten = 0;
	WriteConsoleA(stdout, message, (DWORD)length, bytesWritten, 0);

}

void platform_console_stderr(const char* message) {

	/*
	HANDLE stdout = GetStdHandle(STD_ERROR_HANDLE);
	static u8 levels[] = {64, 4, 6, 2, 1, 8};
	SetConsoleTextAttribute(stdout, levels[color]);

	// NOTE(zgayford): idk if i need this
	u64 length = strlen(message);
	LPDWORD bytesWritten = 0;
	OutputDebugStringA(message);
	WriteConsoleA(stdout, message, (DWORD)length, bytesWritten, 0);
	*/

	HANDLE stderr = GetStdHandle(STD_OUTPUT_HANDLE);
	u64 length = strlen(message);
	LPDWORD bytesWritten = 0;
	WriteConsoleA(stderr, message, (DWORD)length, bytesWritten, 0);

}

f64 platform_time_get() {
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return (f64)currentTime.QuadPart * clockFrequency;

}

void platform_time_sleep(u64 ms) {
	Sleep(ms);
}

//
// internal
//

LRESULT CALLBACK win32_process_message(HWND windowHandle, u32 message, WPARAM wparam, LPARAM lparam) {
	switch (message) {
		case WM_ERASEBKGND:
			// erasing the screen will be handled by the application (return 1)
			return 1;
		case WM_CLOSE:
			// TODO(zgayford): create event for application to quit
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_SIZE:
			// TODO(zgayford): create event for window resize
			/*
			RECT resize;
			GetClientRect(windowHandle, &resize);
			u32 width  = resize.right - resize.left;
			u32 height = resize.bottom - resize.top;
			*/
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			// TODO(zgayford): input processing
			// b8 down = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
			break;
		case WM_MOUSEMOVE:
			// TODO(zgayford): input processing
			/*
			i32 mouseX = GET_X_LPARAM(lparam);
			i32 mouseY = GET_Y_LPARAM(lparam);
			*/
			break;
		case WM_MOUSEWHEEL:
			// TODO(zgayford): input processing
			/*
			i32 wheelDelta = GET_WHEEL_DELTA_WPARAM(wparam);
			if (wheelDelta != 0) {
				wheelDelta = CLAMP(wheelDelta, -1, 1);
			}
			*/
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			// TODO(zgayford): input processing
			// b8 down = (message == WM_LBUTTONDOWN || message == WM_MBUTTONDOWN || message == WM_RBUTTONDOWN);
			break;
	}

	return DefWindowProcA(windowHandle, message, wparam, lparam);

}

#endif // PLATFORM_WINDOWS
