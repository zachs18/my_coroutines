/*
void *
co_yield(
	void *arg           // %rdi
);
*/

# idea:
#  we can assume that any things that are necessary to enter a function are already true, since we just entered co_yield
#  e.g. we don't have to deal with "The CPU shall be in x87 mode upon entry to a function." (x86-64-psABI-1.0.pdf section 3.2.1) and the like
#
# on call:
# set up stack frame
# push all callee-saved registers to our stack (rbx, r12-r15)
# pull our execution info from linked list
# restore other's execution info from linked list
# jump to other's ip (leaving yielded value in %rdi)
#
# on resume:
# move passed value to %rax
# restore saved registers
# clean stack
# return

	.globl	co_yield
	.type	co_yield, @function
co_yield:
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

	lea	.co_yield_resume(%rip),%r12
	mov	%r12,0x00(%rbx) # set current instruction pointer to where to resume to
	mov	%rsp,0x08(%rbx) # set current stack pointer
	mov	%rbp,0x10(%rbx) # set current base pointer

	mov	0x18(%rbx),%rsi # parent's execution info pointer into %rsi

	mov	%rsi,current_execution_state_info(%rip) # pull our execution info from the linked list

# switch to other's stack
	mov	0x08(%rsi),%rsp
	mov	0x10(%rsi),%rbp
# switch to other's ip
	jmp	*0x00(%rsi)

.co_yield_resume:
	endbr64
	# %rsi is the arg passed to co_await
	mov %rsi,%rax

	# co_await already pushed our info and switched to our stack in order to get here, so just restore the registers

	mov	0x28(%rsp),%rbx
	mov	0x20(%rsp),%r12
	mov	0x18(%rsp),%r13
	mov	0x10(%rsp),%r14
	mov	0x08(%rsp),%r15

	leave
	ret
