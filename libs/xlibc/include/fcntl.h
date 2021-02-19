
#ifndef _XLIBC_FCNTL_H
#define _XLIBC_FCNTL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef F_SETFL
enum {
    F_SETFL = 1,
    F_GETFL,
};
#endif

/* file open 文件打开 */
#define O_RDONLY    0x01
#define O_WRONLY    0x02
#define O_RDWR      0x04
#define O_CREAT     0x08
#define O_TRUNC     0x10
#define O_APPEND    0x20
#define O_EXEC      0x80
#define O_TEXT      0x100   // 文本模式打开        
#define O_BINARY    0x200   // 二进制模式打开
#define O_NONBLOCK  0x400   // 无阻塞
#define O_NOCTTY    0x800   // 不设置为控制tty

#ifdef __cplusplus
}
#endif

#endif  /* _XLIBC_FCNTL_H */
