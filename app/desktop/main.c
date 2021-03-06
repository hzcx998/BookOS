/**
 * xtk desktop environment
 */
#include "desktop.h"

int main(int argc, char *argv[]) 
{
    if (desktop_init() < 0) {
        return -1;
    }
    desktop_main();
    desktop_exit();
    return 0;
}
