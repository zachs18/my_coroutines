#ifndef MY_COROUTINES_H
#define MY_COROUTINES_H

#ifdef __cplusplus
extern "C" {
#endif // def __cplusplus

#include <stdbool.h>
#include <stdint.h>

extern void *current_execution_state_info;

typedef void *(*co_handle_t)[4]; // struct execution_state_info *

co_handle_t co_allocate(intptr_t (*func)(intptr_t));
bool co_finished(co_handle_t);
intptr_t co_await(co_handle_t, intptr_t);
intptr_t co_yield(intptr_t);
int co_free(co_handle_t);

#ifdef __cplusplus
}
#endif // def __cplusplus

#endif // ndef MY_COROUTINES_H
