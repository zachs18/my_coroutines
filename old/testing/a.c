extern void *test_ptr;
void *test_initialized = 0;

int main(int argc, char **argv) {
	return !!test_ptr || !!test_initialized;
}

int test(int i) {
	int j = 4;
	int k = 5;
	++k, --j;
	return i + j + k;
}
