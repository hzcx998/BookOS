#ifndef _LIB_XTK_ENTRY_H
#define _LIB_XTK_ENTRY_H

#include <stddef.h>
#include <stdbool.h>

#include "xtk_spirit.h"
#include "xtk_color.h"

typedef enum {
    XTK_ENTRY_CURSOR_LEFT_TO_RIGHT,  //从左向右
    XTK_ENTRY_CURSOR_RIGHT_TO_LEFT,  //从右向左
    XTK_ENTRY_CURSOR_BOTTOM_TO_TOP,  //从下向上
    XTK_ENTRY_CURSOR_TOP_TO_BOTTOM,  //从上向下
} xtk_entry_cursor_orientation_t;

typedef struct {
    xtk_spirit_t spirit;
    bool editable;
    bool visible;
    char invisible_char;
    bool focus; /* 聚焦到当前控件 */
    
    xtk_surface_t *cursor;  /* 输入光标 */
    xtk_surface_t *selection;  /* 选择区域 */

    int cursor_pos;    /* 光标位置 */
    xtk_color_t focus_color;
    xtk_color_t unfocus_color;
    int select_start_pos;       /* 选区起始位置 */
    int select_end_pos;         /* 选区结束位置 */
    int start_selecting;        /* 处于选择选区状态 */
} xtk_entry_t;

#define XTK_ENTRY(spirit)  ((xtk_entry_t *)(spirit))

#define XTK_ENTRY_WIDTH_DEFAULT 100
#define XTK_ENTRY_HEIGHT_DEFAULT 20

#define XTK_ENTRY_FOCUS_COLOR XTK_RGB(225, 225, 225)

#define XTK_ENTRY_INVISIBLE_CHAR_DEFAULT    '*'

xtk_spirit_t *xtk_entry_create(void);

void xtk_entry_set_max_length(xtk_entry_t *entry, 
                              int max);

void xtk_entry_set_text(xtk_entry_t *entry,
                        const char *text);

const char *xtk_entry_get_text(xtk_entry_t *entry);

void xtk_entry_set_editable(xtk_entry_t *entry,
                            bool is_editable);
bool xtk_entry_get_editable(xtk_entry_t *entry);

void xtk_entry_set_visibility(xtk_entry_t *entry,
                              bool visible);
bool xtk_entry_get_visibility (xtk_entry_t *entry);

void xtk_entry_set_invisible_char(xtk_entry_t *entry,
                                   char ch);
void xtk_entry_unset_invisible_char(xtk_entry_t *entry);

void xtk_entry_set_focus(xtk_entry_t *entry, bool is_focus);
bool xtk_entry_get_focus(xtk_entry_t *entry);

void xtk_entry_set_position(xtk_entry_t *entry,
                            int position);
int xtk_entry_get_position (xtk_entry_t *entry);

void gtk_entry_insert_text(xtk_entry_t *entry,
                              const char *new_text,
                              int new_text_length);
void gtk_entry_delete_text(xtk_entry_t *entry,
                           int start_pos,
                           int end_pos);

void xtk_entry_select_region (xtk_entry_t *entry,
                                 int start_pos,
                                 int end_pos);

bool xtk_entry_get_selection_bounds (xtk_entry_t *entry,
                                     int *start_pos,
                                     int *end_pos);

int xtk_entry_locate_position(xtk_entry_t *entry, int position);
void xtk_entry_process_key(xtk_entry_t *entry, int keycode, int modify);
void xtk_entry_move_cursor (xtk_entry_t *entry, xtk_entry_cursor_orientation_t orientation, int step);

#endif /* _LIB_XTK_ENTRY_H */