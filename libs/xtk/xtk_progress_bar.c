#include "xtk_progress_bar.h"
#include <stdlib.h>
#include <stdio.h>

static void __xtk_progress_bar_update(xtk_progress_bar_t *pbar)
{
    if (pbar->fraction > 1.0f)
        pbar->fraction = 1.0f;
    if (pbar->fraction < 0)
        pbar->fraction = 0;

    int w = pbar->spirit.surface->w;
    int h = pbar->spirit.surface->h;
    uint32_t len;
    uint32_t color = pbar->front_color;
    
    xtk_surface_rectfill(pbar->spirit.surface, 0, 0, w, h, pbar->spirit.style.background_color);
    switch (pbar->orientation) {
    case XTK_PROGRESS_LEFT_TO_RIGHT:
        len = (uint32_t) (pbar->fraction * w);
        xtk_surface_rectfill(pbar->spirit.surface, 0, 0,  len, h, color);
        break;
    case XTK_PROGRESS_RIGHT_TO_LEFT:
        len = (uint32_t) (pbar->fraction * w);
        xtk_surface_rectfill(pbar->spirit.surface, w - len, 0, len, h, color);
        break;
    case XTK_PROGRESS_BOTTOM_TO_TOP:
        len = (uint32_t) (pbar->fraction * h);
        xtk_surface_rectfill(pbar->spirit.surface, 0, h - len, w, len, color);
        break;
    case XTK_PROGRESS_TOP_TO_BOTTOM:
        len = (uint32_t) (pbar->fraction * h);
        xtk_surface_rectfill(pbar->spirit.surface, 0, 0, w, len, color);
        break;
    default:
        break;
    }
}

xtk_spirit_t *xtk_progress_bar_create(void)
{
    xtk_progress_bar_t *pbar = malloc(sizeof(xtk_progress_bar_t));
    if (!pbar)
        return NULL;
    pbar->fraction = 0.0f;
    pbar->step = 0.01f;
    pbar->orientation = XTK_PROGRESS_LEFT_TO_RIGHT;
    pbar->front_color = XTK_PBAR_BAR_COLOR;

    xtk_spirit_t *spirit = &pbar->spirit;
    xtk_spirit_init(spirit, 0, 0, XTK_PBAR_WIDTH_DEFAULT, XTK_PBAR_HEIGHT_DEFAULT);

    xtk_spirit_set_type(spirit, XTK_SPIRIT_TYPE_PBAR);
    
    spirit->style.align = XTK_ALIGN_CENTER;
    spirit->style.background_color = XTK_PBAR_BACK_COLOR;
    spirit->style.color = XTK_WHITE;
    spirit->style.border_color = XTK_BLACK;
    
    xtk_spirit_set_surface(spirit, xtk_surface_create(spirit->width, spirit->height));
    __xtk_progress_bar_update(pbar);
    return spirit;
}

void xtk_progress_bar_set_fraction(xtk_progress_bar_t *pbar,
                                    double fraction)
{
    if (pbar) {
        pbar->fraction = fraction;
        __xtk_progress_bar_update(pbar);
        xtk_spirit_show(&pbar->spirit);
    }
}

double xtk_progress_bar_get_fraction(xtk_progress_bar_t *pbar)
{
    return pbar != NULL ? pbar->fraction : 0.0f;
}

void xtk_progress_bar_set_orientation(xtk_progress_bar_t *pbar,
                                      xtk_progress_bar_orientation_t orientation)
{
    if (pbar) {
        pbar->orientation = orientation;
        __xtk_progress_bar_update(pbar);
        xtk_spirit_show(&pbar->spirit);
    }
}

xtk_progress_bar_orientation_t xtk_progress_bar_get_orientation(xtk_progress_bar_t *pbar)
{
    return pbar != NULL ? pbar->orientation : XTK_PROGRESS_LEFT_TO_RIGHT;
}

void xtk_progress_bar_pulse(xtk_progress_bar_t *pbar)
{
    if (pbar) {
        pbar->fraction += pbar->step;
        __xtk_progress_bar_update(pbar);
        xtk_spirit_show(&pbar->spirit);
    }
}

void xtk_progress_bar_set_pulse_step(xtk_progress_bar_t *pbar,
                                    double fraction)
{
    if (pbar)
        pbar->step = fraction;
}

double xtk_progress_bar_get_pulse_step(xtk_progress_bar_t *pbar)
{
    return pbar != NULL ? pbar->step : 0.0f;
}

void xtk_progress_bar_set_text(xtk_progress_bar_t *pbar,
                               const char *text)
{
    if (pbar != NULL) {
        xtk_spirit_set_text(&pbar->spirit, (char *)text);
        __xtk_progress_bar_update(pbar);
        xtk_spirit_show(&pbar->spirit);
    }
}

const char *xtk_progress_bar_get_text(xtk_progress_bar_t *pbar)
{
    if (pbar == NULL)
        return NULL;
    return pbar->spirit.text;
}