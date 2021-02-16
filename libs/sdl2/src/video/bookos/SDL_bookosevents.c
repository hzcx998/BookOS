/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2020 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "../../SDL_internal.h"

//#if SDL_VIDEO_DRIVER_BOOKOS
#if 1

#include "SDL_bookosevents.h"

#include "../../events/SDL_events_c.h"

void BOOKOS_WindowProc(xtk_spirit_t *spirit, uview_msg_t *msg)
{
    xtk_window_t *pwin = XTK_WINDOW(spirit);
    SDL_Window *sdl_win = (SDL_Window *)pwin->extension;

    switch (uview_msg_get_type(msg)) {
    case UVIEW_MSG_PAINT:
        if (pwin->paint_callback) {
            xtk_rect_t rect;
            if (!xtk_window_get_invalid(pwin, &rect)) {
                if (xtk_rect_valid(&rect)) {
                    SDL_SendWindowEvent(sdl_win, SDL_WINDOWEVENT_EXPOSED, 0, 0);
                }
            }
        }
        break;
    default:
        break;
    }
}

void BOOKOS_WindowPaint(xtk_spirit_t *spirit, xtk_rect_t *rect)
{
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    SDL_SendWindowEvent(window, SDL_WINDOWEVENT_EXPOSED, 0, 0);
}

bool BOOKOS_QuitEvent(xtk_spirit_t *spirit, void *arg)
{
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    SDL_SendWindowEvent(window, SDL_WINDOWEVENT_CLOSE, 0, 0);
    xtk_main_quit();
    return true;    // 返回TRUE，表示不会再执行后面的信号来销毁窗口操作
}

bool BOOKOS_MouseEnterEvent(xtk_spirit_t *spirit, void *arg)
{    
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        SDL_SetMouseFocus(window);
    }
    return true;
}

bool BOOKOS_MouseLeaveEvent(xtk_spirit_t *spirit, void *arg)
{
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        SDL_SetMouseFocus(NULL);
    }
    return true;
}

bool BOOKOS_MouseMotionEvent(xtk_spirit_t *spirit, xtk_event_t *event, void *arg)
{    
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        SDL_SendMouseMotion(window, 0, 0, event->motion.x, event->motion.y);
    }
    return true;
}

bool BOOKOS_ButtonPressEvent(xtk_spirit_t *spirit, xtk_event_t *event, void *arg)
{    
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        uint32_t sdl_button;
        switch (event->button.button) {
        case UVIEW_BTN_LEFT:
            sdl_button = SDL_BUTTON_LEFT;
            break;
        case UVIEW_BTN_MIDDLE:
            sdl_button = SDL_BUTTON_MIDDLE;
            break;
        case UVIEW_BTN_RIGHT:
            sdl_button = SDL_BUTTON_RIGHT;
            break;
        default:
            return false;
        }
        SDL_SendMouseButton(window, 0, SDL_PRESSED, sdl_button);
    }
    return true;
}

bool BOOKOS_ButtonReleaseEvent(xtk_spirit_t *spirit, xtk_event_t *event, void *arg)
{    
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        uint32_t sdl_button;
        switch (event->button.button) {
        case UVIEW_BTN_LEFT:
            sdl_button = SDL_BUTTON_LEFT;
            break;
        case UVIEW_BTN_MIDDLE:
            sdl_button = SDL_BUTTON_MIDDLE;
            break;
        case UVIEW_BTN_RIGHT:
            sdl_button = SDL_BUTTON_RIGHT;
            break;
        default:
            return false;
        }
        SDL_SendMouseButton(window, 0, SDL_RELEASED, sdl_button);
    }
    return true;
}

bool BOOKOS_ButtonScrollEvent(xtk_spirit_t *spirit, xtk_event_t *event, void *arg)
{    
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        float x = 0.0f, y = 0.0f;
        switch (event->wheel.wheel) {
        case XTK_WHEEL_UP:
            y = 0 - 1.0f;
            break;
        case XTK_WHEEL_DOWN:
            y = 0 + 1.0f;
            break;
        case XTK_WHEEL_LEFT:
            x = 0 - 1.0f;
            break;
        case XTK_WHEEL_RIGHT:
            x = 0 + 1.0f;
            break;
        default:
            break;
        }
        SDL_SendMouseWheel(window, 0, x, y, SDL_MOUSEWHEEL_NORMAL);
    }
    return true;
}

/**
 * Runs the main event loop.
 * @param   _THIS
 */
void BOOKOS_PumpEvents(_THIS)
{
    if (!xtk_poll()) {
        // 退出事件
        printf("SDL PumpEvents: xtk poll quit event!\n");
    }
}


#endif /* SDL_VIDEO_DRIVER_BOOKOS */

/* vi: set ts=4 sw=4 expandtab: */
