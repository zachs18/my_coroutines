#ifndef CO_AIO_H
#define CO_AIO_H

#ifdef __cplusplus
extern "C" {
#endif // def __cplusplus

#include <unistd.h>

ssize_t co_read(int fd, void *buf, size_t count, off_t offset);
ssize_t co_write(int fd, const void *buf, size_t count, off_t offset);
int co_fsync(int fd);
int co_fdatasync(int fd);


#ifdef __cplusplus
}
#endif // def __cplusplus

#endif // ndef CO_AIO_H
