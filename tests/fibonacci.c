#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "my_coroutines.h"

intptr_t fibonacci(intptr_t arg) {
	// Keeps yielding fibonacci numbers until it is passed a true value
	uintptr_t a = 0;
	uintptr_t b = 1;
	while(arg) {
		uintptr_t c = a + b;
		a = b;
		b = c;
		arg = co_yield(a);
	}
	return -1; // last await
}

int main(void) {
	co_handle_t fib_handle = co_allocate(fibonacci);
	for (intptr_t i = co_await(fib_handle, true); i > 0; i = co_await(fib_handle, true)) { // keep getting values until one is biggern than INTPTR_T_MAX
		printf("%ld\n", i);
	}
	co_await(fib_handle, false);
	co_free(fib_handle);
}
/*
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
*/
