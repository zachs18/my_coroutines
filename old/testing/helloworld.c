#include <unistd.h>

int main(int argc, char **argv) {
	write(STDOUT_FILENO, "Hello, world!", sizeof("Hello, world!")-1);
}
