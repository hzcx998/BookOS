#include "taskbar.h"

int main(int argc, char *argv[]) 
{
    if (taskbar_init() < 0)
        return -1;
    taskbar_main();
    taskbar_exit();
}
