#include "winctl.h"
#include "taskbar.h"
#include <stdlib.h>

static bool winctl_left_btn_handler(xtk_spirit_t *spirit, void *arg)
{
    winctl_t *winctl = (winctl_t *) arg;
    if (!winctl) {
        return false;
    }
    uview_msg_t msg;
    if (winctl == taskbar.last_winctl) {
        /* 隐藏/显示 */
        if (winctl->ishidden) { /* 是一个隐藏的窗口，就显示 */
            uview_msg_header(&msg, UVIEW_MSG_SHOW, winctl->winid);
            uview_send_msg(taskbar.spirit->view, &msg);
        } else {    /* 已经是聚焦的窗口，那么再次点击就隐藏 */
            uview_msg_header(&msg, UVIEW_MSG_HIDE, winctl->winid);
            uview_send_msg(taskbar.spirit->view, &msg);
        }
    } else { /* 不是最新的窗口控制，就显示 */
        uview_msg_header(&msg, UVIEW_MSG_SHOW, winctl->winid);
        uview_send_msg(taskbar.spirit->view, &msg);
    }
    return true;
}

winctl_t *winctl_create(int winid)
{
    if (winid < 0)
        return NULL;

    winctl_t *winctl = malloc(sizeof(winctl_t));
    if (winctl == NULL)
        return NULL;

    winctl->button = xtk_button_create();
    if (!winctl->button) {
        free(winctl);
        return NULL;
    }
    
    winctl->winid = winid;
    winctl->ishidden = false;   /* 默认是显示 */
    winctl->isfocus = false; /* 默认不聚焦 */

    winctl->button->style.border_color = XTK_NONE_COLOR;
    winctl->button->style.background_color = taskbar.winctl_back_color;
    xtk_button_t *btn = XTK_BUTTON(winctl->button);
    btn->color_idle = taskbar.winctl_back_color;
    btn->color_touch = XTK_RGB_ADD(taskbar.winctl_back_color, 0x40, 0x40, 0x40);
    btn->color_click = XTK_RGB_ADD(taskbar.winctl_back_color, 0x20, 0x20, 0x20);
    xtk_spirit_set_size(winctl->button, WINCTL_SIZE_DEFAULT, WINCTL_SIZE_DEFAULT);
    // 点击事件
    xtk_signal_connect(winctl->button, "button_press", winctl_left_btn_handler, winctl);
    
    list_add_tail(&winctl->list, &taskbar.winctl_list_head);
    xtk_container_add(XTK_CONTAINER(taskbar.spirit), winctl->button);
    return winctl;
}

int winctl_destroy(winctl_t *winctl)
{
    if (!winctl)
        return -1;
    xtk_container_remove(XTK_CONTAINER(taskbar.spirit), winctl->button);
    list_del(&winctl->list);
    if (xtk_spirit_destroy(winctl->button) < 0)
        return -1;
    free(winctl);
    return 0;
}

int winctl_destroy_all()
{
    winctl_t *winctl, *winctl_next;
    list_for_each_owner_safe (winctl, winctl_next, &taskbar.winctl_list_head, list) {
        winctl_destroy(winctl);
    }
    return 0;
}

winctl_t *winctl_find(int winid)
{
    winctl_t *winctl;
    list_for_each_owner (winctl, &taskbar.winctl_list_head, list) {
        if (winctl->winid == winid)
            return winctl;
    }
    return NULL;
}

void winctl_get_focus(winctl_t *winctl)
{
    taskbar.last_winctl = winctl;
    winctl->isfocus = true;
    xtk_button_t *btn = XTK_BUTTON(winctl->button);
    btn->color_idle = taskbar.winctl_active_color;
    btn->spirit.style.background_color = btn->color_idle;
}

void winctl_lost_focus(winctl_t *winctl)
{
    winctl->isfocus = false;
    xtk_button_t *btn = XTK_BUTTON(winctl->button);
    btn->color_idle = taskbar.winctl_back_color;
    btn->spirit.style.background_color = btn->color_idle;
}

int winctl_set_icon(winctl_t *winctl, char *pathname)
{
    if (!winctl || !pathname)
        return -1;
    xtk_spirit_t *spirit = winctl->button;
    if (!spirit)
        return -1;
    /* 如果设置图标失败则尝试用默认图标 */
    if (xtk_spirit_set_image2(spirit, pathname, WINCTL_ICON_SIZE, WINCTL_ICON_SIZE) < 0)
        if (xtk_spirit_set_image2(spirit, WINCTL_ICON_PATH_DEFAULT, WINCTL_ICON_SIZE, WINCTL_ICON_SIZE) < 0)
            return -1;
    xtk_spirit_show(spirit);
    return 0;
}

/**
 * 当窗口控制为NULL时，则重绘整个窗口控制
 * 不然就绘制指定的窗口控制
 */
void winctl_paint(winctl_t *winctl)
{
    if (winctl) {
        xtk_spirit_show(winctl->button);
    } else {
        /* 先刷新背景，再显示 */
        taskbar_draw_back();

        int x = 4, y = 4;
        winctl_t *tmp;
        list_for_each_owner (tmp, &taskbar.winctl_list_head, list) {
            xtk_spirit_set_pos(tmp->button, x, y);  /* 调整位置 */
            xtk_spirit_show(tmp->button);
            x += WINCTL_ICON_SIZE + 4 + 2;
            /* 显示到最低端就不显示 */
            if (x >= taskbar.screen_width - WINCTL_ICON_SIZE - 4)
                break;
        }
    }
}
