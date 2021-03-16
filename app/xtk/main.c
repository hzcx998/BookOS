#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <xtk.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>


#include <cairo.h>

#include <math.h>

#define WIN_W   640
#define WIN_H   480


int click_count = 0;

static bool click_btn_event(xtk_spirit_t *spirit, void *label) 
{
    click_count++;
    char str[32] = {0};
    sprintf(str, "count: %d", click_count);
    xtk_label_set_text(label, str);
    return true;
}

static bool clear_btn_event(xtk_spirit_t *spirit, void *label) 
{
    click_count = 0;
    char str[32] = {0};
    sprintf(str, "count: %d", click_count);
    xtk_label_set_text(label, str);
    return true;
}
#define FONT_SIZE   32

FT_Library	library;
FT_Face		face;
void freetype_test(xtk_window_t *win)
{
	FT_Error	error;

	// 1. 初始化freetype2库
	error = FT_Init_FreeType(&library);
    if (error) {
        printf("FT_Init_FreeType error!\n");
        return;
    }
	// 2. 创建一个face
	error = FT_New_Face(library, "/system/fonts/HGXH_CNKI.TTF", 0, &face);
    if (error) {
        printf("FT_New_Face error!\n");
        return;
    }
	
	// 3. 设置字体尺寸
	#if 0
    error = FT_Set_Char_Size(face, 16*64, 16*64, 16, 16);
    if (error) {
        printf("FT_Set_Char_Size error!\n");
        return;
    }
    #endif

    xtk_window_invalid_window(win);
    xtk_window_paint(win);
}

void draw_char(xtk_surface_t *surface, int x, int y, unsigned int wch, int size)
{
    FT_GlyphSlot slot;
    FT_UInt		charIdx;
	// 4. 获取字符图像索引
	charIdx = FT_Get_Char_Index(face, wch);
    slot = face->glyph;
	// 5. 加载字符图像
	FT_Load_Glyph(face, charIdx, FT_LOAD_DEFAULT);
	if (slot->format == FT_GLYPH_FORMAT_OUTLINE) {
		FT_Outline_Embolden(&(slot->outline), 16);	// 加粗轮廓线
	}
 
	// 6. 获取字符位图
	if (slot->format != FT_GLYPH_FORMAT_BITMAP) {
		FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
	}
    
	// 7. 拷贝字符位图到用户的buffer中(用户私人函数)
    int i, j;
    for (i = 0; i < slot->bitmap.rows; ++i) {
        for (j = 0; j < slot->bitmap.width; ++j) {
            //  if it has gray>0 we set show it as 1, o otherwise
            unsigned char gray = slot->bitmap.buffer[i * slot->bitmap.width + j]; 
            if (gray)
                xtk_surface_putpixel(surface, x + j + slot->bitmap_left, y + i + (size - slot->bitmap_top),
                    XTK_RGB(255 - gray, 255 - gray, 255 - gray));
        }
    }
}

void draw_string(xtk_window_t *window, int x, int y, char *wstr, int size)
{
    FT_Error	error;
    error = FT_Set_Pixel_Sizes(face, 0, size);
    if (error) {
        printf("FT_Set_Char_Size error!\n");
        return;
    }

    xtk_surface_t *surface = xtk_window_get_surface(window);
    int start_x = x, start_y = y;
    int len = 0;
    while (*wstr) {
        draw_char(surface, start_x, start_y, *wstr, size);
        wstr++;
        len++;
        start_x += size / 2;
    }
    int w = len * size / 2;
    xtk_window_update(window, x, y, w, size + 12);
}


void draw_string_zh(xtk_window_t *window, int x, int y, char *wstr, int size)
{
    FT_Error	error;
    error = FT_Set_Pixel_Sizes(face, 0, size);
    if (error) {
        printf("FT_Set_Char_Size error!\n");
        return;
    }
    
    xtk_surface_t *surface = xtk_window_get_surface(window);
    int start_x = x, start_y = y;
    int len = 0;
    unsigned int wch = 0;
    while (*wstr) {
        wch = 0;
        int l;
        for (l = 0; l < 3; l++) {
            wch |= (*wstr & 0xff) << (l * 8);
            wstr++;
        }
        draw_char(surface, start_x, start_y, wch, size);
        len++;
        start_x += size;
    }
    int w = len * size;
    xtk_window_update(window, x, y, w, size + 10);
}


int file_test3()
{
    FILE *fp = NULL;
    fp = fopen("/res/test2.txt", "wb");
    if (!fp) {
        perror("fopen failed because: ");
        
        printf("fopen failed\n");
        return -1;
    }
    fwrite("hello", 5, 1, fp);
    fclose(fp);
    printf("test end\n");
    return 0;
}

