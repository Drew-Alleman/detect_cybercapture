#include <windows.h>
#include <stdio.h>

BOOL found = FALSE;

BOOL CALLBACK EnumProc(HWND hwnd, LPARAM) {
    char cls[256] = {};
    GetClassNameA(hwnd, cls, sizeof(cls));

    if (strstr(cls, "snxhk_border_mywnd_")) {
        found = TRUE;
        return FALSE;
    }
    return TRUE;
}

int main() {
    EnumWindows(EnumProc, 0);

    if (found) {
        MessageBoxA(NULL, "It's here", "Overlay Detected", MB_OK | MB_ICONWARNING);
    }
    else {
        MessageBoxA(NULL, "It's not here", "Overlay Not Found", MB_OK | MB_ICONINFORMATION);
    }

    return found ? 1 : 0;
}