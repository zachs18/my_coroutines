/*
not a C function, only used in asm as the entry/exit point for coroutines
*/

# idea:
# this function is given as the ip of newly allocated coroutines
# when a coroutine is allocated, its stack contains (at least) 16 bytes
# a pointer to the function to be called located at the top (%rsp)
#
# When a newly allocated coroutine is co_awaited, this function will start
#  (with %rsp and %rbp already set and arg in %rsi from co_await)
# When a coroutine completes, this function will finish (popping state info and jumping back to the caller of co_await)
	.globl co_wrapper
	.type co_wrapper, @function
co_wrapper: # called (indirectly) on first co_await; same state as .co_yield_resume
	endbr64
	mov	%rsi,%rdi    # arg
	mov	(%rsp),%rsi  # func
	call	*%rsi

	mov	%rax,%rdi    # co_await expects the value in %rdi

	# this part is similar to the beginning of co_yield
	# to pop our execution state and set its ip to NULL to indicate we are finished

	mov	current_execution_state_info(%rip),%rbx # current execution info pointer into %rbx

	xor	%r12,%r12
	mov	%r12,0x00(%rbx) # clear current instruction pointer to indicate finished
	mov     %rsp,0x08(%rbx) # set current stack pointer
        mov     %rbp,0x10(%rbx) # set current base pointer

        mov     0x18(%rbx),%rsi # parent's execution info pointer into %rsi

        mov     %rsi,current_execution_state_info(%rip) # pull our execution info from the linked list

# switch to other's stack
        mov     0x08(%rsi),%rsp
        mov     0x10(%rsi),%rbp
# switch to other's ip (.co_await_resume)
        jmp     *0x00(%rsi)
