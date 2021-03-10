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
    if (window) {
        SDL_SendWindowEvent(window, SDL_WINDOWEVENT_EXPOSED, 0, 0);
    }
}

bool BOOKOS_QuitEvent(xtk_spirit_t *spirit, void *arg)
{
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        SDL_SendWindowEvent(window, SDL_WINDOWEVENT_CLOSE, 0, 0);
        xtk_main_quit();
        return true;    // 返回TRUE，表示不会再执行后面的信号来销毁窗口操作
    }
    return false;
}

bool BOOKOS_WindowActiveEvent(xtk_spirit_t *spirit, void *arg)
{
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        if (SDL_GetKeyboardFocus() != window) {
            SDL_SetKeyboardFocus(window);
        }
    }
    return true;    
}

bool BOOKOS_WindowInactiveEvent(xtk_spirit_t *spirit, void *arg)
{
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        if (SDL_GetKeyboardFocus() == window) {
            SDL_SetKeyboardFocus(NULL);
        }
    }
    return true;    
}

bool BOOKOS_WindowMoveEvent(xtk_spirit_t *spirit, xtk_event_t *event, void *arg)
{    
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        SDL_SendWindowEvent(window, SDL_WINDOWEVENT_MOVED, event->winmove.x, event->winmove.y);
    }
    return true;
}

bool BOOKOS_WindowMinimEvent(xtk_spirit_t *spirit, void *arg)
{
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        SDL_SendWindowEvent(window, SDL_WINDOWEVENT_MINIMIZED, 0, 0);
    }
    return true;    
}

bool BOOKOS_WindowMaximEvent(xtk_spirit_t *spirit, void *arg)
{
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        SDL_SendWindowEvent(window, SDL_WINDOWEVENT_MAXIMIZED, 0, 0);
    }
    return true;    
}

bool BOOKOS_WindowRestoreEvent(xtk_spirit_t *spirit, void *arg)
{
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        SDL_SendWindowEvent(window, SDL_WINDOWEVENT_RESTORED, 0, 0);
    }
    return true;
}

