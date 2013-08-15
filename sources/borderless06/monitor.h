#ifndef MONITOR_H
#define MONITOR_H

#ifdef __cplusplus
   extern "C" {
#endif

typedef struct MONITOR
{
    int x, y, w, h;
} MONITOR;

int count_monitors();
MONITOR *get_monitors(int *monitor_count);

#ifdef __cplusplus
   }
#endif

#endif
