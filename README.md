# my_coroutines
I wanted to make my own coroutine library (for amd64 Linux), so I did.

Yeah, it isn't the best but it was fun to make.

Also, my usage of `await` does not seem consistent with how it is usually used, probably because I don't really understand coroutines 🤷‍. These "coroutines" are more like Python's iterators.

Note that this library only supports using coroutines in a single thread at a time, since IDK how to do thread_local in assembly.

### Usage:

`co_handle_t co_allocate(intptr_t (*func)(intptr_t));`: allocates the necessary resources for a coroutine that will run `func` and returns an opaque pointer handle type usable to await, query finished-ness, and free resources after completion. If the returned `co_handle_t` compares equal to `0`/`NULL`/`nullptr` , then the argument was invalid or the resource allocation failed.
The function passed to `co_allocate` will not start executing until the coroutine is first `co_await`ed.

`intptr_t co_await(co_handle_t handle, intptr_t arg)`: If the coroutine has not started execution, it will start execution with `arg` as its argument. If it has started execution (and has yielded) then `co_yield` will return `arg` and execution will continue from where the function yielded. `co_await` will return either when the coroutine returns (in which case the return value is the return value of the function) or calls `co_yield` (in which case the return value is the arg passed to `co_yield`). This function does not (currently) deal with invalid arguments.

`intptr_t co_yield(intptr_t arg)`: This function should only be called in functions that will be used as coroutines. Calling this function outside of a coroutine is **bad^(TM)**. The caller's `co_await` will return `arg` from where it left off. This coroutine will continue from where it left off when it is next `co_await`ed. This function does not (currently) deal with invalid arguments or call conditions.

`bool co_finished(co_handle_t handle)`: This function can be used to see if a coroutine has completed (returns `true`) or if it is still waiting to be `co_await`ed (returns `false`)


`int co_free(co_handle_t handle)`: This function will release the resources that a completed coroutine was using. Return value of `0` indicates success, `-1` indicates that the coroutine has not completed, `-2` indicated that the `handle` does not refer to a valid coroutine.
