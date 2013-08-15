#include <OpenLayer.hpp>
#include <winalleg.h>


#include "monitor.h"
#include "window.h"


using namespace ol;


int main()
{
    int DESKTOP_W = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int DESKTOP_H = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    if (!Setup::SetupProgram())
    {
        allegro_message("Unable to Setup OpenLayer");
        return 1;
    }
    Settings::SetWindowBorder(false);

    int monitor_count;
    MONITOR *monitors = get_monitors(&monitor_count);

    if (!Setup::SetupScreen(DESKTOP_W, DESKTOP_H, false))
    {
        allegro_message("Unable to Setup Graphics Mode");
        return 1;
    }

    position_window(0, 0);

    while(!key[KEY_ESC])
    {
        rest(20);
        Canvas::Fill( Rgba::BLACK );
        for (int c = 0; c < monitor_count; c++)
        {
            Rgba color = Rgba::BLACK;
            switch (c)
            {
              case 0:
                color = Rgba::WHITE;
                break;
              case 1:
                color = Rgba::RED;
                break;
              case 2:
                color = Rgba::YELLOW;
                break;
              case 3:
                color = Rgba::GREEN;
                break;
            }
            Rect( monitors[c].x, monitors[c].y, monitors[c].w, monitors[c].h ).Draw( color );
        }
        Canvas::Refresh();
    }

    free(monitors);
    return 0;
}
END_OF_MAIN()
