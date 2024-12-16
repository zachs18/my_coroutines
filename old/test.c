#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "my_coroutines.h"

void *test_func(void *arg) {
	return (void*)(4 + (uintptr_t)arg);
}

int main(void) {
	void **test = co_allocate(test_func);
	printf("handle: %p\n\n", test);

	printf("ip: %p\n", test[0]);
	printf("sp: %p\n", test[1]);
	printf("bp: %p\n", test[2]);
	printf("next: %p\n\n", test[3]);

	void **stack = test[1];
	printf("arg (on stack): %p\n\n", stack[-1]);


	printf("Finished? : %s\n", co_finished(test) ? "yes" : "no");

	free(test);
}

void *test(void *(*func)(void*), void *arg) {
	void *stack = malloc(2 << 20);
	void **state_info = malloc(4 * sizeof(void*));
}
