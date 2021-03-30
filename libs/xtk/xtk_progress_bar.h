#ifndef _LIB_XTK_PROGRESS_BAR_H
#define _LIB_XTK_PROGRESS_BAR_H

#include <stddef.h>
#include "xtk_spirit.h"
#include "xtk_label.h"

typedef enum {
    XTK_PROGRESS_LEFT_TO_RIGHT,  //从左向右
    XTK_PROGRESS_RIGHT_TO_LEFT,  //从右向左
    XTK_PROGRESS_BOTTOM_TO_TOP,  //从下向上
    XTK_PROGRESS_TOP_TO_BOTTOM,  //从上向下
} xtk_progress_bar_orientation_t;

typedef struct {
    xtk_spirit_t spirit;
    xtk_progress_bar_orientation_t orientation;
    xtk_color_t front_color;
    double fraction;    // 进度值
    double step;        // 步进值
} xtk_progress_bar_t;

#define XTK_PROGRESS_BAR(spirit)  ((xtk_progress_bar_t *)(spirit))

#define XTK_PBAR_BACK_COLOR     XTK_RGB(192, 192, 192)
#define XTK_PBAR_BAR_COLOR      XTK_RGB(128, 128, 128)

#define XTK_PBAR_WIDTH_DEFAULT 100
#define XTK_PBAR_HEIGHT_DEFAULT 20

xtk_spirit_t *xtk_progress_bar_create(void);

void xtk_progress_bar_set_fraction(xtk_progress_bar_t *pbar,
                                    double fraction);
double xtk_progress_bar_get_fraction(xtk_progress_bar_t *pbar);

void xtk_progress_bar_set_orientation(xtk_progress_bar_t *pbar,
                                      xtk_progress_bar_orientation_t orientation);
xtk_progress_bar_orientation_t xtk_progress_bar_get_orientation(xtk_progress_bar_t *pbar);

void xtk_progress_bar_pulse(xtk_progress_bar_t *pbar);

void xtk_progress_bar_set_pulse_step(xtk_progress_bar_t *pbar,
                                    double fraction);
double xtk_progress_bar_get_pulse_step(xtk_progress_bar_t *pbar);
                                                                            
void xtk_progress_bar_set_text(xtk_progress_bar_t *pbar,
                               const char *text);
const char *xtk_progress_bar_get_text(xtk_progress_bar_t *pbar);      

#endif /* _LIB_XTK_PROGRESS_BAR_H */