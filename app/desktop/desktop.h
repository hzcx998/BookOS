#ifndef _BDE_DESKTOP_H
#define _BDE_DESKTOP_H

#include <xtk.h>

#define MOUSE_CURSOR_DIR            "/system/cursors"
#define BACKGROUND_IMAGE_NAME       "/system/background/picture.jpg"
#define PRGRAMS_DIR                 "/system/programs/"

#define HAS_TASKBAR 1

typedef struct {
    xtk_spirit_t *window;
    list_t icon_list;
    int icon_start_x;
    int icon_start_y;
} bde_desktop_t;

extern bde_desktop_t desktop;

int desktop_init();
void desktop_main();
void desktop_exit();
void desktop_launch(const char *pathname, char *arg);
int desktop_load_background(const char *pathname);

#endif /* _BDE_DESKTOP_H */