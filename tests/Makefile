#TODO: add header as dependencies

CFLAGS += -Wall

default: all

fibonacci: fibonacci.o
	$(CC) $(CFLAGS) $< -L.. -l:coroutine.so -Wl,-rpath=$(shell pwd) -o $@

primes: iterator_example.o
	$(CXX) $(CXXFLAGS) $< -L.. -l:coroutine.so -Wl,-rpath=$(shell pwd) -o $@

#fibonacci_2: fibonacci.o co_allocate.o co_free.o co_await.o co_yield.o current_execution_state_info.o
#	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean all
clean:
	rm -f *.o *.so fibonacci primes

all: fibonacci primes
