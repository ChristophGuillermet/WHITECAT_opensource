#include <allegro.h>
#include <winalleg.h>

#include "monitor.h"


int CALLBACK monitor_data_proc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    MONITOR **data = (MONITOR **)dwData;
    (*data)->x = lprcMonitor->left;
    (*data)->y = lprcMonitor->top;
    (*data)->w = lprcMonitor->right - lprcMonitor->left;
    (*data)->h = lprcMonitor->bottom - lprcMonitor->top;
    (*data)++;

    return 1;
}

int CALLBACK monitor_count_proc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    int *count = (int *)dwData;
    (*count)++;
    return 1;
}

int count_monitors()
{
    int count = 0;
    EnumDisplayMonitors(NULL, NULL, monitor_count_proc, (LPARAM)&count);
    return count;
}

MONITOR *get_monitors(int *monitor_count)
{
    struct MONITOR *monitors = NULL;
    int count = 0;
    MONITOR *data = NULL;


    count = count_monitors();
    if (monitor_count != NULL)
        *monitor_count = count;

    monitors = (struct MONITOR *)malloc(sizeof(MONITOR) * count);
    if (monitors == NULL)
    {
        return NULL;
    }

    data = monitors;
    EnumDisplayMonitors(NULL, NULL, monitor_data_proc, (LPARAM)&data);
    return monitors;
}
