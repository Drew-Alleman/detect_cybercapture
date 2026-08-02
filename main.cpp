#include <windows.h>
#include <stdio.h>

BOOL found = FALSE;

BOOL CALLBACK EnumProc(HWND hwnd, LPARAM) {
    char cls[256] = {};
    GetClassNameA(hwnd, cls, sizeof(cls));

    if (strstr(cls, "border_mywnd_") &&
        (strncmp(cls, "snxh_", 5) == 0 || strncmp(cls, "snxk_", 5) == 0)) {
        found = TRUE;
        return FALSE;   // stop enumeration
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