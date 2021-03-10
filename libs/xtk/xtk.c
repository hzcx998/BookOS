#include "xtk.h"
#include <uview.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <dotfont.h>

static int __xtk_init_done = 0;
static int __xtk_main_loop = 0;

int __xtk_has_window_close = 0;

int xtk_check_main_loop()
{
    return __xtk_main_loop;
}

int xtk_init(int *argc, char **argv[])
{
    // TODO: init everything...    
    xtk_text_init();
    xtk_view_init();
    // atexit(xtk_exit);
    __xtk_main_loop = 1;
    __xtk_has_window_close = 0;
    __xtk_init_done = 1;
    return 0;
}

void xtk_exit(void)
{
    if (__xtk_init_done) {
        xtk_view_t *pview, *pnext;
        xtk_view_for_each_safe (pview, pnext) {
            assert(pview->spirit);
            xtk_window_quit(pview->spirit);
        }
        
        // TODO: do exit everything
        xtk_view_exit();
        //printf("xtk: exit now.\n");
        __xtk_init_done = 0;
    }
}

int xtk_main()
{
    if (!__xtk_init_done) {
        perror("xtk_init not called before xtk_main!");
        abort();
    }
    xtk_spirit_t *spirit;
    xtk_window_t *window;
    uview_msg_t msg;
    xtk_view_t *pview, *vnext;
    int filter_val; 
    int target;
    int view_nr = xtk_view_length();
    while (__xtk_main_loop && view_nr > 0) { 
        view_nr = xtk_view_length();   
        xtk_view_for_each_safe (pview, vnext) {
            if (view_nr >= 2) { // 2个及其以上的视图时设置为nowait
                if (!pview->nowait) { // 视图设置为nowait
                    pview->nowait = 1;
                    uview_set_nowait(pview->view, pview->nowait);
                }
            } else {    // 只有一个的时候就要阻塞
                if (pview->nowait) { // 视图设置为nowait
                    pview->nowait = 0;
                    uview_set_nowait(pview->view, pview->nowait);
                }
            }
            if (uview_get_msg(pview->view, &msg) < 0) {
                continue;
            }
            __xtk_has_window_close = 0; /* 没有窗口关闭 */
            filter_val = 1; /* 消息没有过滤掉 */
            target = uview_msg_get_target(&msg);
            window = XTK_WINDOW(pview->spirit);
            if (pview->real_view == target) {    /* 目标是当前视图才能进行遍历 */ 
                // 遍历每一个视图来获取上面的精灵
                list_for_each_owner (spirit, &pview->spirit_list_head, list) {                
                    if (!(filter_val = xtk_window_main(spirit, &msg)))
                        break;
                    if (!__xtk_main_loop) {
                        return 0;
                    }
                    if (__xtk_has_window_close)
                        break;
                }
                // 需要检测一下是否有窗口已经关闭掉了，如果是那么就不再往后面执行
                if (!__xtk_main_loop) {
                    return 0;
                }
                if (__xtk_has_window_close) {
                    continue;
                }
                // 没有过滤掉才处理用户消息
                if (filter_val)
                    filter_val = xtk_window_filter_msg(window, &msg);
            }
            // 调用用户消息过滤函数
            if (filter_val) {
                // 调用用户处理函数
                if (window->routine)
                    window->routine(&window->spirit, &msg);
            }

        }
    }
    return 0;
}


/**
 * 没有退出返回1
 * 退出返回0
 */
int xtk_poll()
{
    if (!__xtk_init_done) {
        perror("xtk_init not called before xtk_main!");
        abort();
    }
    xtk_spirit_t *spirit;
    xtk_window_t *window;
    uview_msg_t msg;
    xtk_view_t *pview, *vnext;
    int filter_val; 
    int target;
    int view_nr = xtk_view_length();
    if (__xtk_main_loop && view_nr > 0) {    
        xtk_view_for_each_safe (pview, vnext) {
            if (!pview->nowait) { // 视图设置为nowait
                pview->nowait = 1;
                uview_set_nowait(pview->view, pview->nowait);
            }
            while (!uview_get_msg(pview->view, &msg)) {
                __xtk_has_window_close = 0; /* 没有窗口关闭 */
                filter_val = 1; /* 消息没有过滤掉 */
                target = uview_msg_get_target(&msg);
                window = XTK_WINDOW(pview->spirit);
                if (pview->real_view == target) {    /* 目标是当前视图才能进行遍历 */ 
                    // 遍历每一个视图来获取上面的精灵
                    list_for_each_owner (spirit, &pview->spirit_list_head, list) {                
                        if (!(filter_val = xtk_window_main(spirit, &msg)))
                            break;
                        if (!__xtk_main_loop) {
                            return 0;
                        }
                        if (__xtk_has_window_close)
                            break;
                    }

                    // 需要检测一下是否有窗口已经关闭掉了，如果是那么就不再往后面执行
                    if (!__xtk_main_loop) {
                        return 0;
                    }
                    if (__xtk_has_window_close) {
                        continue;
                    }
                    // 没有过滤掉才处理用户消息
                    if (filter_val)
                        xtk_window_filter_msg(window, &msg);
                }
                // 调用用户消息过滤函数
                if (filter_val) {
                    // 调用用户处理函数
                    if (window->routine)
                        window->routine(&window->spirit, &msg);
                }
            }
        }
    } else {
        return 0;  // xtk已经退出执行
    }
    return 1;
}

int xtk_main_quit()
{
    __xtk_main_loop = 0;
    return 0;
}
