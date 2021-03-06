/**
 * 接收消息：创建，销毁，显示，隐藏，聚焦，丢焦。
 */
#include "taskbar.h"
#include "winctl.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/proc.h>

taskbar_t taskbar;

static void taskbar_setup(xtk_spirit_t *spirit);

static void taskbar_msg(xtk_spirit_t *spirit, uview_msg_t *msg);

int taskbar_init()
{
    list_init(&taskbar.winctl_list_head);
    taskbar.taskbar_color = TASKBAR_COLOR_DEFAULT;
    taskbar.winctl_back_color = WINCTL_BACK_COLOR;
    taskbar.winctl_active_color = WINCTL_ACTIVE_COLOR;
    taskbar.last_winctl = NULL;
    if (xtk_init(NULL, NULL) < 0) {
        printf("taskbar: xtk_init failed!\n");
        return -1;
    }
    xtk_spirit_t *spirit = xtk_window_create(XTK_WINDOW_POPUP);
    if (!spirit) {
        printf("taskbar: create window spirit failed!\n");
        return -1;
    }
    taskbar.spirit = spirit;
    xtk_window_get_screen(XTK_WINDOW(spirit), &taskbar.screen_width, &taskbar.screen_height);
    xtk_window_set_fixed(XTK_WINDOW(spirit), true);
    xtk_window_set_default_size(XTK_WINDOW(spirit), taskbar.screen_width, TASKBAR_HEIGHT_DEFAULT);
    xtk_window_set_resizable(XTK_WINDOW(spirit), false);
    xtk_window_set_position_absolute(XTK_WINDOW(spirit), 0, taskbar.screen_height - TASKBAR_HEIGHT_DEFAULT);
    xtk_window_set_routine(XTK_WINDOW(spirit), taskbar_msg);
    xtk_window_set_monitor(XTK_WINDOW(spirit), true);

    taskbar_draw_back();
    taskbar_setup(spirit);
    xtk_spirit_show(spirit);
    return 0;
}

void taskbar_exit()
{
    /* TODO:destroy winctls */

    if (taskbar.spirit) {
        xtk_spirit_destroy(taskbar.spirit);
        taskbar.spirit = NULL;
    }
    taskbar.screen_width = 0;
    taskbar.screen_height = 0;
}

void taskbar_main()
{
    xtk_main();
}

static void taskbar_setup(xtk_spirit_t *spirit)
{
    #if 0
    char *argv[2] = {"/app/xtk", NULL};
    if (!fork()) {
        exit(execv(argv[0], argv));
    }
    #endif
    /*
    char *argv2[3] = {"/app/infones", "/res/nes/contra_u.nes", 0};
    if (!fork()) {
        exit(execv(argv2[0], argv2));
    }*/
}

void taskbar_draw_back()
{
    /* draw with color */
    xtk_surface_t *surface = xtk_window_get_surface(XTK_WINDOW(taskbar.spirit));
    xtk_surface_rectfill(surface, 0, 0, taskbar.spirit->width, taskbar.spirit->height, taskbar.taskbar_color);
    xtk_window_flip(XTK_WINDOW(taskbar.spirit));
}

/* 由于是监视器，因此这些消息都是来自其它视图的。 */
static void taskbar_msg(xtk_spirit_t *spirit, uview_msg_t *msg)
{
    int type = uview_msg_get_type(msg); /* view type */
    int target = uview_msg_get_target(msg); /* view target */
    int id = uview_msg_get_id(msg);
    winctl_t *winctl;
    switch (id) {
    case UVIEW_MSG_CREATE:  
        if (type == UVIEW_TYPE_WINDOW) {
            winctl_create(target);
            winctl_paint(NULL); /* paint all winctl */
        }
        break;
    case UVIEW_MSG_CLOSE:
        if (type == UVIEW_TYPE_WINDOW) {
            winctl = winctl_find(target);
            if (winctl) {
                winctl_destroy(winctl);
                winctl_paint(NULL); /* paint all winctl */
                taskbar.last_winctl = NULL; /* clear last winctl */
            }
        }
        break;
    case UVIEW_MSG_ACTIVATE:
        if (type == UVIEW_TYPE_WINDOW) {
            winctl = winctl_find(target);
            if (winctl) {
                winctl_get_focus(winctl);
                winctl_paint(winctl);
            }
        }
        break;
    case UVIEW_MSG_INACTIVATE:
        if (type == UVIEW_TYPE_WINDOW) {
            winctl = winctl_find(target);
            if (winctl) {
                winctl_lost_focus(winctl);
                winctl_paint(winctl);
            }
        }
        break;
    case UVIEW_MSG_HIDE:
        if (type == UVIEW_TYPE_WINDOW) {
            winctl = winctl_find(target);
            if (winctl) {
                winctl->ishidden = true;
                winctl_paint(winctl);
            }
        }
        break;
    case UVIEW_MSG_SHOW:
        if (type == UVIEW_TYPE_WINDOW) {
            winctl = winctl_find(target);
            if (winctl) {
                winctl->ishidden = false;
                winctl_paint(winctl);
            }
        }
        break;
    default:
        break;
    }
} 

