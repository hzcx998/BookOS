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

// #define SDL_VIDEO_DRIVER_BOOKOS 1

#if SDL_VIDEO_DRIVER_BOOKOS

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../SDL_sysvideo.h"

#include "SDL_mouse.h"
#include "../../events/SDL_mouse_c.h"
#include "SDL_bookos.h"
#include "SDL_bookosevents.h"

#include "SDL_assert.h"

typedef struct {
    uview_mouse_state_info_t info;
} BOOKOS_CursorData;

static SDL_Cursor *
BOOKOS_CreateCursor(SDL_Surface *surface, int hot_x, int hot_y)
{
    SDL_Cursor *cursor;

    cursor = calloc(1, sizeof (*cursor));
    if (cursor) {
        SDL_VideoDevice *vd = SDL_GetVideoDevice ();
        BOOKOS_CursorData *data = calloc (1, sizeof (BOOKOS_CursorData));
        if (!data) {
            SDL_OutOfMemory();
            free(cursor);
            return NULL;
        }
        cursor->driverdata = (void *) data;

        /* Assume ARGB8888 */
        SDL_assert(surface->format->format == SDL_PIXELFORMAT_ARGB8888);
        SDL_assert(surface->pitch == surface->w * 4);

        /* Allocate shared memory buffer for this cursor */
        data->info.bmp = uview_bitmap_create(surface->w, surface->h);

        if (!data->info.bmp)
        {
            free (cursor->driverdata);
            free (cursor);
            return NULL;
        }
        
        SDL_memcpy(data->info.bmp->bits,
                   surface->pixels,
                   surface->h * surface->pitch);

        data->info.off_x = hot_x;
        data->info.off_y = hot_y;
        data->info.state = UVIEW_MOUSE_NORMAL;
    } else {
        SDL_OutOfMemory();
    }
    
    printf("BOOKOS_CreateCursor:%x\n", cursor);
    return cursor;
}

static SDL_Cursor *
CreateCursorFromWlCursor(SDL_VideoData *d, uview_mouse_state_t state)
{
    SDL_Cursor *cursor;
 
    cursor = calloc(1, sizeof (*cursor));
    if (cursor) {
        BOOKOS_CursorData *data = calloc (1, sizeof (BOOKOS_CursorData));
        if (!data) {
            SDL_OutOfMemory();
            free(cursor);
            return NULL;
        }
        cursor->driverdata = (void *) data;

        uview_mouse_state_info_t info;
        info.state = state;
        info.bmp = uview_bitmap_create(32, 32);
        if (!info.bmp) {
            SDL_OutOfMemory();
            free(data);
            free(cursor);
            return NULL;
        }
        #if 1
        uview_get_mouse_state_info_noview(&info);
        #else
        if (uview_get_mouse_state_info_noview(&info) < 0)  {
            SDL_OutOfMemory();
            free(info.bmp);
            free(data);
            free(cursor);
            return NULL;
        }
        #endif
        data->info.bmp = info.bmp;
        data->info.off_x = info.off_x;
        data->info.off_y = info.off_y;
        data->info.state = state;
    } else {
        SDL_OutOfMemory ();
    }
    printf("CreateCursorFromWlCursor:%x\n", cursor);
    return cursor;
}

static SDL_Cursor *
BOOKOS_CreateDefaultCursor()
{
    SDL_VideoDevice *device = SDL_GetVideoDevice();
    SDL_VideoData *data = device->driverdata;

    return CreateCursorFromWlCursor (data,
                                     UVIEW_MOUSE_NORMAL);
}

