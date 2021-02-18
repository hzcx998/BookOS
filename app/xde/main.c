/**
 * xtk desktop environment
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <uview.h>
#include <assert.h>
#include <sys/ioctl.h>

#include <pthread.h>
#include <xtk.h>

void win_thread();

#define MOUSE_CURSOR_DIR            "/system/cursors"
#define BACKGROUND_IMAGE_NAME       "/system/background/picture.jpg"

void desktop_setup(xtk_spirit_t *spirit)
{
    xtk_spirit_show(spirit);
    // 加载鼠标光标
    xtk_window_load_mouse_cursors(XTK_WINDOW(spirit), MOUSE_CURSOR_DIR);
    
    // 加载壁纸
    xtk_image_t *img = xtk_image_load(BACKGROUND_IMAGE_NAME);
    if (!img)
        return;
    
    xtk_surface_t *surface = xtk_window_get_surface(XTK_WINDOW(spirit));
    assert(surface);
    xtk_surface_t img_surface;
    xtk_surface_init(&img_surface, img->w, img->h, (uint32_t *) img->buf);
    xtk_surface_blit_scaled(&img_surface, NULL, surface, NULL);
    // 刷新到窗口里面
    xtk_window_flip(XTK_WINDOW(spirit));
    xtk_image_destroy(img);
    // 创建子进程
    pid_t pid = fork();
    if (!pid) {
        char *_argv[3] = {
            "/app/lite",
            "/res/nes/mario.nes",
            NULL
        };
        execv(_argv[0], _argv);
        exit(-1);
    }
}

int open_desktop()
{
    if (xtk_init(NULL, NULL) < 0) {
        printf("xtk_init failed!\n");
        return -1;
    }
    xtk_spirit_t *screen_window = xtk_window_create(XTK_WINDOW_POPUP);
    if (!screen_window) {
        printf("xtk create desktop failed!\n");
        return -1;
    }

    // 设置为固定窗口，不能移动
    xtk_window_set_fixed(XTK_WINDOW(screen_window), true);
    // 显示到屏幕上后再调整大小
    assert(xtk_window_resize_to_screen(XTK_WINDOW(screen_window)) == 0);
    // 禁止窗口大小调整
    xtk_window_set_resizable(XTK_WINDOW(screen_window), false);
    desktop_setup(screen_window);

    xtk_main();
    return 0;
}


int main(int argc, char *argv[]) 
{
    if (open_desktop() < 0)
        return -1;
    return 0;
}