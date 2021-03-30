#include "xtk_entry.h"
#include "xtk_label.h"
#include "xtk_signal.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static void xtk_entry_draw_cursor(xtk_spirit_t *spirit)
{
    xtk_entry_t *entry = XTK_ENTRY(spirit);
    if (!entry->focus)
        return;
    xtk_spirit_t *attached_spirit = (xtk_spirit_t *)spirit->attached_container->spirit;
        if (!attached_spirit->surface)
            return;
    if (!xtk_entry_get_selection_bounds(entry, NULL, NULL)) {
        int start_x = spirit->x;
        int start_y = spirit->y;
        int off_x = 1, off_y = 1;
        off_x += entry->cursor_pos * 8;
        xtk_rect_t srcrect = {0, 0, entry->cursor->w, entry->cursor->h};
        xtk_rect_t dstrect = {start_x + off_x, start_y + off_y, entry->cursor->w, entry->cursor->h};
        xtk_surface_blit(entry->cursor, &srcrect, attached_spirit->surface, &dstrect);
    }
}

static void xtk_entry_draw_selection(xtk_spirit_t *spirit)
{
    xtk_entry_t *entry = XTK_ENTRY(spirit);
    if (!entry->focus)
        return;
    xtk_spirit_t *attached_spirit = (xtk_spirit_t *)spirit->attached_container->spirit;
        if (!attached_spirit->surface)
            return;
    if (xtk_entry_get_selection_bounds(entry, NULL, NULL)) {
        int chars = abs(entry->select_start_pos - entry->select_end_pos);
        if (!chars) // 没有字符就返回
            return;
        int min_pos = min(entry->select_start_pos, entry->select_end_pos);

        int w = chars * 8;
        int h = 18;

        int start_x = spirit->x;
        int start_y = spirit->y;
        int off_x = 1, off_y = 1;
        off_x += min_pos * 8;

        /* 调整surface大小 */
        if (!entry->selection) {
            entry->selection = xtk_surface_create(w, h);
            if (!entry->selection)  /* 创建失败 */
                return;
        } else {
            if (xtk_surface_resize(entry->selection, w, h) < 0)
                return;
        }
        xtk_surface_rectfill(entry->selection, 0, 0, entry->selection->w, entry->selection->h, XTK_GREEN);

        xtk_rect_t srcrect = {0, 0, entry->selection->w, entry->selection->h};
        xtk_rect_t dstrect = {start_x + off_x, start_y + off_y, entry->selection->w, entry->selection->h};
        xtk_surface_blit(entry->selection, &srcrect, attached_spirit->surface, &dstrect);
    }
}

xtk_spirit_t *xtk_entry_create(void)
{
    xtk_entry_t *entry = malloc(sizeof(xtk_entry_t));
    if (!entry)
        return NULL;
    
    entry->editable = true;
    entry->visible = true;
    entry->invisible_char = XTK_ENTRY_INVISIBLE_CHAR_DEFAULT;
    entry->focus = false;
    entry->cursor_pos = 0;
    entry->focus_color = XTK_ENTRY_FOCUS_COLOR;
    entry->unfocus_color = XTK_GRAY;
    entry->select_end_pos = entry->select_start_pos = 0;    /* no select, same */
    entry->start_selecting = 0;
    entry->selection = NULL;
    
    entry->cursor = xtk_surface_create(1, XTK_ENTRY_HEIGHT_DEFAULT - 2);
    if (!entry->cursor) {
        free(entry);
        return NULL;
    }
    /*
    entry->selection = xtk_surface_create(8, 16);
    if (!entry->selection) {
        free(entry->cursor);
        free(entry);
        return NULL;
    }*/
    xtk_surface_rectfill(entry->cursor, 0, 0, 1, entry->cursor->h, XTK_BLACK);

    xtk_spirit_t *spirit = &entry->spirit;
    xtk_spirit_init(spirit, 0, 0, XTK_ENTRY_WIDTH_DEFAULT, XTK_ENTRY_HEIGHT_DEFAULT);

    xtk_spirit_set_type(spirit, XTK_SPIRIT_TYPE_ENTRY);
    
    spirit->style.padding = 1;
    spirit->style.align = XTK_ALIGN_LEFT;
    spirit->style.background_color = XTK_WHITE;
    spirit->style.color = XTK_BLACK;
    spirit->style.border_color = entry->unfocus_color;
    
    spirit->show_middle = xtk_entry_draw_selection;
    spirit->show_bottom = xtk_entry_draw_cursor;
    
    xtk_spirit_set_text(spirit, "");    /* set empty string */

    assert(!xtk_signal_create(spirit, "activate"));

    return spirit;
}

