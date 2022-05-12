#include <iostream>
#include <Windows.h>

HHOOK _hook;

KBDLLHOOKSTRUCT kbdStruct;

LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

        BYTE KeyboardState[256] = {};

        wchar_t UnicodeCharacter[3] = {};

        GetKeyState(VK_SHIFT);
        GetKeyState(VK_MENU);
        GetKeyboardState(KeyboardState);

        ToUnicodeEx((UINT)kbdStruct.vkCode, (UINT)kbdStruct.scanCode, KeyboardState, UnicodeCharacter, sizeof(UnicodeCharacter) / sizeof(*UnicodeCharacter) - 1, (UINT)kbdStruct.flags, GetKeyboardLayout(0));

        switch (wParam)
        {
        case WM_KEYUP:
            if (UnicodeCharacter[0] == 'j')
            {
                printf("J was pressed and blocked \n");
                return -1;
            }
            else
            {
                std::cout << (char)UnicodeCharacter[0] << " pressed Down" << std::endl;
            }
            break;
        }
    }

    return CallNextHookEx(_hook, nCode, wParam, lParam);
}


int main()
{
    SetConsoleTitleW(L"Key blocking in progress");

    _hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
    {
        PeekMessage(&msg, 0, 0, 0, 0x0001);
    }

    return 0;
}
