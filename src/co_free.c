#include "my_coroutines.h"
#include <stdlib.h>

int co_free(co_handle_t execution_state_info_ptr) {
	if (!execution_state_info_ptr) {
		// the co_handle_t does not refer to a coroutine
		return -2;
	} else if (execution_state_info_ptr->ip != NULL) {
		// the coroutine is not finished
		return -1;
	} else {
		// the coroutine is finished, deallocate it and it's stack
		char *sp = execution_state_info_ptr->sp;
		sp -= (2 << 20) - 0x10;
		free(sp);
		free(execution_state_info_ptr);
		return 0;
	}
}
