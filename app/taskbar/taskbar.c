/**
 * 接收消息：创建，销毁，显示，隐藏，聚焦，丢焦。
 */
#include "taskbar.h"
#include "winctl.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/proc.h>
#include <sys/ipc.h>

taskbar_t taskbar;

static void taskbar_msg(xtk_spirit_t *spirit, uview_msg_t *msg);

int taskbar_init()
{
    list_init(&taskbar.winctl_list_head);
    taskbar.taskbar_color = TASKBAR_COLOR_DEFAULT;
    taskbar.winctl_back_color = WINCTL_BACK_COLOR;
    taskbar.winctl_active_color = WINCTL_ACTIVE_COLOR;
    taskbar.last_winctl = NULL;
    /* 创建消息队列，接收图标路径 */
    taskbar.icon_msgid = msgget(XTK_WINDOW_ICON_MSG_NAME, IPC_CREAT | IPC_EXCL);
    if (taskbar.icon_msgid < 0) {
        printf("create taskbar message failed!\n");
        return -1;
    }

    if (xtk_init(NULL, NULL) < 0) {
        printf("taskbar: xtk_init failed!\n");
        return -1;
    }
    xtk_spirit_t *spirit = xtk_window_create(XTK_WINDOW_POPUP);
    if (!spirit) {
        printf("taskbar: create window spirit failed!\n");
        return -1;
    }
    spirit->style.background_color = taskbar.taskbar_color;
    taskbar.spirit = spirit;
    xtk_window_get_screen(XTK_WINDOW(spirit), &taskbar.screen_width, &taskbar.screen_height);
    xtk_window_set_fixed(XTK_WINDOW(spirit), true);
    xtk_window_set_default_size(XTK_WINDOW(spirit), taskbar.screen_width, TASKBAR_HEIGHT_DEFAULT);
    xtk_window_set_resizable(XTK_WINDOW(spirit), false);
    xtk_window_set_position_absolute(XTK_WINDOW(spirit), 0, taskbar.screen_height - TASKBAR_HEIGHT_DEFAULT);
    xtk_window_set_routine(XTK_WINDOW(spirit), taskbar_msg);
    
    xtk_window_set_monitor(XTK_WINDOW(spirit), true);
    xtk_rect_t rect = {0, 0, (int) taskbar.screen_width, (int) (taskbar.screen_height - spirit->height)};
    xtk_window_set_maxim_rect(XTK_WINDOW(spirit), &rect);
    taskbar_draw_back();
    if (tasbar_clock_init(spirit) < 0)
        return -1;
    xtk_spirit_show_all(spirit);

    if (!fork())
        exit(execl("/app/sdl2", "/app/sdl2", NULL));

    return 0;
}

void taskbar_exit()
{
    msgput(taskbar.icon_msgid);
    winctl_destroy_all();
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
            xtk_spirit_show(taskbar.clock_label);
        }
        break;
    case UVIEW_MSG_CLOSE:
        if (type == UVIEW_TYPE_WINDOW) {
            winctl = winctl_find(target);
            if (winctl) {
                winctl_destroy(winctl);
                winctl_paint(NULL); /* paint all winctl */
                xtk_spirit_show(taskbar.clock_label);
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
    case UVIEW_MSG_SETICON:
        if (type == UVIEW_TYPE_WINDOW) {
            winctl = winctl_find(target);
            if (winctl && uview_msg_get_icontype(msg) == XTK_WINDOW_ICON_MIDDLE) {
                char icon_path[MAX_PATH] = {0};
                /* 如果加载失败就使用默认图标 */
                if (taskbar.icon_msgid >= 0) {
                    if (msgrecv(taskbar.icon_msgid, icon_path, MAX_PATH, 0) < 0)
                        strcpy(icon_path, WINCTL_ICON_PATH_DEFAULT);
                } else {
                    strcpy(icon_path, WINCTL_ICON_PATH_DEFAULT);
                }
                winctl_set_icon(winctl, icon_path);
            }
        }
        break;
    default:
        break;
    }
} 

