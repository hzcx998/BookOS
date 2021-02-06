/**
 * xtk desktop environment
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/ioctl.h>

#include <pthread.h>
#include <xtk.h>

int click_count = 0;

static bool click_btn_event(xtk_spirit_t *spirit, void *label) 
{
    click_count++;
    char str[32] = {0};
    sprintf(str, "count: %d", click_count);
    xtk_label_set_text(label, str);
    return true;
}

static bool clear_btn_event(xtk_spirit_t *spirit, void *label) 
{
    click_count = 0;
    char str[32] = {0};
    sprintf(str, "count: %d", click_count);
    xtk_label_set_text(label, str);
    return true;
}

int main(int argc, char *argv[]) 
{
    xtk_init(&argc, &argv);
    //xtk_spirit_t *win = xtk_window_create_simple("xtk-test", 100, 100, 320, 240);
    xtk_spirit_t *win = xtk_window_create(XTK_WINDOW_TOPLEVEL);
    assert(win);
    xtk_window_set_default_size(XTK_WINDOW(win), 320, 240);
    xtk_spirit_t *l0 = xtk_label_create("count: 0");
    assert(l0);
    assert(!xtk_container_add(XTK_CONTAINER(win), l0));
    xtk_spirit_set_pos(l0, 160, 20);
    xtk_spirit_t *btn0 = xtk_button_create_with_label("click");
    assert(btn0);
    xtk_signal_connect(btn0, "button_press", click_btn_event, l0);
    assert(!xtk_container_add(XTK_CONTAINER(win), btn0));
    xtk_spirit_set_pos(btn0, 160 - 50, 100);
    xtk_spirit_t *btn1 = xtk_button_create_with_label("clear");
    assert(btn1);
    xtk_signal_connect(btn1, "button_press", clear_btn_event, l0);
    assert(!xtk_container_add(XTK_CONTAINER(win), btn1));
    xtk_spirit_set_pos(btn1, 160 + 50, 100);
       
    xtk_spirit_show_all(win);    
    xtk_main();
    return 0;
}