void cairo_test(xtk_spirit_t *spirit)
{
    cairo_surface_t *surface;  
    cairo_t *cr;  
  
    int width = WIN_W;  
    int height = WIN_H;  
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);  
    cr = cairo_create (surface);  
    
    #if 0
    /* Drawing code goes here */  
    cairo_set_line_width (cr, 10);  
    cairo_set_source_rgb (cr, 0, 0, 0);  
    cairo_rectangle (cr, width/4, height/4, width/2, height/2);  
    cairo_stroke (cr);  
    #else
    #if 0
    cairo_set_line_width (cr, 6);

    cairo_rectangle (cr, 12, 12, 232, 70);
    cairo_new_sub_path (cr); cairo_arc (cr, 64, 64, 40, 0, 2*M_PI);
    cairo_new_sub_path (cr); cairo_arc_negative (cr, 192, 64, 40, 0, -2*M_PI);

    cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
    cairo_set_source_rgb (cr, 0, 0.7, 0); cairo_fill_preserve (cr);
    cairo_set_source_rgb (cr, 0, 0, 0); cairo_stroke (cr);

    cairo_translate (cr, 0, 128);
    cairo_rectangle (cr, 12, 12, 232, 70);
    cairo_new_sub_path (cr); cairo_arc (cr, 64, 64, 40, 0, 2*M_PI);
    cairo_new_sub_path (cr); cairo_arc_negative (cr, 192, 64, 40, 0, -2*M_PI);

    cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
    cairo_set_source_rgb (cr, 0, 0, 0.9); cairo_fill_preserve (cr);
    cairo_set_source_rgb (cr, 0, 0, 0); cairo_stroke (cr);
    #endif
    #endif
    #if 0
    cairo_pattern_t *pat;

    pat = cairo_pattern_create_linear (0.0, 0.0,  0.0, 256.0);
    cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
    cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
    cairo_rectangle (cr, 0, 0, 256, 256);
    cairo_set_source (cr, pat);
    cairo_fill (cr);
    cairo_pattern_destroy (pat);

    pat = cairo_pattern_create_radial (115.2, 102.4, 25.6,
                                    102.4,  102.4, 128.0);
    cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
    cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
    cairo_set_source (cr, pat);
    cairo_arc (cr, 128.0, 128.0, 76.8, 0, 2 * M_PI);
    cairo_fill (cr);
    cairo_pattern_destroy (pat);
    #endif
    cairo_text_extents_t extents;

    const char *utf8 = "cairo";
    double x,y;

    cairo_select_font_face (cr, "Sans",
        CAIRO_FONT_SLANT_NORMAL,
        CAIRO_FONT_WEIGHT_NORMAL);

    cairo_set_font_size (cr, 52.0);
    cairo_text_extents (cr, utf8, &extents);
    x = 128.0-(extents.width/2 + extents.x_bearing);
    y = 128.0-(extents.height/2 + extents.y_bearing);

    cairo_move_to (cr, x, y);
    cairo_show_text (cr, utf8);

    /* draw helping lines */
    cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
    cairo_set_line_width (cr, 6.0);
    cairo_arc (cr, x, y, 10.0, 0, 2*M_PI);
    cairo_fill (cr);
    cairo_move_to (cr, 128.0, 0);
    cairo_rel_line_to (cr, 0, 256);
    cairo_move_to (cr, 0, 128.0);
    cairo_rel_line_to (cr, 256, 0);
    cairo_stroke (cr);

    /* Write output and clean up */  
    cairo_surface_write_to_png (surface, "/res/image.png");  
    cairo_destroy (cr);  
    cairo_surface_destroy (surface);
    
    xtk_window_invalid_window(XTK_WINDOW(spirit));
    xtk_window_paint(XTK_WINDOW(spirit));
}



