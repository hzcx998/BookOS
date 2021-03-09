/*
  Sdatae DirectMedia Layer
  Copyright (C) 2017 BlackBerry Limited

  This software is provided 'as-is', without any express or dataied
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
#include "SDL_assert.h"
#include "../SDL_sysvideo.h"
#include "../SDL_sysvideo.h"
#include "sdl_bookos.h"
#include "SDL_bookosevents.h"

#include <stdio.h>

int BOOKOS_GetCurrentVideoMode(SDL_DisplayMode *mode)
{
    xtk_spirit_t *spirit = xtk_window_create(XTK_WINDOW_POPUP);
    if (!spirit)    /* no device */
        return -1;
    if (xtk_window_get_screen(XTK_WINDOW(spirit), &mode->w, &mode->h) < 0) {    
        xtk_spirit_destroy(spirit);
        return -1;
    }
    xtk_spirit_destroy(spirit);
    mode->refresh_rate = 0;
    mode->format = SDL_PIXELFORMAT_ARGB8888;
    mode->driverdata = NULL;
    // printf("BOOKOS_GetCurrentVideoMode: %dx%d@%d.%x\n", mode->w, mode->h, mode->refresh_rate, mode->format);
    return 0;
}

/**
 * Initializes the BOOKOS video plugin.
 * Creates the Screen context and event handles used for all window operations
 * by the plugin.
 * @param   _THIS
 * @return  0 if successful, -1 on error
 */
static int
BOOKOS_VideoInit(_THIS)
{
    if (xtk_init(NULL, NULL)) {
        return -1;
    }
    SDL_VideoDisplay display;
    SDL_DisplayMode current_mode;
    BOOKOS_GetCurrentVideoMode(&current_mode);
    
    SDL_zero(display);
    display.desktop_mode = current_mode;
    display.current_mode = current_mode;
    display.driverdata = NULL;
    if (SDL_AddVideoDisplay(&display) < 0) {
        return -1;
    }

    _this->num_displays = 1;
    return 0;
}

static void
BOOKOS_VideoQuit(_THIS)
{
    xtk_exit();
}

/**
 * Creates a new native Screen window and associates it with the given SDL
 * window.
 * @param   _THIS
 * @param   window  SDL window to initialize
 * @return  0 if successful, -1 on error
 */
static int
BOOKOS_CreateWindow(_THIS, SDL_Window *window)
{
    SDL_WindowData *data;
    if (window->flags & SDL_WINDOW_OPENGL) {
        perror("bookos do not support OPENGL!\n");
        return -1;
    }
    
    data = SDL_calloc(1, sizeof(*data));
    if (data == NULL) {
        return -1;
    }

    data->window = xtk_window_create(XTK_WINDOW_TOPLEVEL);
    if (!data->window) {
        goto fail;
    }
    data->deviceData = (SDL_VideoDeviceData *)_this->driverdata;   /* 指向设备数据 */

    xtk_window_t *pwin = XTK_WINDOW(data->window);
    if (xtk_window_set_default_size(pwin, window->w, window->h) < 0) {
        perror("bookos window size invalid!\n");
        goto fail;
    }
    if (xtk_window_set_position(pwin, XTK_WIN_POS_CENTER) < 0) {
        perror("bookos window set position failed!\n");
        goto fail;
    }

    /*  */
    xtk_window_paint_callback(pwin, BOOKOS_WindowPaint);
    /* window events */
    xtk_signal_connect(data->window, "delete_event",
                        XTK_CALLBACK(BOOKOS_QuitEvent), NULL);
    xtk_signal_connect(data->window, "active",
                        XTK_CALLBACK(BOOKOS_WindowActiveEvent), NULL);
    xtk_signal_connect(data->window, "inactive",
                        XTK_CALLBACK(BOOKOS_WindowInactiveEvent), NULL);
    xtk_signal_connect(data->window, "move_event",
                        XTK_CALLBACK(BOOKOS_WindowMoveEvent), NULL);
    xtk_signal_connect(data->window, "minim_notify",
                        XTK_CALLBACK(BOOKOS_WindowMinimEvent), NULL);
    xtk_signal_connect(data->window, "maxim_notify",
                        XTK_CALLBACK(BOOKOS_WindowMaximEvent), NULL);
    xtk_signal_connect(data->window, "restore_notify",
                        XTK_CALLBACK(BOOKOS_WindowRestoreEvent), NULL);
    xtk_signal_connect(data->window, "resize_event",
                        XTK_CALLBACK(BOOKOS_WindowResizeEvent), NULL);
    
    /* mouse events */
    xtk_signal_connect(data->window, "enter_notify", 
                        XTK_CALLBACK(BOOKOS_MouseEnterEvent), NULL);
    xtk_signal_connect(data->window, "leave_notify", 
                        XTK_CALLBACK(BOOKOS_MouseLeaveEvent), NULL);
    xtk_signal_connect(data->window, "motion_notify", 
                        XTK_CALLBACK(BOOKOS_MouseMotionEvent), NULL);
    xtk_signal_connect(data->window, "button_press", 
                        XTK_CALLBACK(BOOKOS_ButtonPressEvent), NULL);
    xtk_signal_connect(data->window, "button_release", 
                        XTK_CALLBACK(BOOKOS_ButtonReleaseEvent), NULL);
    xtk_signal_connect(data->window, "button_scroll", 
                        XTK_CALLBACK(BOOKOS_ButtonScrollEvent), NULL);
    
    /* keyboard events */
    xtk_signal_connect(data->window, "key_press", 
                        XTK_CALLBACK(BOOKOS_KeyPressEvent), NULL);
    xtk_signal_connect(data->window, "key_release", 
                        XTK_CALLBACK(BOOKOS_KeyReleaseEvent), NULL);
    
    pwin->extension = window;
    window->driverdata = data;  /* SDL_Window->driverdata -> SDL_WindowData */
    
    return 0;
fail:
    if (data->window) {
        xtk_spirit_destroy(data->window);
    }
    SDL_free(data);

    return -1;
}