void xtk_entry_set_max_length(xtk_entry_t *entry, 
                              int max)
{
    if (entry != NULL) {
        xtk_spirit_set_text_max_len(&entry->spirit, max);
    }
}

void xtk_entry_set_text(xtk_entry_t *entry,
                        const char *text)
{
    if (entry != NULL) {
        xtk_spirit_set_text(&entry->spirit, (char *)text);
        xtk_spirit_show(&entry->spirit);
    }
}

const char *xtk_entry_get_text(xtk_entry_t *entry)
{
    if (entry == NULL)
        return NULL;
    return entry->spirit.text;
}

void xtk_entry_set_editable(xtk_entry_t *entry,
                            bool is_editable)
{
    entry->editable = is_editable;
}

bool xtk_entry_get_editable(xtk_entry_t *entry)
{
    return entry->editable;
}

void xtk_entry_set_visibility(xtk_entry_t *entry,
                              bool visible)
{
    entry->visible = visible;
}

bool xtk_entry_get_visibility (xtk_entry_t *entry)
{
    return entry->visible;
}

void xtk_entry_set_invisible_char (xtk_entry_t *entry,
                                   char ch)
{
    entry->invisible_char = ch;
}

void xtk_entry_unset_invisible_char (xtk_entry_t *entry)
{
    entry->invisible_char = XTK_ENTRY_INVISIBLE_CHAR_DEFAULT;
}

void xtk_entry_move_cursor (xtk_entry_t *entry, xtk_entry_cursor_orientation_t orientation, int step)
{
    if (!entry)
        return;
    switch (orientation) {
    case XTK_ENTRY_CURSOR_LEFT_TO_RIGHT:
        xtk_entry_set_position(entry, entry->cursor_pos + step);
        break;
    case XTK_ENTRY_CURSOR_RIGHT_TO_LEFT:
        xtk_entry_set_position(entry, entry->cursor_pos - step);
        break;
    default:
        break;
    }
    xtk_spirit_show(&entry->spirit);
}

void xtk_entry_set_focus(xtk_entry_t *entry, bool is_focus)
{
    xtk_spirit_t *spirit = &entry->spirit;
    if (is_focus) {
        entry->focus = true;
        spirit->style.border_color = entry->focus_color;
        xtk_spirit_show(spirit);
    } else {
        if (entry->focus) {
            entry->focus = false;
            spirit->style.border_color = entry->unfocus_color;
            xtk_spirit_show(spirit);
        }
    }
}

bool xtk_entry_get_focus(xtk_entry_t *entry)
{
    return entry != NULL ? entry->focus : false;
}

/* entry种的某个x位置转换成光标位置 */
int xtk_entry_locate_position (xtk_entry_t *entry, int position)
{
    xtk_spirit_t *spirit = &entry->spirit;
    int cursor_pos = position - spirit->style.padding;
    int char_width = 8;
    cursor_pos += char_width / 2;
    cursor_pos /= char_width;
    xtk_entry_set_position(entry, cursor_pos);
    return entry->cursor_pos;
}

void xtk_entry_set_position(xtk_entry_t *entry,
                            int position)
{
    entry->cursor_pos = position;
    int lablen = xtk_label_length(&entry->spirit);
    if (entry->cursor_pos > lablen)
        entry->cursor_pos = lablen;
    if (entry->spirit.max_text_len > 0 && entry->cursor_pos > entry->spirit.max_text_len)
        entry->cursor_pos = entry->spirit.max_text_len;
    if (entry->cursor_pos < 0)
        entry->cursor_pos = 0;    
}

int xtk_entry_get_position (xtk_entry_t *entry)
{
    return entry != NULL ? entry->cursor_pos : 0;
}

