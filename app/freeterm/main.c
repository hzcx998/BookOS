#include <stdio.h>
#include <stdlib.h>
#include <ft_console.h>
#include <ft_cmd.h>
#include <ft_window.h>
#include <pty.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ft_pty.h>

#define SHELL_NAME "/bin/sh"

static void exit_freeterm()
{
    exit_cmd_man();
    exit_console();
    ft_pty_exit(&ft_pty);
}

int main(int argc, char *argv[]) 
{
    putenv("TERM=freeterm");
    setbuf(stdout,NULL);
    if (init_console() < 0) {
        printf("freeterm: init console failed!\n");
        return -1;
    }
    
    if (init_cmd_man() < 0) {
        printf("freeterm: init cmd failed!\n");
        exit_console();
        return -1;
    }

    if (ft_pty_init(&ft_pty) < 0) {
        printf("freeterm: init pty failed!\n");
        exit_cmd_man();
        exit_console();
        return -1;
    }
    
    int pid = fork();
    if (pid < 0) {
        printf("freeterm: do fork failed!\n");
        return -1;
    } else if (!pid) { // 子进程
        ft_pty_launch(&ft_pty, SHELL_NAME);
    } else { // 父进程
        ft_pty_setup(pid);
    }
    window_loop();
    exit_freeterm();
    return 0;
}
