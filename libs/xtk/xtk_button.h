#ifndef _LIB_XTK_BUTTON_H
#define _LIB_XTK_BUTTON_H

#include <stddef.h>
#include "xtk_spirit.h"

#define XTK_BUTTON_COLOR_IDLE_FIXED 0X01
#define XTK_BUTTON_COLOR_TOUCH_FIXED 0X02
#define XTK_BUTTON_COLOR_CLICK_FIXED 0X04

typedef enum {
    XTK_BUTTON_IDLE = 0,
    XTK_BUTTON_TOUCH,
    XTK_BUTTON_CLICK,
} xtk_button_state_t;

typedef struct {
    xtk_spirit_t spirit;
    xtk_button_state_t state;
    xtk_color_t color_idle;
    xtk_color_t color_touch;
    xtk_color_t color_click;
    xtk_color_t color_disabled;
    int disabled;
    int flags;
} xtk_button_t;

#define XTK_BUTTON(spirit)  ((xtk_button_t *)(spirit))

#define XTK_BUTTON_WIDTH_DEFAULT  48
#define XTK_BUTTON_HEIGHT_DEFAULT  24

xtk_spirit_t *xtk_button_create();
xtk_spirit_t *xtk_button_create_with_label(char *label);
void xtk_button_change_state(xtk_button_t *button, xtk_button_state_t state);
int xtk_button_set_disable(xtk_button_t *button);
int xtk_button_set_enable(xtk_button_t *button);

#endif /* _LIB_XTK_BUTTON_H */