#include "my_coroutines.h"
#include "stdlib.h" // malloc

extern void co_wrapper(void);

co_handle_t co_allocate(intptr_t (*func)(intptr_t)) {
	char *stack = malloc(2 << 20); // 2 MiB, 16-byte aligned
	if (!stack) return NULL;

	co_handle_t handle = malloc(sizeof(*handle));
	if (!handle) {
		free(stack);
		return NULL;
	}

	intptr_t *bp = (void*)(stack + (2 << 20));
	intptr_t *sp = bp - 2;
	sp[0] = (intptr_t)(void*)func;

	handle->ip = &co_wrapper;
	handle->sp = sp;
	handle->bp = bp;
	handle->next = NULL;

	return handle;
}
