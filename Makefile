#TODO: add header as dependencies

CFLAGS += -Wall

default: all

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

%.o: %.s
	$(CC) -c $(CFLAGS) $< -o $@

coroutine.so: co_allocate.o co_free.o co_await.o co_yield.o current_execution_state_info.o
	$(CC) -shared $(CFLAGS) $^ -o $@

fibonacci: fibonacci.o coroutine.so
	$(CC) $(CFLAGS) $< -L. -l:coroutine.so -Wl,-rpath=$(shell pwd) -o $@

primes: iterator_example.o coroutine.so
	$(CXX) $(CXXFLAGS) $< -L. -l:coroutine.so -Wl,-rpath=$(shell pwd) -o $@

#fibonacci_2: fibonacci.o co_allocate.o co_free.o co_await.o co_yield.o current_execution_state_info.o
#	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean all
clean:
	rm -f *.o *.so fibonacci primes

all: fibonacci primes
