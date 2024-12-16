#include "my_coroutines.h"

bool co_finished(co_handle_t handle) {
	return handle->ip == 0;
}
