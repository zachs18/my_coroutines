cmake_minimum_required(VERSION 2.8)

link_directories(${CMAKE_CURRENT_BINARY_DIR})

project(mycoroutines C CXX ASM)

enable_testing()

set(CO_SOURCES
	src/co_allocate.c
	src/co_wrapper.s
	src/co_free.c
	src/co_finished.c
	src/co_await.s
	src/co_yield.s
	src/current_execution_state_info.c
)

set(AIO_SOURCES
	src/co_aio.c
)

include_directories(include)

add_library(coroutine SHARED ${CO_SOURCES})

add_library(co_aio SHARED ${AIO_SOURCES})
target_link_libraries(co_aio coroutine)

add_executable(fibonacci tests/fibonacci.c)
add_dependencies(fibonacci coroutine)
target_link_libraries(fibonacci coroutine)

add_executable(primes tests/iterator_example.cpp)
add_dependencies(primes coroutine)
target_link_libraries(primes coroutine)

add_test(NAME fibonacci COMMAND fibonacci)
