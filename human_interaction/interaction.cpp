#include "../common.h"

HHOOK clickHook;
POINT coords;
POINT prev;
bool clickSuccess = false;
int sleepTime = 1000;
int dialogID;

void init_interaction() {
    
    GetCursorPos(&prev);

}

/// @brief Checks if a mouse is present using GetSystemMetrics
/// @return True if mouse is found, false if not
bool check_mouse_presence() {
    
    if (GetSystemMetrics(SM_MOUSEPRESENT) != 0) {

        return true;

    } else {

        return false;

    }
    
}

/// @brief Checks if the mouse moves
/// @return True if mouse moves, false if not
bool check_mouse_movement() {
    
    Sleep(sleepTime);
    GetCursorPos(&coords);

    if (coords.x == prev.x && coords.y == prev.y) {

        prev = coords;
        return false;

    } else {

        prev = coords;
        return true;

    }

}

/// @brief Checks mouse speed
/// @return True if mouse moves realistically, false if not
bool check_mouse_speed() {
    
    //Recommended speed acquired from pafish
    int maxSpeedX = GetSystemMetrics(SM_CXFULLSCREEN) / 5;
    int maxSpeedY = GetSystemMetrics(SM_CYFULLSCREEN) / 5;
    int timeSpent = 0;
    bool highSpeed;

    while(timeSpent < sleepTime) {

        Sleep(10);
        GetCursorPos(&coords);

        if (coords.x - prev.x < maxSpeedX) {

            if (coords.y - prev.y < maxSpeedY) {

                highSpeed = false;

            } else {

                highSpeed = true;
                break;

            }

        } else {

            highSpeed = true;
            break;

        }

        prev = coords;
        timeSpent = timeSpent + 10;

    }
    if (highSpeed == false) {

        return true;

    } else if (highSpeed == true) {

        return false;

    } else {

        std::cerr << "Could not properly read mouse speed!";
        std::exit(EXIT_FAILURE);

    }

}

VOID CALLBACK timer_call() {

    PostMessageA(NULL, (WM_USER + 1), 0, 0);

}

LRESULT CALLBACK click(int nCode, WPARAM wParam, LPARAM lp) {

    if (nCode >= 0) {

        if (wParam == WM_LBUTTONUP) {

            clickSuccess = true;

        }

    }
    return CallNextHookEx(clickHook, nCode, wParam, lp);
}

/// @brief Checks if the mouse clicks, uses mouse hooks
/// @return True if mouse clicks realistically, false if not
bool check_mouse_click() {

    SetTimer(NULL, 0, sleepTime, (TIMERPROC) &timer_call);
    clickHook = SetWindowsHookEx(WH_MOUSE_LL, click, NULL, 0);
    MSG msg;
    dialogID = MessageBox(NULL, "Please Confirm", "Confirm", MB_OK);

    while(GetMessage(&msg, NULL, 0, 0) > 0) {

        if(msg.message == WM_USER + 1) {

            break;

        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }

    KillTimer(NULL, 0);
    UnhookWindowsHookEx(clickHook);

    return clickSuccess;
}


/// @brief Checks if dialogue box is clicked
/// @return True if dialogue box is clicked, false if not
bool check_dialog_confirmation() {

    if (dialogID == IDOK) {

        return true;

    } else {

        return false;

    }

}
