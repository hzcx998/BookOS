/*
TODO: 将资源配置成文件，然后读取文件，根据文件配置进行处理
*/

#include <stdio.h>
#include <xtk.h>
#include <assert.h>
#include <unistd.h>
#include <sys/sys.h>

#define MOUSE_CURSOR_DIR            "/system/cursors"
#define BACKGROUND_IMAGE_NAME       "/system/background/login.jpg"
#define PRGRAMS_DIR                 "/system/programs/"
#define ICON_NAME                   "/system/icons/login.png"

#define WINLOGIN_BLOCK_W    320
#define WINLOGIN_BLOCK_H    240

#define WINLOGIN_ICON_W    64
#define WINLOGIN_ICON_H    64

static int winlogin_init();
static void winlogin_setup();
static void winlogin_widget();

xtk_spirit_t *winlogin_win;
xtk_surface_t *winlogin_surf;
xtk_surface_t *winlogin_block;
int window_width;
int window_height;

xtk_spirit_t *login_state;

xtk_spirit_t *acct_entry;
xtk_spirit_t *pwd_entry;

int main(int argc, char *argv[]) 
{
    if (winlogin_init() < 0) {
        printf("winlogin: init error!\n");
        return -1;
    }
    winlogin_setup();
    xtk_main();
    return 0;
}

void winlogin_launch(const char *pathname, char *arg)
{
    char *argv[3] = {(char *)pathname, arg, NULL};
    if (!fork()) {
        /* 如果是绝对路径，那么就 */
        char *p = (char *)pathname;
        if (*p == '/' && *(p + 1)) {
            char *p = strrchr(pathname, '/');
            if (p) {
                *p = '\0';
                chdir((const char *) pathname);
                *p = '/';
            }
        }
        exit(execv((const char *) pathname, argv));
    }
}


const char *root_name = "root";
const char *root_pwd = "1234";

bool login_btn_event(xtk_spirit_t *spirit, void *arg)
{
    /* 获取账号和密码，检测是否正确 */
    const char *name = xtk_entry_get_text(XTK_ENTRY(acct_entry));
    const char *password = xtk_entry_get_text(XTK_ENTRY(pwd_entry));
    
    if (strlen(name) < 1)
        return true;
    
    if (!login(name, password)) {
        xtk_label_set_text(login_state, "Login success!");

        /* 成功后就不能再点击了 */
        xtk_button_set_disable(XTK_BUTTON(spirit));

        /* 隐藏登录窗口 */
        xtk_spirit_hide(winlogin_win);

        /* 启动桌面程序 */
        winlogin_launch(PRGRAMS_DIR "desktop", NULL);
    } else {
        xtk_label_set_text(login_state, "Login failed!");
    }
    return true;
}

static int winlogin_init()
{
    winlogin_win = NULL;
    if (xtk_init(NULL, NULL) < 0) {
        printf("winlogin: xtk_init error!\n");
        return -1;
    }
    xtk_spirit_t *spirit = xtk_window_create(XTK_WINDOW_POPUP);
    if (!spirit) {
        printf("winlogin: xtk create winlogin window error!\n");
        return -1;
    }
    xtk_window_set_fixed(XTK_WINDOW(spirit), true);
    assert(xtk_window_resize_to_screen(XTK_WINDOW(spirit)) == 0);
    xtk_window_set_resizable(XTK_WINDOW(spirit), false);
    xtk_window_get_screen(XTK_WINDOW(spirit), &window_width, &window_height);
    winlogin_surf = xtk_window_get_surface(XTK_WINDOW(spirit));
    xtk_spirit_show(spirit);
    winlogin_win = spirit;
    return 0;
}

int winlogin_load_background(const char *pathname)
{
    // 加载壁纸
    xtk_image_t *img = xtk_image_load(BACKGROUND_IMAGE_NAME);
    if (!img)
        return -1;
    
    xtk_surface_t *surface = xtk_window_get_surface(XTK_WINDOW(winlogin_win));
    assert(surface);
    xtk_surface_t img_surface;
    xtk_surface_init(&img_surface, img->w, img->h, (uint32_t *) img->buf);
    xtk_surface_blit_scaled(&img_surface, NULL, surface, NULL);
    xtk_window_flip(XTK_WINDOW(winlogin_win));

    xtk_image_destroy(img);
    return 0;
}

static void winlogin_setup()
{
    // 加载鼠标光标
    xtk_window_load_mouse_cursors(XTK_WINDOW(winlogin_win), MOUSE_CURSOR_DIR);
    // 加载背景图
    winlogin_load_background(BACKGROUND_IMAGE_NAME);
    // 加载控件
    winlogin_widget();
}

