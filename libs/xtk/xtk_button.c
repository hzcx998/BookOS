#include "xtk.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static int xtk_button_create_signals(xtk_spirit_t *spirit)
{
    assert(!xtk_signal_create(spirit, "button_press"));
    assert(!xtk_signal_create(spirit, "button_release"));
    assert(!xtk_signal_create(spirit, "enter_notify"));
    assert(!xtk_signal_create(spirit, "leave_notify"));
    return 0;
}

xtk_spirit_t *xtk_button_create()
{
    xtk_button_t *button = malloc(sizeof(xtk_button_t));
    if (!button)
        return NULL;
    button->state = XTK_BUTTON_IDLE;
    button->color_idle = XTK_WHITE;
    button->color_touch = XTK_RGB_SUB(button->color_idle, 0x40, 0x40, 0x40);
    button->color_click = XTK_RGB_SUB(button->color_idle, 0x20, 0x20, 0x20);
    button->color_disabled = XTK_RGB(0x80, 0x80, 0x80);
    button->disabled = 0;
    button->flags = 0;

    xtk_spirit_t *spirit = &button->spirit;
    xtk_spirit_init(spirit, 0, 0, XTK_BUTTON_WIDTH_DEFAULT, XTK_BUTTON_HEIGHT_DEFAULT);
    xtk_spirit_set_type(spirit, XTK_SPIRIT_TYPE_BUTTON);
    
    spirit->style.border_color = XTK_GRAY;
    spirit->style.background_color = button->color_idle;
    spirit->style.color = XTK_BLACK;
    spirit->style.align = XTK_ALIGN_CENTER;
    
    if (xtk_button_create_signals(spirit) < 0) {
        xtk_spirit_cleanup(spirit);
        free(button);
        return NULL;
    }
    return spirit;
}

xtk_spirit_t *xtk_button_create_with_label(char *label)
{
    xtk_spirit_t *spirit = xtk_button_create();
    if (!spirit)
        return NULL;
    if (xtk_spirit_set_text(spirit, label) < 0) {
        xtk_spirit_destroy(spirit);
        return NULL;
    }
    return spirit;
}

int xtk_button_set_disable(xtk_button_t *button)
{
    if (!button)
        return -1;
    if (button->disabled)
        return -1;
    button->disabled = 1;
    button->state = XTK_BUTTON_IDLE;
    button->spirit.style.background_color = button->color_disabled;
    if (button->spirit.style.color)
        button->spirit.style.color = XTK_RGB_ADD(button->color_disabled, 0x40, 0x40, 0x40);
    xtk_spirit_show(&button->spirit);
    return 0;
}

int xtk_button_set_enable(xtk_button_t *button)
{
    if (!button)
        return -1;
    if (!button->disabled)
        return -1;
    button->disabled = 0;
    button->state = XTK_BUTTON_IDLE;
    button->spirit.style.background_color = button->color_idle;
    if (button->spirit.style.color)
        button->spirit.style.color = XTK_BLACK;
    xtk_spirit_show(&button->spirit);
    return 0;
}

void xtk_button_change_state(xtk_button_t *button, xtk_button_state_t state)
{
    if (button->state == state)
        return;
    xtk_spirit_t *spirit = &button->spirit;
    // printf("btn state %d to %d\n", button->state, state);
    button->state = state;

    switch (state) {
    case XTK_BUTTON_IDLE:    
        if (spirit->style.background_color != XTK_NONE_COLOR)
            spirit->style.background_color = button->color_idle;
        break;
    case XTK_BUTTON_TOUCH:    
        if (spirit->style.background_color != XTK_NONE_COLOR)
            spirit->style.background_color = button->color_touch;
        break;
    case XTK_BUTTON_CLICK:    
        if (spirit->style.background_color != XTK_NONE_COLOR)
            spirit->style.background_color = button->color_click;
        break;
    default:
        break;
    }
    xtk_spirit_show(spirit);    
}