void gtk_entry_insert_text(xtk_entry_t *entry,
                              const char *new_text,
                              int new_text_length)
{
    if (!entry)
        return;
    xtk_spirit_t *spirit = &entry->spirit;
    char *p = spirit->text;
    if (p) {
        /* 插入后长度超出 */
        if (spirit->max_text_len > 0 && new_text_length + xtk_label_length(spirit) > spirit->max_text_len)
            return;
        char strbuf[256] = {0};
        /* 复制光标前面的内容 */
        strncpy(strbuf, p, entry->cursor_pos);
        /* 插入新的字符 */
        strcat(strbuf, new_text);
        /* 复制光标后面的内容 */
        p += entry->cursor_pos;
        strcat(strbuf, p);
        
        xtk_entry_set_text(entry, strbuf);
        xtk_entry_move_cursor(entry, XTK_ENTRY_CURSOR_LEFT_TO_RIGHT, new_text_length);
    } else {
        xtk_entry_set_text(entry, new_text);
    }
}

/* 删除字符串区间[start_pos, end_pos)，如果end_pos为负数，那么就到末尾 */
void gtk_entry_delete_text(xtk_entry_t *entry,
                           int start_pos,
                           int end_pos)
{
    if (!entry)
        return;
    xtk_spirit_t *spirit = &entry->spirit;
    char *p = spirit->text;
    if (p) {
        char strbuf[256] = {0};
        int old_len;
        /* 光标位置小于0，就直接返回 */
        if (start_pos < 0)
            return;

        old_len = xtk_label_length(spirit);
        if (end_pos < 0)    // pos为负，end是字符串末尾
            end_pos = old_len;
            if (end_pos < 0)
                end_pos = 0;
        /* 边界修复 */
        if (end_pos > old_len)
            end_pos = old_len;
        if (start_pos > old_len)
            start_pos = old_len;       
        
        /* 保留开始位置前的字符串 */
        if (start_pos > 0)
            strncpy(strbuf, p, start_pos);
        
        /* 复制结束位置后面的内容 */
        p += end_pos;
        strcat(strbuf, p);

        xtk_entry_set_position(entry, start_pos);
        xtk_entry_set_text(entry, strbuf);
    }
}

void xtk_entry_process_key(xtk_entry_t *entry, int keycode, int modify)
{
    if (entry->editable == false)
        return;
    xtk_spirit_t *spirit = &entry->spirit;
    
    /* TODO: 如果此时有选区，那么添加字符和删除，就是针对选区进行 */
    //xtk_entry_select_region(entry, entry->cursor_pos, entry->cursor_pos);

    /* 获取字符串，并加入字符串 */    
    switch (keycode) {
    case UVIEW_KEY_UP:
        break;
    case UVIEW_KEY_DOWN:
        break;
    case UVIEW_KEY_LEFT:
        if (modify & UVIEW_KMOD_CTRL) { /* 移动到开头 */
            xtk_entry_move_cursor(entry, XTK_ENTRY_CURSOR_RIGHT_TO_LEFT, entry->cursor_pos);
        } else {
            xtk_entry_move_cursor(entry, XTK_ENTRY_CURSOR_RIGHT_TO_LEFT, 1);
        }
        break;
    case UVIEW_KEY_RIGHT:
        if (modify & UVIEW_KMOD_CTRL) { /* 移动到末尾 */
            xtk_entry_move_cursor(entry, XTK_ENTRY_CURSOR_LEFT_TO_RIGHT, xtk_label_length(spirit));
        } else {
            xtk_entry_move_cursor(entry, XTK_ENTRY_CURSOR_LEFT_TO_RIGHT, 1);
        }
        break;
    case UVIEW_KEY_ENTER:   /* 完成输入 */
        xtk_entry_set_focus(entry, false);
        xtk_signal_emit_by_name(spirit, "activate");
        break;
    case UVIEW_KEY_BACKSPACE:
        gtk_entry_delete_text(entry, entry->cursor_pos - 1, entry->cursor_pos);
        break;
    default:
        {
            char s[2] = {keycode, 0};
            gtk_entry_insert_text(entry, s, 1);
        }
        break;
    }
}

void xtk_entry_select_region (xtk_entry_t *entry,
                                 int start_pos,
                                 int end_pos)
{
    if (!entry)
        return;
    entry->select_start_pos = start_pos;
    entry->select_end_pos = end_pos;
}

bool xtk_entry_get_selection_bounds (xtk_entry_t *entry,
                                     int *start_pos,
                                     int *end_pos)
{
    if (!entry)
        return false;
    if (entry->select_start_pos == entry->select_end_pos)
        return false;
    if (start_pos)
        *start_pos = entry->select_start_pos;
    if (end_pos)
        *end_pos = entry->select_end_pos;
    return true;
}