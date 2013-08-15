#include "window.h"

#include <allegro.h>
#include <winalleg.h>

void position_window(int x, int y)
{
    HWND hwnd = win_get_window();
    if (hwnd != NULL)
        MoveWindow(hwnd, x, y, SCREEN_W, SCREEN_H, TRUE);
}
