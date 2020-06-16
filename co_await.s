/*
void *
co_await(
	co_handle_t handle, // %rdi
	void *arg           // %rsi
);
*/

# idea:
#  we can assume that any things that are necessary to enter a function are already true, since we just entered co_await
#  e.g. we don't have to deal with "The CPU shall be in x87 mode upon entry to a function." (x86-64-psABI-1.0.pdf section 3.2.1) and the like
#
# on call:
# set up stack frame
# push all callee-saved registers to our stack (rbx, r12-r15)
# push execution_state_info to linked list
# jump to other's ip (leaving passed value in %rsi)
#
# on resume:
# move yielded value to %rax
# restore saved registers
# clean stack
# return

	.globl	co_await
	.type	co_await, @function
co_await:
	endbr64
	push	%rbp
	mov	%rsp, %rbp
	sub	$0x30,%rsp # 5 saved registers + 1 local var

	mov	%rbx,0x28(%rsp)
	mov	%r12,0x20(%rsp)
	mov	%r13,0x18(%rsp)
	mov	%r14,0x10(%rsp)
	mov	%r15,0x08(%rsp)

	mov	current_execution_state_info(%rip),%rbx # current execution info pointer into %rbx

	lea	.co_await_resume(%rip),%r12
	mov	%r12,0x00(%rbx) # set current instruction pointer to where to resume to
	mov	%rsp,0x08(%rbx) # set current stack pointer
	mov	%rbp,0x10(%rbx) # set current base pointer

	mov	%rbx,0x18(%rdi) # set other's next pointer to current

	mov	%rdi,current_execution_state_info(%rip) # push other's execution info to the linked list

# switch to other's stack
	mov	0x08(%rdi),%rsp
	mov	0x10(%rdi),%rbp
# switch to other's ip (.co_yield_resume or co_wrapper)
	jmp	*0x00(%rdi)

.co_await_resume:
	endbr64
	# %rdi is the arg passed to co_yield (or returned)
	mov %rdi,%rax

	# co_yield already popped other's info and switched to our stack in order to get here, so just restore the registers

	mov	0x28(%rsp),%rbx
	mov	0x20(%rsp),%r12
	mov	0x18(%rsp),%r13
	mov	0x10(%rsp),%r14
	mov	0x08(%rsp),%r15

	leave
	ret