int winlogin_load_icon(const char *pathname, int x, int y)
{
    xtk_image_t *img = xtk_image_load2((char *) pathname, WINLOGIN_ICON_W, WINLOGIN_ICON_H);
    if (!img)
        return -1;
    xtk_surface_t img_surface;
    xtk_surface_init(&img_surface, img->w, img->h, (uint32_t *) img->buf);
    xtk_rect_t dstrect = {x, y, img->w, img->h};
    xtk_surface_blit(&img_surface, NULL, winlogin_surf, &dstrect);
    xtk_image_destroy(img);
    return 0;
}

static void winlogin_widget()
{
    winlogin_block = xtk_surface_create(WINLOGIN_BLOCK_W, WINLOGIN_BLOCK_H);
    assert(winlogin_block);
    xtk_surface_rectfill(winlogin_block, 0, 0, winlogin_block->w, winlogin_block->h, 
        XTK_RGB(240, 240, 240));
    xtk_surface_rect(winlogin_block, 0, 0, winlogin_block->w, winlogin_block->h, 
        XTK_RGB(220, 220, 220));
    
    /* 背景色设置为块的颜色 */
    winlogin_win->style.background_color = XTK_RGB(240, 240, 240);

    /* 输入框 */
    acct_entry = xtk_entry_create();
    assert(acct_entry);
    assert(!xtk_container_add(XTK_CONTAINER(winlogin_win), acct_entry));
    xtk_spirit_reset_size(acct_entry, 180, acct_entry->height);

    pwd_entry = xtk_entry_create();
    assert(pwd_entry);
    assert(!xtk_container_add(XTK_CONTAINER(winlogin_win), pwd_entry));
    xtk_spirit_reset_size(pwd_entry, 180, pwd_entry->height);
    xtk_entry_set_visibility(pwd_entry, false);

    /* 输入框名字 */
    xtk_spirit_t *acct_label = xtk_label_create(" account:");
    assert(acct_label);
    assert(!xtk_container_add(XTK_CONTAINER(winlogin_win), acct_label));
    acct_label->style.background_color = XTK_NONE_COLOR;

    xtk_spirit_t *pwd_label = xtk_label_create("password:");
    assert(pwd_label);
    assert(!xtk_container_add(XTK_CONTAINER(winlogin_win), pwd_label));
    pwd_label->style.background_color = XTK_NONE_COLOR;
    
    /* 消息提示 */
    login_state = xtk_label_create("");
    assert(login_state);
    assert(!xtk_container_add(XTK_CONTAINER(winlogin_win), login_state));
    login_state->style.background_color = XTK_NONE_COLOR;

    /* 登录按钮 */
    xtk_spirit_t *login_btn = xtk_button_create_with_label("Log In");
    assert(login_btn);
    assert(!xtk_container_add(XTK_CONTAINER(winlogin_win), login_btn));
    xtk_spirit_reset_size(login_btn, 80, login_btn->height);
    xtk_signal_connect(login_btn, "button_press", login_btn_event, NULL);

    /* 定位 */
    int center_x = window_width / 2;
    int center_y = window_height / 2;
    
    int block_x = center_x - winlogin_block->w / 2;
    int block_y = center_y - winlogin_block->h / 2 + 50;

    int label_x = block_x + 30;
    int label_y = block_y + 110;

    int entry_x = label_x + 80;
    int entry_y = label_y;

    int btn_x = entry_x + 100;
    int btn_y = entry_y + 30 + 50;
    
    int login_state_x = label_x;
    int login_state_y = btn_y;
    
    xtk_spirit_set_pos(acct_label, label_x, label_y);
    xtk_spirit_set_pos(pwd_label, label_x, label_y + 30);
    xtk_spirit_set_pos(acct_entry, entry_x, entry_y);
    xtk_spirit_set_pos(pwd_entry, entry_x, entry_y + 30);
    xtk_spirit_set_pos(login_btn, btn_x, btn_y);
    xtk_spirit_set_pos(login_state, login_state_x, login_state_y);
    
    /* 显示 */
    xtk_rect_t dstrect = {block_x, block_y, winlogin_block->w, winlogin_block->h};
    xtk_surface_blit(winlogin_block, NULL, winlogin_surf, &dstrect);

    winlogin_load_icon(ICON_NAME, block_x + WINLOGIN_BLOCK_W / 2 - WINLOGIN_ICON_W / 2, block_y + 20);
    xtk_window_flip(XTK_WINDOW(winlogin_win));

    xtk_spirit_show(acct_label);
    xtk_spirit_show(pwd_label);
    xtk_spirit_show(acct_entry);
    xtk_spirit_show(pwd_entry);
    xtk_spirit_show(login_btn);
    xtk_spirit_show(login_state);

}