void paint_callback(xtk_spirit_t *spirit, xtk_rect_t *rect)
{
    #if 0
    FT_GlyphSlot slot;
    FT_UInt		charIdx;
    wchar_t		wch = 'a';
	char*		buffer;		// 用户申请的显示区域空间
	int			start_x, start_y;	// 字符图像开始装入的位置
 
	// 4. 获取字符图像索引
	charIdx = FT_Get_Char_Index(face, wch);
	
    slot = face->glyph;
	// 5. 加载字符图像
	FT_Load_Glyph(face, charIdx, FT_LOAD_DEFAULT);
	if (slot->format == FT_GLYPH_FORMAT_OUTLINE)
	{
		FT_Outline_Embolden(&(slot->outline), 1);	// 加粗轮廓线
	}
 
	// 6. 获取字符位图
	if (slot->format != FT_GLYPH_FORMAT_BITMAP)
	{
		FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
	}
    
	// 7. 拷贝字符位图到用户的buffer中(用户私人函数)
	// 注意左边的计算方法
	/*ft2CopyBitmapToBuf(buffer, start_x+face->glyph->bitmap_left,
		start_y+face->size->metrics.ascender/64-face->glyph->bitmap_top,
		face->glyph->bitmap);*/
    xtk_surface_t *surf = xtk_window_get_surface(XTK_WINDOW(spirit));
    
    start_x = 0;
    start_y = 0;
    
    int i, j;
    for (i = 0; i < slot->bitmap.rows; ++i) {
        for (j = 0; j < slot->bitmap.width; ++j) {
            //  if it has gray>0 we set show it as 1, o otherwise
            unsigned char gray = slot->bitmap.buffer[i * slot->bitmap.width + j]; 
            // printf( " %d " , gray ? 1 : 0 );
            if (gray)
                xtk_surface_putpixel(surf, start_x + j + slot->bitmap_left, start_y + i + (16 - slot->bitmap_top),
                    XTK_RGB(gray, gray, gray));
        }
        // printf( " \n " );
    }
	start_x += face->glyph->advance.x/64;
    xtk_window_update(XTK_WINDOW(spirit), rect->x, rect->y, rect->w, rect->h);
    #else
    #if 0
    draw_string(XTK_WINDOW(spirit), 0, 0, "hello, world!", 16);
    draw_string(XTK_WINDOW(spirit), 0, 16, "hello, world!", 32);
    draw_string(XTK_WINDOW(spirit), 0, 16 + 32, "hello, world!", 48);

    char str[] = {0x11, 0x62, 0x00, 0x31, 0x72, 0x00, 0x60, 0x4f, 0x00, 0x2d, 0x4e, 0x00, 0xfd, 0x56, 0x00, 0x01, 0xff, 0x00}; 
    draw_string_zh(XTK_WINDOW(spirit), 0, 16 + 32 + 48, str, 48);
    #endif
    #endif

    xtk_image_t *img = xtk_image_load("/res/image.png");
    assert(img);
    xtk_surface_t img_surf;
    xtk_surface_init(&img_surf, img->w, img->h, (uint32_t *) img->buf);
    xtk_surface_blit(&img_surf, NULL, xtk_window_get_surface(XTK_WINDOW(spirit)), NULL);
    xtk_window_flip(XTK_WINDOW(spirit));
}

bool motion(xtk_spirit_t *spirit, xtk_event_t *event, void *arg)
{
    printf("motion %d, %d\n", event->motion.x, event->motion.y);
    return true;
}

bool enter(xtk_spirit_t *spirit, void *arg)
{
    printf("enter\n");
    return true;
}

bool leave(xtk_spirit_t *spirit, void *arg)
{
    printf("leave\n");
    return true;
}

int main(int argc, char *argv[]) 
{
    xtk_init(&argc, &argv);
    //xtk_spirit_t *win = xtk_window_create_simple("xtk-test", 100, 100, 320, 240);
    xtk_spirit_t *win = xtk_window_create(XTK_WINDOW_TOPLEVEL);
    assert(win);
    xtk_window_set_default_size(XTK_WINDOW(win), WIN_W, WIN_H);
    xtk_signal_connect(win, "motion_notify", XTK_CALLBACK(motion), NULL);
    xtk_signal_connect(win, "enter_notify", XTK_CALLBACK(enter), NULL);
    xtk_signal_connect(win, "leave_notify", XTK_CALLBACK(leave), NULL);

    xtk_spirit_t *l0 = xtk_label_create("count: 0");
    assert(l0);
    assert(!xtk_container_add(XTK_CONTAINER(win), l0));
    xtk_spirit_set_pos(l0, 160, 20);
    xtk_spirit_t *btn0 = xtk_button_create_with_label("click");
    assert(btn0);
    xtk_signal_connect(btn0, "button_press", click_btn_event, l0);
    assert(!xtk_container_add(XTK_CONTAINER(win), btn0));
    xtk_spirit_set_pos(btn0, 160 - 50, 100);
    xtk_spirit_t *btn1 = xtk_button_create_with_label("clear");
    assert(btn1);
    xtk_signal_connect(btn1, "button_press", clear_btn_event, l0);
    assert(!xtk_container_add(XTK_CONTAINER(win), btn1));
    xtk_spirit_set_pos(btn1, 160 + 50, 100);
    // xtk_window_paint_callback(XTK_WINDOW(win), paint_callback);
    xtk_spirit_show_all(win);
    
    // cairo_test(XTK_WINDOW(win));
    /* 图标图形测试 */
    printf("spirit image test\n");
    xtk_spirit_t *icon = xtk_spirit_create(0, 0, 48, 48);
    assert(icon != NULL);
    
    if (xtk_spirit_set_image2(icon, "/usr/local/infones/infones.png", -1, -1) < 0)
        printf("set image failed!\n");
    xtk_spirit_set_text(icon, "123");
    assert(!xtk_container_add(XTK_CONTAINER(win), icon));
    xtk_spirit_set_pos(icon, 10, 10);
    xtk_spirit_show(icon);

    xtk_main();
    return 0;
}
