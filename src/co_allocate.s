/*
struct execution_state_info *
co_allocate(
	void (*func)(void*), // %rdi
);
*/

	.globl	co_allocate
	.type	co_allocate, @function
co_allocate: # This could probably be written in C
	push	%rbp        # Save old frame pointer
	mov	%rsp,%rbp   # point frame pointer to top of stack
	sub	$0x20,%rsp
		# local vars:
		# 0x00(%rsp): coroutine stack pointer
		# 0x08(%rsp): coroutine execution info pointer
		# 0x10(%rsp): func
		# 0x18(%rsp): unused

	testq	%rdi,%rdi
	je	.bad_arg         # func must not be NULL

	mov	%rdi,0x10(%rsp)  # func

	mov	$(2 << 20),%rdi  # Allocate coroutine stack (2 MiB), malloc is 16-byte aligned on amd64
	call	malloc
	testq	%rax, %rax       # If NULL, return NULL
	je	.bad_stack
	mov	%rax,0x0(%rsp)

	mov	$(4 * 8),%rdi  # Allocate coroutine state info struct (4 pointers)
	call	malloc
	testq	%rax, %rax     # If NULL, return NULL
	je	.bad_info

	# Coroutine state info struct pointer in %rax
	# ip at 0x00(%rax)
	# sp at 0x08(%rax)
	# bp at 0x10(%rax)
	# next at 0x18(%rax)

	mov	0x0(%rsp),%rcx  # load top of stack
	add	$(2 << 20),%rcx # bottom of stack
	mov	%rcx,0x10(%rax) # info->bp
	sub	$0x10,%rcx      # allocate room on coroutine stack for func pointer for co_wrapper
	mov	%rcx,0x08(%rax) # info->sp

	mov	0x10(%rsp),%rdi # load func into %rdi
	mov	%rdi,(%rcx)

	lea	co_wrapper(%rip),%rdx # load wrapper entry point into %rdx
	mov	%rdx,0x00(%rax)       # and store in info->ip

	leave
	ret
.bad_info:
	mov	0x0(%rsp),%rdi
	call	free
.bad_stack:
.bad_arg:
	mov	$0x0,%rax
	leave
	ret



	.globl	co_finished
	.type	co_finished, @function
co_finished:
	endbr64
	cmp	$0,(%rdi)
	setz	%al
	ret
/*
co_wrapper
not a C function, only used in asm as the entry/exit point for coroutines
*/

# idea:
# this function is given as the ip of newly allocated coroutines
# when a coroutine is allocated, it's stack contains (at least) 16 bytes
# a pointer to the function to be called located at the top (%rsp)
#
# When a newly allocated coroutine is co_awaited, this function will start
#  (with %rsp and %rbp already set and arg in %rsi from co_await)
# When a coroutine completes, this function will finish (popping state info and jumping back to the caller of co_await)
co_wrapper: # called (indirectly) on first co_await; same state as .co_yield_resume
	endbr64
	mov	%rsi,%rdi    # arg
	mov	(%rsp),%rsi  # func
	call	*%rsi

	mov	%rax,%rdi    # co_await expects the value in %rdi

	# this part is similar to the beginning of co_yield
	# to pop our execution state and set it's ip to NULL to indicate we are finished

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
