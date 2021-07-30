#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/proc.h>
#include "desktop.h"
#include "icon.h"
#include "taskbar.h"

bde_desktop_t desktop;

#define CONFIG_PROC_EXEC

void desktop_launch(const char *pathname, char *arg)
{
    char *argv[3] = {(char *)pathname, arg, NULL};
#ifdef CONFIG_PROC_EXEC
    create_process(argv, environ, 0);
#else
    if (!fork()) {
        /* 如果是绝对路径，那么就 */
        char *p = (char *)pathname;
        if (*p == '/' && *(p + 1)) {
            char *p = strrchr(pathname, '/');
            if (p) {
                *p = '\0';
                chdir(pathname);
                *p = '/';
            }
        }
        exit(execv(pathname, argv));
    }
#endif
}

int desktop_load_background(const char *pathname)
{
    // 加载壁纸
    xtk_image_t *img = xtk_image_load(BACKGROUND_IMAGE_NAME);
    if (!img)
        return -1;
    
    xtk_surface_t *surface = xtk_window_get_surface(XTK_WINDOW(desktop.window));
    assert(surface);
    xtk_surface_t img_surface;
    xtk_surface_init(&img_surface, img->w, img->h, (uint32_t *) img->buf);
    xtk_surface_blit_scaled(&img_surface, NULL, surface, NULL);
    xtk_window_flip(XTK_WINDOW(desktop.window));

    xtk_image_destroy(img);
    return 0;
}

static void desktop_setup()
{
    // 加载鼠标光标
    xtk_window_load_mouse_cursors(XTK_WINDOW(desktop.window), MOUSE_CURSOR_DIR);
    // 加载背景图
    desktop_load_background(BACKGROUND_IMAGE_NAME);
    
    icon_init();

#if HAS_TASKBAR == 0
    // 启动任务栏
    desktop_launch(PRGRAMS_DIR "taskbar", NULL);
#else
    taskbar_init();
#endif
}

int desktop_init()
{
    if (xtk_init(NULL, NULL) < 0) {
        printf("desktop: xtk_init failed!\n");
        return -1;
    }
    xtk_spirit_t *spirit = xtk_window_create(XTK_WINDOW_POPUP);
    if (!spirit) {
        printf("desktop: xtk create desktop window failed!\n");
        return -1;
    }
    xtk_window_set_fixed(XTK_WINDOW(spirit), true);
    assert(xtk_window_resize_to_screen(XTK_WINDOW(spirit)) == 0);
    xtk_window_set_resizable(XTK_WINDOW(spirit), false);
    xtk_spirit_show(spirit);
    
    desktop.window = spirit;
    list_init(&desktop.icon_list);
    desktop.icon_start_x = 24;
    desktop.icon_start_y = 24;

    desktop_setup(spirit);
    return 0;
}

void desktop_main()
{
    xtk_main();
}

void desktop_exit()
{
#if HAS_TASKBAR == 1
    taskbar_exit();
#endif
    xtk_exit();
}
