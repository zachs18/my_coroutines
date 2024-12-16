#include <stdio.h>

extern void rbp_until_null(void);

void test(unsigned i) {
	int j = i;
	printf("At depth: %d\n", i);
	rbp_until_null();
	if(i) test(i-1);
}

int main(void) {
	rbp_until_null();
	test(4);
	test(20);
}
