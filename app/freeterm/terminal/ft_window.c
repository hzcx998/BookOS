#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

#include <ft_window.h>
#include <ft_console.h>
#include <ft_clipboard.h>
#include <ft_terminal.h>
#include <ft_cmd.h>
#include <ft_pty.h>

sh_window_t sh_window;

bool ft_button_press_event(xtk_spirit_t *spirit, xtk_event_t *event, void *data)
{
    if (event->button.button == UVIEW_BTN_LEFT)
        clipboard_start_select(event->button.x, event->button.y);
    else if (event->button.button == UVIEW_BTN_RIGHT)
        clipboard_copy_select();
    return true;
}

bool ft_button_release_event(xtk_spirit_t *spirit, xtk_event_t *event, void *data)
{
    if (event->button.button == UVIEW_BTN_LEFT)    
        clipboard_end_select(event->button.x, event->button.y);
    return true;
}

bool ft_motion_event(xtk_spirit_t *spirit, xtk_event_t *event, void *data)
{
    clipboard_move_select(event->motion.x, event->motion.y);
    return true;
}

bool ft_key_event(xtk_spirit_t *spirit, xtk_event_t *event, void *data)
{
    con_get_key(uview_keypad2ascii(event->key.keycode.code), event->key.keycode.modify);
    return true;
}

void ft_paint_callback(xtk_spirit_t *spirit, xtk_rect_t *rect)
{
    con_screen.flush();
}

int init_window()
{
    /* 初始化 */
    if (xtk_init(NULL, NULL) < 0) {
        printf("freeterm: init xtk failed!\n");
        return -1;
    }

    xtk_spirit_t *win = xtk_window_create_simple(APP_NAME, 300, 100, SH_WIN_WIDTH, SH_WIN_HEIGHT);
    if (!win) {
        printf("freeterm: create window failed!\n");
        return -1;
    }
    sh_window.win = win;
    sh_window.surf = xtk_window_get_surface(XTK_WINDOW(win));
    assert(sh_window.surf);

    xtk_signal_connect(win, "button_press", XTK_CALLBACK(ft_button_press_event), NULL);
    xtk_signal_connect(win, "button_release", XTK_CALLBACK(ft_button_release_event), NULL);
    xtk_signal_connect(win, "key_press", XTK_CALLBACK(ft_key_event), NULL);
    xtk_signal_connect(win, "motion_notify", XTK_CALLBACK(ft_motion_event), NULL);
    xtk_window_paint_callback(XTK_WINDOW(win), XTK_CALLBACK(ft_paint_callback));
    xtk_window_set_resizable(XTK_WINDOW(win), false);
    xtk_window_disable_maxim(XTK_WINDOW(win));
    
    xtk_spirit_show(win);
    return 0;
}

void window_loop()
{
    int len;
    char buf[FT_PTM_BUFLEN] = {0};
    while (1) {
        /* 消息轮询 */
        if (!xtk_poll())
            break;
        /* 查看子进程状态 */
        int st;
        int pid = waitpid(ft_pty.pid_slaver, &st, WNOHANG);
        if (pid == ft_pty.pid_slaver)
        {
            /* 读取剩余的再pty中的字符，避免子进程退出时，发出的数据依然存在，没有读取干净 */
            while (1) 
            {
                memset(buf, 0, FT_PTM_BUFLEN);
                if (read(ft_pty.fd_master, buf, FT_PTM_BUFLEN - 1) < 0)  /* 非阻塞读取 */
                    break;
            }
            break;
        }
        /* 从ptm读取数据 */
        if ((len = read(ft_pty.fd_master, buf, FT_PTM_BUFLEN - 1)) > 0) {
            #ifdef DEBUG_FT
            printf("freeterm: master read: %s\n", buf);
            #else
            shell_printf("%s", buf);
            #endif
            memset(buf, 0, FT_PTM_BUFLEN);
        }
        sched_yield();
    }
}

int exit_window()
{
    xtk_exit();
    return 0;
}

void sh_window_rect_fill(int x, int y, uint32_t width, uint32_t height, uint32_t color)
{
    xtk_surface_t *surf = xtk_surface_create(width, height);
    if (!surf)
        return;
    xtk_surface_rectfill(surf, 0, 0, width, height, color);
    xtk_rect_t rect;
    xtk_rect_init(&rect, x, y, surf->w, surf->h);
    xtk_surface_blit(surf, NULL, sh_window.surf, &rect);
    xtk_window_update(XTK_WINDOW(sh_window.win), x, y, surf->w, surf->h);
    xtk_surface_destroy(surf);
}

void sh_window_rect(int x, int y, uint32_t width, uint32_t height, uint32_t color)
{
    xtk_surface_t *surf = xtk_surface_create(width, height);
    if (!surf)
        return;
    xtk_surface_rect(surf, 0, 0, width, height, color);
    xtk_rect_t rect;
    xtk_rect_init(&rect, x, y, surf->w, surf->h);
    xtk_surface_blit(surf, NULL, sh_window.surf, &rect);
    xtk_window_update(XTK_WINDOW(sh_window.win), x, y, surf->w, surf->h);
    xtk_surface_destroy(surf);
}

void sh_window_char(int x, int y, char ch, uint32_t color)
{
    xtk_surface_t *surf = xtk_surface_create(CON_CHAR_WIDTH, CON_CHAR_HEIGHT);
    if (!surf)
        return;
    char s[2] = {0};
    s[0] = ch;
    xtk_text_to_surface(s, color, DOTF_STANDARD_NAME, surf, 0, 0);
    xtk_rect_t rect;
    xtk_rect_init(&rect, x, y, surf->w, surf->h);
    xtk_surface_blit(surf, NULL, sh_window.surf, &rect);
    /* 不需要刷新每个字符，会在绘制后统一刷新，提高效率 */
    // xtk_window_update(XTK_WINDOW(sh_window.win), x, y, surf->w, surf->h);
    xtk_surface_destroy(surf);
}

void sh_window_update(int left, int top, int right, int bottom)
{
    xtk_window_update(XTK_WINDOW(sh_window.win), left, top, right - left, bottom - top);
}

int sh_window_size(uint32_t *w, uint32_t *h)
{
    if (!sh_window.win)
        return -1;
    if (w)
        *w = sh_window.win->width;
    if (h)
        *h = sh_window.win->height;
    return 0;
}
