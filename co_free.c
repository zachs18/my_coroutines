#include "my_coroutines.h"
#include <stdlib.h>

int co_free(co_handle_t handle) {
	void **execution_state_info_ptr = (void**)handle;
	if (!execution_state_info_ptr) {
		// the co_handle_t does not refer to a coroutine
		return -2;
	} else if (execution_state_info_ptr[0] != NULL) {
		// the coroutine is not finished
		return -1;
	} else {
		// the coroutine is finished, deallocate it and its stack
		char *sp = execution_state_info_ptr[1];
		sp -= (2 << 20) - 0x10;
		free(sp);
		free(execution_state_info_ptr);
		return 0;
	}
}
