#include <stdbool.h>
#include <stdint.h>

typedef void *(*co_handle_t)[4]; // struct execution_state_info *

co_handle_t co_allocate(intptr_t (*func)(intptr_t));
bool co_finished(co_handle_t);
intptr_t co_await(co_handle_t, intptr_t);
intptr_t co_yield(intptr_t);
int co_free(co_handle_t);
