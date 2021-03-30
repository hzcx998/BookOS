#include "xtk_keyboard.h"
#include "xtk_entry.h"

#include <stdio.h>
#include <string.h>

int xtk_keyboard_filter(int keycode, int modify)
{
    keycode = uview_keypad2ascii(keycode);
    switch (keycode) {
    case UVIEW_KEY_NUMLOCK:
    case UVIEW_KEY_CAPSLOCK:
    case UVIEW_KEY_SCROLLOCK:
    case UVIEW_KEY_RSHIFT:
    case UVIEW_KEY_LSHIFT:
    case UVIEW_KEY_RCTRL:
    case UVIEW_KEY_LCTRL:
    case UVIEW_KEY_RALT:
    case UVIEW_KEY_LALT:
        return 0;
    default:
        break;
    }
    return keycode;
}

int xtk_keyboard_key_down(xtk_spirit_t *spirit, int keycode, int modify)
{
    xtk_container_t *container = spirit->container;
    if (!container)
        return -1;
    xtk_spirit_t *tmp;
    list_for_each_owner (tmp, &container->children_list, list) {
        if (!tmp->visible)
            continue;
        switch (tmp->type) {
        case XTK_SPIRIT_TYPE_LABEL:
            {

            }
            break;
        case XTK_SPIRIT_TYPE_BUTTON:
            {
                
            }
            break;
        case XTK_SPIRIT_TYPE_ENTRY:
            {
                xtk_entry_t *entry = XTK_ENTRY(tmp);
                if (entry->focus) {
                    keycode = xtk_keyboard_filter(keycode, modify);
                    if (!keycode)
                        break;
                    xtk_entry_process_key(entry, keycode, modify);
                }
            }
            break;
        default:
            break;
        }
    }
    return -1;
}

int xtk_keyboard_key_up(xtk_spirit_t *spirit, int keycode, int modify)
{
    xtk_container_t *container = spirit->container;
    if (!container)
        return -1;
    xtk_spirit_t *tmp;
    list_for_each_owner (tmp, &container->children_list, list) {
        if (!tmp->visible)
            continue;
        switch (tmp->type) {
        case XTK_SPIRIT_TYPE_LABEL:
            {

            }
            break;
        case XTK_SPIRIT_TYPE_BUTTON:
            {
                
            }
            break;
        default:
            break;
        }
    }
    return -1;
}
