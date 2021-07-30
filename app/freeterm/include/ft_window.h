#ifndef _FREETERM_WINDOW_H
#define _FREETERM_WINDOW_H

#include <xtk.h>

#define SH_WIN_WIDTH    720
#define SH_WIN_HEIGHT   560

typedef struct {
    xtk_spirit_t *win;
    xtk_surface_t *surf;
} sh_window_t;

extern sh_window_t sh_window;

int init_window();
void window_loop();
int exit_window();

void set_win_proc(int state);
void sh_window_rect_fill(int x, int y, uint32_t width, uint32_t height, uint32_t color);
void sh_window_rect(int x, int y, uint32_t width, uint32_t height, uint32_t color);
void sh_window_char(int x, int y, char ch, uint32_t color);
int sh_window_size(uint32_t *w, uint32_t *h);
void sh_window_update(int left, int top, int right, int bottom);
int poll_window();

#endif /* _FREETERM_WINDOW_H */