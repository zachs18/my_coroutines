#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "my_coroutines.h"
#include "co_aio.h"


struct parameters {
	int fd;
	void *buf;
	size_t count;
};

intptr_t read_loop(intptr_t buf_ptr_) {
	rdwr_buf const *buf_ptr = reinterpret_cast<rdwr_buf const*>(buf_ptr_);
	off_t offset = 0;
	while(buf_ptr->count) {
		auto result = co_read(buf_ptr->fd, buf_ptr->buf, buf_ptr->count, offset);
		offset += result;
		count -= result;
		co_yield(0);
	}
	return -1; // last await
}

int main(void) {
	co_handle_t rd_handle = co_allocate(read_loop);
	co_handle_t wr_handle = co_allocate(write_loop);
	for (intptr_t i = co_await(fib_handle, true); i > 0; i = co_await(fib_handle, true)) { // keep getting values until one is biggern than INTPTR_T_MAX
		printf("%ld\n", i);
	}
	co_await(fib_handle, false);
	co_free(fib_handle);
}