bool BOOKOS_WindowResizeEvent(xtk_spirit_t *spirit, xtk_event_t *event, void *arg)
{    
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    if (window) {
        SDL_SendWindowEvent(window, SDL_WINDOWEVENT_RESIZED, event->winresize.w, event->winresize.h);
    }
    return true;
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

uint32_t xtk_code_to_sdl_code(uint32_t code)
{
    switch (code) {
    case UVIEW_KEY_A:
    case UVIEW_KEY_a: return SDL_SCANCODE_A;
    case UVIEW_KEY_B:
    case UVIEW_KEY_b: return SDL_SCANCODE_B;
    case UVIEW_KEY_C:
    case UVIEW_KEY_c: return SDL_SCANCODE_C;
    case UVIEW_KEY_D:
    case UVIEW_KEY_d: return SDL_SCANCODE_D;
    case UVIEW_KEY_E:
    case UVIEW_KEY_e: return SDL_SCANCODE_E;
    case UVIEW_KEY_F:
    case UVIEW_KEY_f: return SDL_SCANCODE_F;
    case UVIEW_KEY_G:
    case UVIEW_KEY_g: return SDL_SCANCODE_G;
    case UVIEW_KEY_H:
    case UVIEW_KEY_h: return SDL_SCANCODE_H;
    case UVIEW_KEY_I:
    case UVIEW_KEY_i: return SDL_SCANCODE_I;
    case UVIEW_KEY_J:
    case UVIEW_KEY_j: return SDL_SCANCODE_J;
    case UVIEW_KEY_K:
    case UVIEW_KEY_k: return SDL_SCANCODE_K;
    case UVIEW_KEY_L:
    case UVIEW_KEY_l: return SDL_SCANCODE_L;
    case UVIEW_KEY_M:
    case UVIEW_KEY_m: return SDL_SCANCODE_M;
    case UVIEW_KEY_N:
    case UVIEW_KEY_n: return SDL_SCANCODE_N;
    case UVIEW_KEY_O:
    case UVIEW_KEY_o: return SDL_SCANCODE_O;
    case UVIEW_KEY_P:
    case UVIEW_KEY_p: return SDL_SCANCODE_P;
    case UVIEW_KEY_Q:
    case UVIEW_KEY_q: return SDL_SCANCODE_Q;
    case UVIEW_KEY_R:
    case UVIEW_KEY_r: return SDL_SCANCODE_R;
    case UVIEW_KEY_S:
    case UVIEW_KEY_s: return SDL_SCANCODE_S;
    case UVIEW_KEY_T:
    case UVIEW_KEY_t: return SDL_SCANCODE_T;
    case UVIEW_KEY_U:
    case UVIEW_KEY_u: return SDL_SCANCODE_U;
    case UVIEW_KEY_V:
    case UVIEW_KEY_v: return SDL_SCANCODE_V;
    case UVIEW_KEY_W:
    case UVIEW_KEY_w: return SDL_SCANCODE_W;
    case UVIEW_KEY_X:
    case UVIEW_KEY_x: return SDL_SCANCODE_X;
    case UVIEW_KEY_Y:
    case UVIEW_KEY_y: return SDL_SCANCODE_Y;
    case UVIEW_KEY_Z:
    case UVIEW_KEY_z: return SDL_SCANCODE_Z;
    case UVIEW_KEY_EXCLAIM: /* ! */
    case UVIEW_KEY_1: return SDL_SCANCODE_1;
    case UVIEW_KEY_AT: /* @ */
    case UVIEW_KEY_2: return SDL_SCANCODE_2;
    case UVIEW_KEY_HASH: /* # */
    case UVIEW_KEY_3: return SDL_SCANCODE_3;
    case UVIEW_KEY_DOLLAR:  /* $ */
    case UVIEW_KEY_4: return SDL_SCANCODE_4;
    case UVIEW_KEY_PERSENT: /* % */
    case UVIEW_KEY_5: return SDL_SCANCODE_5;
    case UVIEW_KEY_CARET:   /* ^ */
    case UVIEW_KEY_6: return SDL_SCANCODE_6;
    case UVIEW_KEY_AMPERSAND:   /* & */
    case UVIEW_KEY_7: return SDL_SCANCODE_7;
    case UVIEW_KEY_ASTERISK:    /* * */
    case UVIEW_KEY_8: return SDL_SCANCODE_8;
    case UVIEW_KEY_LEFTPAREN: /* ( */
    case UVIEW_KEY_9: return SDL_SCANCODE_9;
    case UVIEW_KEY_RIGHTPAREN:  /* ) */
    case UVIEW_KEY_0: return SDL_SCANCODE_0;
    case UVIEW_KEY_ENTER: return SDL_SCANCODE_RETURN;
    case UVIEW_KEY_ESCAPE: return SDL_SCANCODE_ESCAPE;
    case UVIEW_KEY_BACKSPACE: return SDL_SCANCODE_BACKSPACE;
    case UVIEW_KEY_TAB: return SDL_SCANCODE_TAB;
    case UVIEW_KEY_SPACE: return SDL_SCANCODE_SPACE;

    case UVIEW_KEY_UNDERSCRE:  /* _ */
    case UVIEW_KEY_MINUS: return SDL_SCANCODE_MINUS;
    case UVIEW_KEY_PLUS:    /* + */
    case UVIEW_KEY_EQUALS: return SDL_SCANCODE_EQUALS;
    case UVIEW_KEY_LEFTSQUAREBRACKET:
    case UVIEW_KEY_LEFTBRACKET: return SDL_SCANCODE_LEFTBRACKET;
    case UVIEW_KEY_RIGHTSQUAREBRACKET:
    case UVIEW_KEY_RIGHTBRACKET: return SDL_SCANCODE_RIGHTBRACKET;
    case UVIEW_KEY_VERTICAL:
    case UVIEW_KEY_BACKSLASH: return SDL_SCANCODE_BACKSLASH;
    /* SDL_SCANCODE_NONUSHASH */
    case UVIEW_KEY_COLON:   /* : */
    case UVIEW_KEY_SEMICOLON: return SDL_SCANCODE_SEMICOLON;
    case UVIEW_KEY_QUOTEDBL:    /* ' */
    case UVIEW_KEY_QUOTE: return SDL_SCANCODE_APOSTROPHE;
    case UVIEW_KEY_BACKQUOTE:   /* ` */
    case UVIEW_KEY_TILDE: return SDL_SCANCODE_GRAVE;
    case UVIEW_KEY_LESS:    /* < */
    case UVIEW_KEY_COMMA: return SDL_SCANCODE_COMMA;
    case UVIEW_KEY_GREATER: /* > */
    case UVIEW_KEY_PERIOD: return SDL_SCANCODE_PERIOD;
    case UVIEW_KEY_QUESTION:    /* ? */
    case UVIEW_KEY_SLASH: return SDL_SCANCODE_SLASH;
    
    case UVIEW_KEY_CAPSLOCK: return SDL_SCANCODE_CAPSLOCK;

    case UVIEW_KEY_F1: return SDL_SCANCODE_F1;
    case UVIEW_KEY_F2: return SDL_SCANCODE_F2;
    case UVIEW_KEY_F3: return SDL_SCANCODE_F3;
    case UVIEW_KEY_F4: return SDL_SCANCODE_F4;
    case UVIEW_KEY_F5: return SDL_SCANCODE_F5;
    case UVIEW_KEY_F6: return SDL_SCANCODE_F6;
    case UVIEW_KEY_F7: return SDL_SCANCODE_F7;
    case UVIEW_KEY_F8: return SDL_SCANCODE_F8;
    case UVIEW_KEY_F9: return SDL_SCANCODE_F9;
    case UVIEW_KEY_F10: return SDL_SCANCODE_F10;
    case UVIEW_KEY_F11: return SDL_SCANCODE_F11;
    case UVIEW_KEY_F12: return SDL_SCANCODE_F12;

    case UVIEW_KEY_PRINT: return SDL_SCANCODE_PRINTSCREEN;
    case UVIEW_KEY_SCROLLOCK: return SDL_SCANCODE_SCROLLLOCK;
    case UVIEW_KEY_PAUSE: return SDL_SCANCODE_PAUSE;
    case UVIEW_KEY_INSERT: return SDL_SCANCODE_INSERT;
    case UVIEW_KEY_HOME: return SDL_SCANCODE_HOME;
    case UVIEW_KEY_PAGEUP: return SDL_SCANCODE_PAGEUP;
    case UVIEW_KEY_DELETE: return SDL_SCANCODE_DELETE;
    case UVIEW_KEY_END: return SDL_SCANCODE_END;
    case UVIEW_KEY_PAGEDOWN: return SDL_SCANCODE_PAGEDOWN;
    case UVIEW_KEY_RIGHT: return SDL_SCANCODE_RIGHT;
    case UVIEW_KEY_LEFT: return SDL_SCANCODE_LEFT;
    case UVIEW_KEY_DOWN: return SDL_SCANCODE_DOWN;
    case UVIEW_KEY_UP: return SDL_SCANCODE_UP;

    case UVIEW_KEY_NUMLOCK: return SDL_SCANCODE_NUMLOCKCLEAR;
    case UVIEW_KEY_KP_DIVIDE: return SDL_SCANCODE_KP_DIVIDE;
    case UVIEW_KEY_KP_MULTIPLY: return SDL_SCANCODE_KP_MULTIPLY;
    case UVIEW_KEY_KP_MINUS: return SDL_SCANCODE_KP_MINUS;
    case UVIEW_KEY_KP_PLUS: return SDL_SCANCODE_KP_PLUS;
    case UVIEW_KEY_KP_ENTER: return SDL_SCANCODE_KP_ENTER;
    case UVIEW_KEY_KP1: return SDL_SCANCODE_KP_1;
    case UVIEW_KEY_KP2: return SDL_SCANCODE_KP_2;
    case UVIEW_KEY_KP3: return SDL_SCANCODE_KP_3;
    case UVIEW_KEY_KP4: return SDL_SCANCODE_KP_4;
    case UVIEW_KEY_KP5: return SDL_SCANCODE_KP_5;
    case UVIEW_KEY_KP6: return SDL_SCANCODE_KP_6;
    case UVIEW_KEY_KP7: return SDL_SCANCODE_KP_7;
    case UVIEW_KEY_KP8: return SDL_SCANCODE_KP_8;
    case UVIEW_KEY_KP9: return SDL_SCANCODE_KP_9;
    case UVIEW_KEY_KP0: return SDL_SCANCODE_KP_0;
    case UVIEW_KEY_KP_PERIOD: return SDL_SCANCODE_KP_PERIOD;
    /* SDL_SCANCODE_NONUSBACKSLASH */
    /* SDL_SCANCODE_APPLICATION */
    case UVIEW_KEY_POWER: return SDL_SCANCODE_POWER;
    
    case UVIEW_KEY_KP_EQUALS: return SDL_SCANCODE_KP_EQUALS;
    case UVIEW_KEY_F13: return SDL_SCANCODE_F13;
    case UVIEW_KEY_F14: return SDL_SCANCODE_F14;
    case UVIEW_KEY_F15: return SDL_SCANCODE_F15;
    /* F16~F24 */
    /* SDL_SCANCODE_EXECUTE */
    case UVIEW_KEY_HELP: return SDL_SCANCODE_HELP;
    case UVIEW_KEY_MENU: return SDL_SCANCODE_MENU;
    /* SDL_SCANCODE_SELECT */
    case UVIEW_KEY_EURO: return SDL_SCANCODE_AGAIN; /* REDO */
    case UVIEW_KEY_UNDO: return SDL_SCANCODE_UNDO;
    /*
    SDL_SCANCODE_CUT = 123,
    SDL_SCANCODE_COPY = 124,
    SDL_SCANCODE_PASTE = 125,
    SDL_SCANCODE_FIND = 126,
    SDL_SCANCODE_MUTE = 127,
    SDL_SCANCODE_VOLUMEUP = 128,
    SDL_SCANCODE_VOLUMEDOWN = 129,
    SDL_SCANCODE_KP_COMMA = 133,
    SDL_SCANCODE_KP_EQUALSAS400 = 134,
    */
    #if 0
    SDL_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    SDL_SCANCODE_INTERNATIONAL2 = 136,
    SDL_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
    SDL_SCANCODE_INTERNATIONAL4 = 138,
    SDL_SCANCODE_INTERNATIONAL5 = 139,
    SDL_SCANCODE_INTERNATIONAL6 = 140,
    SDL_SCANCODE_INTERNATIONAL7 = 141,
    SDL_SCANCODE_INTERNATIONAL8 = 142,
    SDL_SCANCODE_INTERNATIONAL9 = 143,
    SDL_SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
    SDL_SCANCODE_LANG2 = 145, /**< Hanja conversion */
    SDL_SCANCODE_LANG3 = 146, /**< Katakana */
    SDL_SCANCODE_LANG4 = 147, /**< Hiragana */
    SDL_SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
    SDL_SCANCODE_LANG6 = 149, /**< reserved */
    SDL_SCANCODE_LANG7 = 150, /**< reserved */
    SDL_SCANCODE_LANG8 = 151, /**< reserved */
    SDL_SCANCODE_LANG9 = 152, /**< reserved */

    SDL_SCANCODE_ALTERASE = 153, /**< Erase-Eaze */
    #endif
    case UVIEW_KEY_SYSREQ: return SDL_SCANCODE_SYSREQ;
    /* SDL_SCANCODE_CANCEL */
    case UVIEW_KEY_CLEAR: return SDL_SCANCODE_CLEAR;
    /*
    SDL_SCANCODE_PRIOR = 157,
    SDL_SCANCODE_RETURN2 = 158,
    SDL_SCANCODE_SEPARATOR = 159,
    SDL_SCANCODE_OUT = 160,
    SDL_SCANCODE_OPER = 161,
    SDL_SCANCODE_CLEARAGAIN = 162,
    SDL_SCANCODE_CRSEL = 163,
    SDL_SCANCODE_EXSEL = 164,
    
    SDL_SCANCODE_KP_00 = 176,
    SDL_SCANCODE_KP_000 = 177,
    SDL_SCANCODE_THOUSANDSSEPARATOR = 178,
    SDL_SCANCODE_DECIMALSEPARATOR = 179,
    SDL_SCANCODE_CURRENCYUNIT = 180,
    SDL_SCANCODE_CURRENCYSUBUNIT = 181,
    SDL_SCANCODE_KP_LEFTPAREN = 182,
    SDL_SCANCODE_KP_RIGHTPAREN = 183,
    SDL_SCANCODE_KP_LEFTBRACE = 184,
    SDL_SCANCODE_KP_RIGHTBRACE = 185,
    SDL_SCANCODE_KP_TAB = 186,
    SDL_SCANCODE_KP_BACKSPACE = 187,
    SDL_SCANCODE_KP_A = 188,
    SDL_SCANCODE_KP_B = 189,
    SDL_SCANCODE_KP_C = 190,
    SDL_SCANCODE_KP_D = 191,
    SDL_SCANCODE_KP_E = 192,
    SDL_SCANCODE_KP_F = 193,
    SDL_SCANCODE_KP_XOR = 194,
    SDL_SCANCODE_KP_POWER = 195,
    SDL_SCANCODE_KP_PERCENT = 196,
    SDL_SCANCODE_KP_LESS = 197,
    SDL_SCANCODE_KP_GREATER = 198,
    SDL_SCANCODE_KP_AMPERSAND = 199,
    SDL_SCANCODE_KP_DBLAMPERSAND = 200,
    SDL_SCANCODE_KP_VERTICALBAR = 201,
    SDL_SCANCODE_KP_DBLVERTICALBAR = 202,
    SDL_SCANCODE_KP_COLON = 203,
    SDL_SCANCODE_KP_HASH = 204,
    SDL_SCANCODE_KP_SPACE = 205,
    SDL_SCANCODE_KP_AT = 206,
    SDL_SCANCODE_KP_EXCLAM = 207,
    SDL_SCANCODE_KP_MEMSTORE = 208,
    SDL_SCANCODE_KP_MEMRECALL = 209,
    SDL_SCANCODE_KP_MEMCLEAR = 210,
    SDL_SCANCODE_KP_MEMADD = 211,
    SDL_SCANCODE_KP_MEMSUBTRACT = 212,
    SDL_SCANCODE_KP_MEMMULTIPLY = 213,
    SDL_SCANCODE_KP_MEMDIVIDE = 214,
    SDL_SCANCODE_KP_PLUSMINUS = 215,
    SDL_SCANCODE_KP_CLEAR = 216,
    SDL_SCANCODE_KP_CLEARENTRY = 217,
    SDL_SCANCODE_KP_BINARY = 218,
    SDL_SCANCODE_KP_OCTAL = 219,
    SDL_SCANCODE_KP_DECIMAL = 220,
    SDL_SCANCODE_KP_HEXADECIMAL = 221,
    */
    case UVIEW_KEY_LCTRL: return SDL_SCANCODE_LCTRL;
    case UVIEW_KEY_LSHIFT: return SDL_SCANCODE_LSHIFT;
    case UVIEW_KEY_LALT: return SDL_SCANCODE_LALT;
    /* SDL_SCANCODE_LGUI */
    case UVIEW_KEY_RCTRL: return SDL_SCANCODE_RCTRL;
    case UVIEW_KEY_RSHIFT: return SDL_SCANCODE_RSHIFT;
    case UVIEW_KEY_RALT: return SDL_SCANCODE_RALT;
    /* SDL_SCANCODE_RGUI */
    case UVIEW_KEY_MODE: return SDL_SCANCODE_MODE;
    /*
    SDL_SCANCODE_AUDIONEXT = 258,
    SDL_SCANCODE_AUDIOPREV = 259,
    SDL_SCANCODE_AUDIOSTOP = 260,
    SDL_SCANCODE_AUDIOPLAY = 261,
    SDL_SCANCODE_AUDIOMUTE = 262,
    SDL_SCANCODE_MEDIASELECT = 263,
    SDL_SCANCODE_WWW = 264,
    SDL_SCANCODE_MAIL = 265,
    SDL_SCANCODE_CALCULATOR = 266,
    SDL_SCANCODE_COMPUTER = 267,
    SDL_SCANCODE_AC_SEARCH = 268,
    SDL_SCANCODE_AC_HOME = 269,
    SDL_SCANCODE_AC_BACK = 270,
    SDL_SCANCODE_AC_FORWARD = 271,
    SDL_SCANCODE_AC_STOP = 272,
    SDL_SCANCODE_AC_REFRESH = 273,
    SDL_SCANCODE_AC_BOOKMARKS = 274,
    */
    #if 0
    SDL_SCANCODE_BRIGHTNESSDOWN = 275,
    SDL_SCANCODE_BRIGHTNESSUP = 276,
    SDL_SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    SDL_SCANCODE_KBDILLUMTOGGLE = 278,
    SDL_SCANCODE_KBDILLUMDOWN = 279,
    SDL_SCANCODE_KBDILLUMUP = 280,
    SDL_SCANCODE_EJECT = 281,
    SDL_SCANCODE_SLEEP = 282,

    SDL_SCANCODE_APP1 = 283,
    SDL_SCANCODE_APP2 = 284,

    SDL_SCANCODE_AUDIOREWIND = 285,
    SDL_SCANCODE_AUDIOFASTFORWARD = 286,
    #endif
    default: break;
    }
    return SDL_SCANCODE_UNKNOWN;
}

SDL_bool SDL_SacncodeVisiable(uint32_t scancode)
{
    if ((scancode >= SDL_SCANCODE_A && scancode <= SDL_SCANCODE_0)
    || (scancode == SDL_SCANCODE_SPACE)
    || (scancode >= SDL_SCANCODE_MINUS && scancode <= SDL_SCANCODE_SLASH)
    || (scancode >= SDL_SCANCODE_KP_DIVIDE && scancode <= SDL_SCANCODE_KP_PERIOD))
        return SDL_TRUE;
    return SDL_FALSE;
}

/**
 * 如果打开了文本输入：
 * 如果按下了组合键，那么就发送组合键+Key中的key。
 * 没有按下组合键，如果是可显示按键，那么就发送成为SDL_TEXTINPUT事件
 * 如果不是可显示按键，就直接发送该按键。
 */
bool BOOKOS_KeyPressEvent(xtk_spirit_t *spirit, xtk_event_t *event, void *arg)
{    
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
    if (window) {
        uint32_t scancode = xtk_code_to_sdl_code(event->key.keycode.code);
        if (data->deviceData->startTextInput) {
            if (event->key.keycode.modify & (UVIEW_KMOD_CTRL | UVIEW_KMOD_ALT)) {    /* 有组合按键，就直接转发按键 */
                if (scancode != SDL_SCANCODE_UNKNOWN)
                    SDL_SendKeyboardKey(SDL_PRESSED, scancode);   
            } else {    /* 没有组合按键，可显示字符就发送给文本，不可显示就直接发送按键值 */
                if (SDL_SacncodeVisiable(scancode) && scancode != SDL_SCANCODE_UNKNOWN) {
                    /* 生产文本字符，产生SDL_TEXTINPUT消息 */
                    char s[2] = {0};
                    s[0] = uview_keypad2ascii(event->key.keycode.code); /* 可显示按键转ASCII */
                    if (s[0])
                        SDL_SendKeyboardText(s);
                } else {
                    if (scancode != SDL_SCANCODE_UNKNOWN)
                        SDL_SendKeyboardKey(SDL_PRESSED, scancode);
                }
            }
        } else {    /* 没有开启文本输入，是什么按键就转发什么按键 */
            if (scancode != SDL_SCANCODE_UNKNOWN)
                SDL_SendKeyboardKey(SDL_PRESSED, scancode);   
        }
    }
    return true;
}

/**
 * 如果打开了文本输入：
 * 如果弹起了组合键，那么就发送组合键+Key中的key。
 * 没有按下组合键，如果是可显示按键，那么忽略不管
 * 如果不是可显示按键，就直接发送该按键。
 */
bool BOOKOS_KeyReleaseEvent(xtk_spirit_t *spirit, xtk_event_t *event, void *arg)
{    
    SDL_Window *window = XTK_WINDOW(spirit)->extension;
    SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
    if (window) {
        uint32_t scancode = xtk_code_to_sdl_code(event->key.keycode.code);
        if (data->deviceData->startTextInput) {
            if (event->key.keycode.modify & (UVIEW_KMOD_SHIFT | UVIEW_KMOD_CTRL | UVIEW_KMOD_ALT)) {    /* 有组合按键，就直接转发按键 */
                if (scancode != SDL_SCANCODE_UNKNOWN)
                    SDL_SendKeyboardKey(SDL_RELEASED, scancode);   
            } else {    /* 没有组合按键，不可显示就直接发送按键值 */
                if (!SDL_SacncodeVisiable(scancode)) {
                    if (scancode != SDL_SCANCODE_UNKNOWN)
                        SDL_SendKeyboardKey(SDL_RELEASED, scancode);
                }
            }
        } else {    /* 没有开启文本输入，是什么按键就转发什么按键 */
            if (scancode != SDL_SCANCODE_UNKNOWN)
                SDL_SendKeyboardKey(SDL_RELEASED, scancode);   
        }
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
        // printf("SDL PumpEvents: xtk poll quit event!\n");
    }
}


#endif /* SDL_VIDEO_DRIVER_BOOKOS */

/* vi: set ts=4 sw=4 expandtab: */
