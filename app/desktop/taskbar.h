/**
 * bookos desktop environment
 * -taskbar
 */

#ifndef _BDE_TASKBAR_H   /* bookos desktop environment (BDE) */
#define _BDE_TASKBAR_H

#include <xtk.h>
#include <sys/list.h>

#include "winctl.h"

#define TASKBAR_HEIGHT_DEFAULT  48
#define TASKBAR_COLOR_DEFAULT   XTK_BLACK

typedef struct {
    xtk_spirit_t *spirit;
    xtk_color_t taskbar_color;
    xtk_color_t winctl_back_color;         /* 窗口控制背景颜色 */
    xtk_color_t winctl_active_color;       /* 窗口控制激活后的颜色 */
    int screen_width;
    int screen_height;
    list_t winctl_list_head;    /* 窗口控制的链表头 */
    winctl_t *last_winctl;      /* 最近访问过的窗口控制 */
    int icon_msgid;              /* 图标消息队列 */
    xtk_spirit_t *clock_label;
} taskbar_t;

extern taskbar_t taskbar;

int taskbar_init();
void taskbar_exit();
// void taskbar_main();
void taskbar_draw_back();

int tasbar_clock_init(xtk_spirit_t *spirit);

#endif  /* _BDE_TASKBAR_H */