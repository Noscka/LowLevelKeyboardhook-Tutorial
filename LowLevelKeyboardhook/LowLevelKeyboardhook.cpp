#include <iostream>
#include <Windows.h>

// Hook Variable 
HHOOK _hook;


// This struct contains the data received by the hook callback. As you see in the callback function
// it contains the thing you will need: vkCode = virtual key code.
KBDLLHOOKSTRUCT kbdStruct;

LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);

        BYTE KeyboardState[256] = {};

        // KeyBoardState BYTE array
        wchar_t UnicodeCharacter[3] = {};

        // Get keystate from Shift and ctrl when getting keyboardstate
        GetKeyState(VK_SHIFT);
        GetKeyState(VK_MENU);
        GetKeyboardState(KeyboardState);

        // Get the key hit while taking into account the modifiers (shift+5 -> %)
        ToUnicodeEx((UINT)kbdStruct.vkCode, (UINT)kbdStruct.scanCode, KeyboardState, UnicodeCharacter, sizeof(UnicodeCharacter) / sizeof(*UnicodeCharacter) - 1, (UINT)kbdStruct.flags, GetKeyboardLayout(0));

        switch (wParam)
        {
#if defined BLOCK
            // if keydown hook event (allows for holding and "spamming" key)
        case WM_KEYDOWN:
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


#else
            // if keydown hook event (allows for holding and "spamming" key)
        case WM_KEYDOWN:
            std::cout << (char)UnicodeCharacter[0] << " pressed Down" << std::endl;
            break;

        case WM_KEYUP:
            std::cout << (char)UnicodeCharacter[0] << " pressed up" << std::endl;
            break;
#endif
        
        }
    }

    return CallNextHookEx(_hook, nCode, wParam, lParam);
}


int main()
{
    SetConsoleTitleW(L"Key blocking in progress");

    // Set the hook and set it to use the callback function above
    // WH_KEYBOARD_LL means it will set a low level keyboard hook.
    // The last 2 parameters are NULL, 0 because the callback function is in the same thread and window as the
    // function that sets and releases the hook.
    _hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);

    // loop to keep the program open while still accepting input
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
    {
        PeekMessage(&msg, 0, 0, 0, 0x0001);
    }

    return 0;
}