/**
 * Gets a pointer to the Screen buffer associated with the given window. Note
 * that the buffer is actually created in createWindow().
 * @param       _THIS
 * @param       window  SDL window to get the buffer for
 * @param[out]  pixles  Holds a pointer to the window's buffer
 * @param[out]  format  Holds the pixel format for the buffer
 * @param[out]  pitch   Holds the number of bytes per line
 * @return  0 if successful, -1 on error
 */
static int
BOOKOS_CreateWindowFramebuffer(_THIS, SDL_Window * window, Uint32 * format,
                        void ** pixels, int *pitch)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    *pixels = (void *)xtk_window_get_surface(XTK_WINDOW(data->window))->pixels;  
    *format = SDL_PIXELFORMAT_ARGB8888;
    /* Calculate pitch */
    *pitch = (((window->w * SDL_BYTESPERPIXEL(*format)) + 3) & ~3);
    return 0;
}

/**
 * Informs the window manager that the window needs to be updated.
 * @param   _THIS
 * @param   window      The window to update
 * @param   rects       An array of reectangular areas to update
 * @param   numrects    Rect array length
 * @return  0 if successful, -1 on error
 */
static int
BOOKOS_UpdateWindowFramebuffer(_THIS, SDL_Window *window, const SDL_Rect *rects,
                        int numrects)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    int x1 = rects->x, y1 = rects->y, x2 = 0, y2 = 0;
    while (--numrects >= 0) {
        x1 = SDL_min(x1, rects->x);
        y1 = SDL_min(y1, rects->y);
        x2 = SDL_max(x2, rects->x + rects->w);
        y2 = SDL_max(y2, rects->y + rects->h);
        rects++;
    }
    xtk_window_update(XTK_WINDOW(data->window), x1, y1, x2 - x1, y2 - y1);
    return 0;
}

/**
 * Updates the size of the native window using the geometry of the SDL window.
 * @param   _THIS
 * @param   window  SDL window to update
 */
static void
BOOKOS_SetWindowSize(_THIS, SDL_Window *window)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    xtk_window_set_default_size(XTK_WINDOW(data->window), window->w, window->h);
}

/**
 * Makes the native window associated with the given SDL window visible.
 * @param   _THIS
 * @param   window  SDL window to update
 */
static void
BOOKOS_ShowWindow(_THIS, SDL_Window *window)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    xtk_spirit_show(data->window);    
}

/**
 * Makes the native window associated with the given SDL window invisible.
 * @param   _THIS
 * @param   window  SDL window to update
 */
static void
BOOKOS_HideWindow(_THIS, SDL_Window *window)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    xtk_spirit_hide(data->window);    
}

/**
 * Destroys the native window associated with the given SDL window.
 * @param   _THIS
 * @param   window  SDL window that is being destroyed
 */
static void
BOOKOS_DestroyWindow(_THIS, SDL_Window *window)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;

    if (data) {
        xtk_spirit_destroy(data->window);
        if (window->driverdata) {
            SDL_free(window->driverdata);
        }
        window->driverdata = NULL;
    }
}

static void 
BOOKOS_SetWindowTitle(_THIS, SDL_Window * window)
{
    SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
    char *title_locale = NULL;
    title_locale = SDL_iconv_utf8_locale(window->title);
    if (!title_locale) {
        SDL_OutOfMemory();
        return;
    }
    xtk_window_set_title(XTK_WINDOW(data->window), title_locale);
    SDL_free(title_locale);
}

static void BOOKOS_SetWindowPosition(_THIS, SDL_Window * window)
{
    SDL_WindowData *data = (SDL_WindowData *) window->driverdata;
    xtk_window_set_position_absolute(XTK_WINDOW(data->window), window->x, window->y);
}


static void BOOKOS_SetWindowResizable(_THIS, SDL_Window * window, SDL_bool resizable)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    xtk_window_set_resizable(XTK_WINDOW(data->window), (bool) resizable);
}

static void
BOOKOS_MaximizeWindow(_THIS, SDL_Window * window)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    xtk_window_maxim(XTK_WINDOW(data->window));
}

static void
BOOKOS_MinimizeWindow(_THIS, SDL_Window * window)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    xtk_window_minim(XTK_WINDOW(data->window));
}