static SDL_Cursor *
BOOKOS_CreateSystemCursor(SDL_SystemCursor id)
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *d = vd->driverdata;

    uview_mouse_state_t state;
    switch(id)
    {
    default:
        SDL_assert(0);
        return NULL;
    case SDL_SYSTEM_CURSOR_ARROW:
        state = UVIEW_MOUSE_NORMAL;
        break;
    case SDL_SYSTEM_CURSOR_IBEAM:
        state = UVIEW_MOUSE_TEXT;
        break;
    case SDL_SYSTEM_CURSOR_WAIT:
        state = UVIEW_MOUSE_BUSY;
        break;
    case SDL_SYSTEM_CURSOR_CROSSHAIR:
        state = UVIEW_MOUSE_NORMAL;
        break;
    case SDL_SYSTEM_CURSOR_WAITARROW:
        state = UVIEW_MOUSE_BUSY;
        break;
    case SDL_SYSTEM_CURSOR_SIZENWSE:
        state = UVIEW_MOUSE_DRESIZE1;
        break;
    case SDL_SYSTEM_CURSOR_SIZENESW:
        state = UVIEW_MOUSE_DRESIZE2;
        break;
    case SDL_SYSTEM_CURSOR_SIZEWE:
        state = UVIEW_MOUSE_HRESIZE;
        break;
    case SDL_SYSTEM_CURSOR_SIZENS:
        state = UVIEW_MOUSE_VRESIZE;
        break;
    case SDL_SYSTEM_CURSOR_SIZEALL:
        state = UVIEW_MOUSE_RESIZEALL;
        break;
    case SDL_SYSTEM_CURSOR_NO:
        state = UVIEW_MOUSE_UNUSABLE;
        break;
    case SDL_SYSTEM_CURSOR_HAND:
        state = UVIEW_MOUSE_HAND;
        break;
    }
    return CreateCursorFromWlCursor(d, state);
}

static void
BOOKOS_FreeCursor(SDL_Cursor *cursor)
{
    BOOKOS_CursorData *d;

    if (!cursor)
        return;

    d = cursor->driverdata;

    /* Probably not a cursor we own */
    if (!d)
        return;

    if (d->info.bmp)
        uview_bitmap_destroy(d->info.bmp);

    /* Not sure what's meant to happen to shm_data */
    free (cursor->driverdata);
    SDL_free(cursor);
}

static int
BOOKOS_ShowCursor(SDL_Cursor *cursor)
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *d = vd->driverdata;
    BOOKOS_CursorData *data = cursor->driverdata;
    if (cursor)
    {
        
        printf("show cursor:%d\n", data->info.state);
        #if 0
        /* 将数据中的内容写入到光标中 */
        if (uview_set_mouse_state_info_noview(&data->info) < 0)
            printf("show cursor:%d failed!\n", data->info.state);
        #else
        if (uview_set_mouse_state_noview(data->info.state) < 0)
            printf("show cursor:%d failed!\n", data->info.state);
        #endif
    }
    else
    {
        printf("hide cursor:%d\n", data->info.state);
        /* 清空光标内容 */
        // uview_set_mouse_state_info_noview(data->info.state, &data->info);
    }
    
    return 0;
}

static void
BOOKOS_WarpMouse(SDL_Window *window, int x, int y)
{
    SDL_Unsupported();
}

static int
BOOKOS_WarpMouseGlobal(int x, int y)
{
    return SDL_Unsupported();
}

static int
BOOKOS_SetRelativeMouseMode(SDL_bool enabled)
{
    SDL_VideoDevice *vd = SDL_GetVideoDevice();
    SDL_VideoData *data = (SDL_VideoData *) vd->driverdata;
    /* enabled：true表示锁住鼠标移动，false：表示解锁鼠标移动 */
    if (enabled)
        return SDL_Unsupported();
    else
        return SDL_Unsupported();
}

void
BOOKOS_InitMouse(void)
{
    SDL_Mouse *mouse = SDL_GetMouse();

    mouse->CreateCursor = BOOKOS_CreateCursor;
    mouse->CreateSystemCursor = BOOKOS_CreateSystemCursor;
    mouse->ShowCursor = BOOKOS_ShowCursor;
    mouse->FreeCursor = BOOKOS_FreeCursor;
    mouse->WarpMouse = BOOKOS_WarpMouse;
    mouse->WarpMouseGlobal = BOOKOS_WarpMouseGlobal;
    mouse->SetRelativeMouseMode = BOOKOS_SetRelativeMouseMode;

    SDL_SetDefaultCursor(BOOKOS_CreateDefaultCursor());
}

void
BOOKOS_FiniMouse(void)
{
    /* This effectively assumes that nobody else
     * touches SDL_Mouse which is effectively
     * a singleton */
}
#endif  /* SDL_VIDEO_DRIVER_WAYLAND */
