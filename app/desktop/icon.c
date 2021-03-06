#include "icon.h"
#include "desktop.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <dirent.h>
#include <sys/types.h>

static void icon_launch(icon_t *icon)
{
    char *name, *p, *arg = NULL;
    name = icon->exec;
    p = name;
    /* replace ' '(space) to '\0' */ 
    while (*p) {
        if (*p == ' ') {
            *p = '\0';
            if (*(p + 1)) {
                arg = p + 1;
            }
        }
        p++;
    }
    desktop_launch(name, arg);
}

static bool icon_button_press_event(xtk_spirit_t *button, void *arg)
{
    icon_t *icon = (icon_t *) arg;
    struct timeval tv;
    if (icon->click_ms) { // 第二次点击
        gettimeofday(&tv, NULL);
        int tm = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        if ((tm - icon->click_ms) < ICON_CLICK_DISTANCE_MS) { // 双击间隔
            printf("# luanch %s\n", icon->exec);
            icon_launch(icon);
        }
        icon->click_ms = 0;
    } else {
        // 记录第一次点击时间
        gettimeofday(&tv, NULL);
        icon->click_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }
    return true;
}

static int icon_create_spirit(icon_t *icon, char *name, char *icon_file)
{
    icon->button = xtk_button_create();
    if (!icon->button) {
        return -1;    
    }
    xtk_spirit_set_size(icon->button, ICON_BUTTON_SIZE, ICON_BUTTON_SIZE);
    icon->button->style.border_color = icon->button->style.background_color;
    icon->button->style.background_color = XTK_NONE_COLOR;
    xtk_signal_connect(icon->button, "button_press", icon_button_press_event, icon);

    icon->label = xtk_label_create(name);
    if (!icon->label) {
        xtk_spirit_destroy(icon->button);
        return -1;    
    }
    icon->label->style.background_color = XTK_NONE_COLOR;

    icon->icon = xtk_spirit_create(0, 0, ICON_SIZE_DEFAULT, ICON_SIZE_DEFAULT);
    if (!icon->icon) {
        xtk_spirit_destroy(icon->label);
        xtk_spirit_destroy(icon->button);
        return -1;    
    }
    if (xtk_spirit_set_background_image(icon->icon, icon_file) < 0)
        printf("icon: set image failed!\n");

    return 0;
}

icon_t *icon_create(char *name, char *exec, char *icon_file)
{
    icon_t *icon = malloc(sizeof(icon_t));
    if (!icon)
        return NULL;
    if (icon_create_spirit(icon, name, icon_file) < 0) {
        free(icon);
        return NULL;    
    }
    memset(icon->exec, 0, MAX_PATH);
    strcpy(icon->exec, exec);
    icon->terminal = false;
    icon->type = ICON_TYPE_APP;
    icon->click_ms = 0;
    list_add(&icon->list, &desktop.icon_list);
    return icon;
}

int icon_destroy(icon_t *icon)
{
    list_del_init(&icon->list);
    xtk_spirit_destroy(icon->button);
    xtk_spirit_destroy(icon->icon);
    xtk_spirit_destroy(icon->label);
    free(icon);
    return 0;
}

void icon_add(icon_t *icon, xtk_spirit_t *window)
{
    xtk_container_add(XTK_CONTAINER(window), icon->button);
    xtk_container_add(XTK_CONTAINER(window), icon->label);
    xtk_container_add(XTK_CONTAINER(window), icon->icon);
}

void icon_remove(icon_t *icon, xtk_spirit_t *window)
{
    xtk_container_remove(XTK_CONTAINER(window), icon->button);
    xtk_container_remove(XTK_CONTAINER(window), icon->label);
    xtk_container_remove(XTK_CONTAINER(window), icon->icon);
}

/**
 * 显示图标，把图标绘制到桌面上
 */
int icon_locate(icon_t *icon, int x, int y)
{
    if (!icon)
        return -1;
    icon->x = x;
    icon->y = y;
    int width = icon->button->width;
    xtk_spirit_set_pos(icon->button, icon->x, icon->y);
    xtk_spirit_set_pos(icon->icon,
        icon->x + (width / 2) - icon->icon->width / 2,
        icon->y + 8);
    xtk_spirit_set_pos(icon->label,
        icon->x + (width / 2) - xtk_label_width(icon->label) / 2 - 2,
        icon->icon->y + icon->icon->height + 2);
    return 0;
}

/**
 * 设置图标位置，根据系统自动设置
 */
int icon_locate_all()
{
    icon_t *icon;
    int x, y;
    x = desktop.icon_start_x;
    y = desktop.icon_start_y;
    
    list_for_each_owner (icon, &desktop.icon_list, list) {
        icon_locate(icon, x, y);
        y += ICON_BUTTON_SIZE + ICON_DISTANCE;
        if (y >= desktop.window->height - (ICON_BUTTON_SIZE + ICON_DISTANCE)) {
            x += ICON_BUTTON_SIZE + ICON_DISTANCE;
            y = desktop.icon_start_y;
        }
    }
    return 0;
}


void icon_paint(icon_t *icon)
{
    xtk_spirit_show(icon->button);
    xtk_spirit_show(icon->icon);
    xtk_spirit_show(icon->label);
}

void icon_paint_all()
{
    icon_t *icon;
    list_for_each_owner (icon, &desktop.icon_list, list) {
        icon_paint(icon);
    }
}

int icon_read_desktop_file(char *pathname)
{
    printf("icon file: %s\n", pathname);
    return 0;
}

/**
 * 从桌面目录扫描图标描述文件，生成图标
 */
void icon_scan()
{
    DIR *dir = opendir(DESKTOP_DIR);
    if (!dir) {
        printf("desktp: open dir %s failed!\n", DESKTOP_DIR);
        return;
    }
    
    while (1) {
        struct dirent *de = readdir(dir);
        if (!de)
            break;
        // 获取文件名，并打开之
        if (!(de->d_attr & DE_DIR)) {
            printf("dir: %s, file: %s\n", DESKTOP_DIR, de->d_name);
            char name[_MAX_PATH] = {0};
            strcpy(name, DESKTOP_DIR);
            strcat(name, "/");
            strcat(name, de->d_name);
            icon_read_desktop_file(name);
        }
    }
    closedir(dir);
}

int icon_init()
{
    icon_t *icon = icon_create("infones", 
        "/app/infones /res/nes/mario.nes",
        "/usr/local/infones/infones.png");

    if (icon)
        icon_add(icon, desktop.window);
    icon = icon_create("invader", 
        "/app/invader",
        "/usr/local/infones/infones.png");
    if (icon)
        icon_add(icon, desktop.window);
    icon = icon_create("lite", 
        "/app/lite", 
        "/usr/local/lite/lite.png");
    if (icon)
        icon_add(icon, desktop.window);
    icon_locate_all();
    icon_paint_all();

    icon_scan();

    return 0;
}
