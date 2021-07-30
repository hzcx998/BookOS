#include "taskbar.h"
#include <sys/walltime.h>
#include <stdio.h>

bool timer_func(xtk_spirit_t *window, uint32_t id, void *data)
{
    /* 读取 */
    walltime_t wtm;
    walltime(&wtm);
    char buf[32] = {0};
    sprintf(buf, "%d/%d/%d %d:%d", wtm.year, wtm.month, wtm.day, wtm.hour, wtm.minute);
    xtk_spirit_t *label = (xtk_spirit_t *) data;
    xtk_label_set_text(label, buf);
    return true;
}

int tasbar_clock_init(xtk_spirit_t *spirit)
{
    walltime_t wtm;
    walltime(&wtm);
    char buf[32] = {0};
    sprintf(buf, "%d/%d/%d %d:%d", wtm.year, wtm.month, wtm.day, wtm.hour, wtm.minute);
    xtk_spirit_t *clock_label = xtk_label_create(buf);
    xtk_container_add(XTK_CONTAINER(spirit), clock_label);
    xtk_spirit_set_pos(clock_label, taskbar.spirit->width - (strlen(buf) + 1) * 8,
        taskbar.spirit->height / 2 - 16 / 2);
    clock_label->style.color = XTK_WHITE;
    clock_label->style.background_color = taskbar.taskbar_color;
    taskbar.clock_label = clock_label;
    xtk_window_add_timer(XTK_WINDOW(spirit), 1000, timer_func, clock_label);
    return 0;
}