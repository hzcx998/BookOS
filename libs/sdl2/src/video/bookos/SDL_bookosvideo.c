/*
  Simple DirectMedia Layer
  Copyright (C) 2017 BlackBerry Limited

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
#include "SDL_assert.h"
#include "../SDL_sysvideo.h"
#include "../SDL_sysvideo.h"
#include "sdl_bookos.h"
#include "sdl_bookosevents.h"

#include <stdio.h>

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
    SDL_VideoDisplay display;

    if (xtk_init(NULL, NULL)) {
        return -1;
    }
    
    SDL_zero(display);

    if (SDL_AddVideoDisplay(&display) < 0) {
        return -1;
    }

    _this->num_displays = 1;
    return 0;
}

static void
BOOKOS_VideoQuit(_THIS)
{
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
    window_impl_t   *impl;
    if (window->flags & SDL_WINDOW_OPENGL) {
        perror("xbook do not support OPENGL!\n");
        return -1;
    }
    
    impl = SDL_calloc(1, sizeof(*impl));
    if (impl == NULL) {
        return -1;
    }

    impl->window = xtk_window_create(XTK_WINDOW_TOPLEVEL);
    if (!impl->window) {
        goto fail;
    }
    xtk_window_t *pwin = XTK_WINDOW(impl->window);
    if (xtk_window_set_default_size(pwin, window->w, window->h) < 0) {
        perror("xbook window size invalid!\n");
        goto fail;
    }
    if (xtk_window_set_position(pwin, XTK_WIN_POS_CENTER) < 0) {
        perror("xbook window set position failed!\n");
        goto fail;
    }
    /*  */
    xtk_window_paint_callback(pwin, BOOKOS_WindowPaint);
    /* close window signal handle */
    xtk_signal_connect(impl->window, "delete_event",
                        XTK_CALLBACK(BOOKOS_QuitEvent), NULL);
    
    /* mouse events */
    xtk_signal_connect(impl->window, "enter_notify", 
                        XTK_CALLBACK(BOOKOS_MouseEnterEvent), NULL);
    xtk_signal_connect(impl->window, "leave_notify", 
                        XTK_CALLBACK(BOOKOS_MouseLeaveEvent), NULL);
    xtk_signal_connect(impl->window, "motion_notify", 
                        XTK_CALLBACK(BOOKOS_MouseMotionEvent), NULL);
    xtk_signal_connect(impl->window, "button_press", 
                        XTK_CALLBACK(BOOKOS_ButtonPressEvent), NULL);
    xtk_signal_connect(impl->window, "button_release", 
                        XTK_CALLBACK(BOOKOS_ButtonReleaseEvent), NULL);
    xtk_signal_connect(impl->window, "button_scroll", 
                        XTK_CALLBACK(BOOKOS_ButtonScrollEvent), NULL);
    
    pwin->extension = window;
    window->driverdata = impl;
    return 0;
fail:
    if (impl->window) {
        xtk_spirit_destroy(impl->window);
    }
    SDL_free(impl);

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
    window_impl_t   *impl = (window_impl_t *)window->driverdata;
    *pixels = (void *)xtk_window_get_surface(XTK_WINDOW(impl->window))->pixels;  
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
    window_impl_t   *impl = (window_impl_t *)window->driverdata;
    int x1 = rects->x, y1 = rects->y, x2 = 0, y2 = 0;
    while (--numrects >= 0) {
        x1 = SDL_min(x1, rects->x);
        y1 = SDL_min(y1, rects->y);
        x2 = SDL_max(x2, rects->x + rects->w);
        y2 = SDL_max(y2, rects->y + rects->h);
        rects++;
    }
    xtk_window_update(XTK_WINDOW(impl->window), x1, y1, x2 - x1, y2 - y1);
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
    window_impl_t   *impl = (window_impl_t *)window->driverdata;
    xtk_window_set_default_size(XTK_WINDOW(impl->window), window->w, window->h);
}

/**
 * Makes the native window associated with the given SDL window visible.
 * @param   _THIS
 * @param   window  SDL window to update
 */
static void
BOOKOS_ShowWindow(_THIS, SDL_Window *window)
{
    window_impl_t   *impl = (window_impl_t *)window->driverdata;
    xtk_spirit_show(impl->window);    
}

/**
 * Makes the native window associated with the given SDL window invisible.
 * @param   _THIS
 * @param   window  SDL window to update
 */
static void
BOOKOS_HideWindow(_THIS, SDL_Window *window)
{
    window_impl_t   *impl = (window_impl_t *)window->driverdata;
    xtk_spirit_hide(impl->window);    
}

/**
 * Destroys the native window associated with the given SDL window.
 * @param   _THIS
 * @param   window  SDL window that is being destroyed
 */
static void
BOOKOS_DestroyWindow(_THIS, SDL_Window *window)
{
    window_impl_t   *impl = (window_impl_t *)window->driverdata;

    if (impl) {
        xtk_spirit_destroy(impl->window);
        if (window->driverdata) {
            SDL_free(window->driverdata);
        }
        window->driverdata = NULL;
    }
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

    device->driverdata = NULL;
    device->VideoInit = BOOKOS_VideoInit;
    device->VideoQuit = BOOKOS_VideoQuit;
    device->CreateSDLWindow = BOOKOS_CreateWindow;
    device->CreateWindowFramebuffer = BOOKOS_CreateWindowFramebuffer;
    device->UpdateWindowFramebuffer = BOOKOS_UpdateWindowFramebuffer;
    device->SetWindowSize = BOOKOS_SetWindowSize;
    device->ShowWindow = BOOKOS_ShowWindow;
    device->HideWindow = BOOKOS_HideWindow;
    device->PumpEvents = BOOKOS_PumpEvents;
    device->DestroyWindow = BOOKOS_DestroyWindow;

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