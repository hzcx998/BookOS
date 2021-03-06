#ifndef _BDE_ICON_H
#define _BDE_ICON_H

#include <xtk.h>
#include <sys/list.h>
#include <stddef.h>

/*
图标描述文件内容：xxx.desktop
[Desktop Entry]               # 每个desktop文件都已这个标签开始，说明这是一个Desktop Entry 文件.
Version = 1.0                 # 标明Desktop Entry的版本(可选).
Name=Firefox                  # 程序名称(必须),这里是以创建一个eclipse的快捷方式为例.
GenericName = Web Browser     # 程序通用名称(可选).
Comment = Browse the Web      # 程序描述(可选).
Exec = firefox %u             # 程序的启动命令(必选),可以带参数运行
                              #+当下面的Type为Application，此项有效
Icon = firefox                # 设置快捷方式的图标(可选).
Terminal = false                    # 是否在终端中运行(可选),当Type为Application,此项有效.
Type = Application                  # desktop的类型(必选),常见值有“Application”和“Link”.
Categories = GNOME;Application;Network;   # 注明在菜单栏中显示的类别(可选)      
*/

#define ICON_SIZE_DEFAULT   32 
#define ICON_BUTTON_SIZE    64
#define ICON_DISTANCE       2

#define ICON_TYPE_UNKNOWN   0
#define ICON_TYPE_APP       1
#define ICON_TYPE_LINK      2

#define ICON_CLICK_DISTANCE_MS      500

#define DESKTOP_DIR            "/system/desktop"

typedef struct {
    list_t list;
    int x;
    int y;
    int click_ms;
    xtk_spirit_t *button;   // 图标的按钮
    xtk_spirit_t *label;    // 图标的显示文本
    xtk_spirit_t *icon;     // 图标的图标显示
    char exec[_MAX_PATH];
    bool terminal;              
    int type;                   // 图标类型
} icon_t;

icon_t *icon_create(char *name, char *exec, char *icon_file);
int icon_destroy(icon_t *icon);
void icon_paint(icon_t *icon);
void icon_paint_all();
int icon_init();

#endif /* _BDE_ICON_H */