static void
BOOKOS_RestoreWindow(_THIS, SDL_Window * window)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    xtk_window_restore(XTK_WINDOW(data->window));
}

static void
BOOKOS_SetWindowFullscreen(_THIS, SDL_Window * window, SDL_VideoDisplay * display, SDL_bool fullscreen)
{
    SDL_WindowData *data = (SDL_WindowData *)window->driverdata;
    /* TODO: use real full screen to instead maxim&restore */
    if (fullscreen) {
        xtk_window_maxim(XTK_WINDOW(data->window));
    } else {
        xtk_window_restore(XTK_WINDOW(data->window));
    }
}

static void BOOKOS_GetDisplayModes(_THIS, SDL_VideoDisplay *display) 
{
    /* FIXME: get modes from hardware, only one modes now. */
    SDL_DisplayMode mode;
    if (!BOOKOS_GetCurrentVideoMode(&mode)) {
        SDL_AddDisplayMode(display, &mode);
    }
}

static int BOOKOS_SetDisplayMode(_THIS, SDL_VideoDisplay *display, SDL_DisplayMode *mode)
{
    // printf("BOOKOS_SetDisplayMode: %dx%d@%d.%x\n", mode->w, mode->h, mode->refresh_rate, mode->format);
    
    /* do not support now! */
    return 0;
}

/**
 * 开始接收文本输入，按键产生文本字符串
 */
static void
BOOKOS_StartTextInput(_THIS)
{
    SDL_VideoDeviceData *data = (SDL_VideoDeviceData *) _this->driverdata;
    data->startTextInput = 1;
}

/**
 * 结束文本输入，按键不产生文本字符串，是默认的按键值
 */
static void
BOOKOS_StopTextInput(_THIS)
{
    SDL_VideoDeviceData *data = (SDL_VideoDeviceData *) _this->driverdata;
    data->startTextInput = 0;
#ifdef SDL_USE_IME
    SDL_IME_Reset();
#endif
}

/**
 * 设置输入法已经输入的文本的区域
 */
static void
BOOKOS_SetTextInputRect(_THIS, SDL_Rect *rect)
{
#ifdef SDL_USE_IME
    SDL_IME_UpdateTextRect(rect);
#endif
}

/**
 * Frees the plugin object created by createDevice().
 * @param   device  Plugin object to free
 */
static void
BOOKOS_DeleteDevice(SDL_VideoDevice *device)
{
    SDL_free(device);

}

/**
 * Creates the BOOKOS video plugin used by SDL.
 * @param   devindex    Unused
 * @return  Initialized device if successful, NULL otherwise
 */
static SDL_VideoDevice *
BOOKOS_CreateDevice(int devindex)
{
    SDL_VideoDevice *device;

    device = (SDL_VideoDevice *)SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (device == NULL) {
        return NULL;
    }

    SDL_VideoDeviceData *data;
    data = (SDL_VideoDeviceData *)SDL_calloc(1, sizeof(SDL_VideoDeviceData));
    if (data == NULL) {
        SDL_free(device);
        return NULL;
    }
    data->startTextInput = 0;   /* 默认没打开 */
    device->driverdata = data;
    device->VideoInit = BOOKOS_VideoInit;
    device->VideoQuit = BOOKOS_VideoQuit;
    device->SetDisplayMode = BOOKOS_SetDisplayMode;
    device->GetDisplayModes = BOOKOS_GetDisplayModes;
    device->CreateSDLWindow = BOOKOS_CreateWindow;
    device->CreateWindowFramebuffer = BOOKOS_CreateWindowFramebuffer;
    device->UpdateWindowFramebuffer = BOOKOS_UpdateWindowFramebuffer;
    device->SetWindowTitle = BOOKOS_SetWindowTitle;
    device->SetWindowSize = BOOKOS_SetWindowSize;
    device->SetWindowPosition = BOOKOS_SetWindowPosition;
    device->SetWindowResizable = BOOKOS_SetWindowResizable;
    device->MaximizeWindow = BOOKOS_MaximizeWindow;
    device->MinimizeWindow = BOOKOS_MinimizeWindow;
    device->RestoreWindow = BOOKOS_RestoreWindow;
    device->SetWindowFullscreen = BOOKOS_SetWindowFullscreen;
    device->ShowWindow = BOOKOS_ShowWindow;
    device->HideWindow = BOOKOS_HideWindow;
    device->PumpEvents = BOOKOS_PumpEvents;
    device->DestroyWindow = BOOKOS_DestroyWindow;
    /* TODO: 
    device->SetClipboardText;
    device->GetClipboardText;
    device->HasClipboardText;
    */
    device->StartTextInput = BOOKOS_StartTextInput;
    device->StopTextInput = BOOKOS_StopTextInput;
    device->SetTextInputRect = BOOKOS_SetTextInputRect;

    device->free = BOOKOS_DeleteDevice;
    return device;
}

static int
BOOKOS_Available()
{
    return 1;
}

VideoBootStrap BOOKOS_bootstrap = {
    "bookos", "BOOKOS Screen",
    BOOKOS_Available, BOOKOS_CreateDevice
};
