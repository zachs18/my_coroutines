#include "my_coroutines.h"
#include "co_aio.h"
#include <aio.h>
#include <errno.h>
#include <string.h> // memset

ssize_t co_read(int fd, void *buf, size_t count, off_t offset) {
	struct aiocb aiocb;
	memset(&aiocb, 0, sizeof(aiocb));
	aiocb.aio_fildes = fd;
	aiocb.aio_buf = buf;
	aiocb.aio_nbytes = count;
	aiocb.aio_offset = offset;
	int result = aio_read(&aiocb);
	if (result == 0) {
		while (true) {
			co_yield(0);
			result = aio_error(&aiocb);
			if (result == EINPROGRESS) { // pass
			} else {
				errno = result; // 0 for success, ECANCELED for canceled, other for other
				return aio_return(&aiocb);
			}
		}
	} else { // error
		// aio_read already set errno
		return -1;
	}
}

ssize_t co_write(int fd, const void *buf, size_t count, off_t offset) {
	struct aiocb aiocb;
	memset(&aiocb, 0, sizeof(aiocb));
	aiocb.aio_fildes = fd;
	aiocb.aio_buf = buf;
	aiocb.aio_nbytes = count;
	aiocb.aio_offset = offset;
	int result = aio_write(&aiocb);
	if (result == 0) {
		while (true) {
			co_yield(0);
			result = aio_error(&aiocb);
			if (result == EINPROGRESS) { // pass
			} else {
				errno = result; // 0 for success, ECANCELED for canceled, other for other
				return aio_return(&aiocb);
			}
		}
	} else { // error
		// aio_write already set errno
		return -1;
	}
}
