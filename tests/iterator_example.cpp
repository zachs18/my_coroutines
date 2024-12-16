#include "my_coroutines.h"

#include <limits>
#include <iostream>

class coroutine_iterator {
	co_handle_t m_handle;
	intptr_t m_cached;
	bool m_past_end = false;
public:
	coroutine_iterator() : m_handle{} {} // end iterator
	coroutine_iterator(intptr_t (*func)(intptr_t), intptr_t arg = 0) {
		m_handle = co_allocate(func);
		if (m_handle) {
			m_cached = co_await(m_handle, arg);
		} else {
			m_past_end = true;
		}
	}
	~coroutine_iterator() {
		co_free(m_handle);
	}

	coroutine_iterator(const coroutine_iterator&) = delete;
	coroutine_iterator &operator=(const coroutine_iterator&) = delete;

	coroutine_iterator(coroutine_iterator &&rhs) : m_handle{} {
		using std::swap;
		swap(m_handle, rhs.m_handle);
		m_cached = rhs.m_cached;
		m_past_end = rhs.m_past_end;
	}
	coroutine_iterator &operator=(coroutine_iterator &&rhs) {
		using std::swap;
		swap(m_handle, rhs.m_handle);
		swap(m_cached, rhs.m_cached);
		swap(m_past_end, rhs.m_past_end);
		return *this;
	}

	bool operator==(const coroutine_iterator &rhs) const {
		if (m_handle == nullptr && rhs.m_handle == nullptr) return true;
		else if (m_handle == nullptr && rhs.m_past_end) return true;
		else if (m_past_end && rhs.m_handle == nullptr) return true;
		else return false;
	}

	bool operator!=(const coroutine_iterator &rhs) const {
		return !operator==(rhs);
	}

	coroutine_iterator &operator++(void) {
		if (co_finished(m_handle)) {
			m_past_end = true;
		} else {
			m_cached = co_await(m_handle, 0);
		}
		return *this;
	}

	intptr_t operator*(void) const {
		return m_cached;
	}
};

class coroutine_range {
	intptr_t (*m_func)(intptr_t);
	intptr_t m_arg;
public:
	coroutine_range() : m_func{}, m_arg{} {} // NULL
	coroutine_range(intptr_t (*func)(intptr_t), intptr_t arg = 0) : m_func{func}, m_arg{arg} {}

	coroutine_iterator begin() const {
		return coroutine_iterator{m_func, m_arg};
	}
	coroutine_iterator end() const {
		return coroutine_iterator{};
	}
};

bool is_prime(intptr_t p) {
	if (p < 2 || (p % 2 == 0)) return false; // .. -1, 0, 1, 2, 4, 6, ...
	if (p < 9) return true;// 3, 5, 7
	for (intptr_t i = 3; i < p/2; i += 2) { // this function is inefficient anyway, but could use intsqrt instead of /2
		if (p % i == 0) return false;
	}
	return true;
}

intptr_t primes_upto(intptr_t upto) {
	intptr_t prev = 2, p = 3;
	while (p < upto) {
		co_yield(prev);
		prev = p;
		p += 2;
		while (!is_prime(p) && p < upto) p += 2;
	}
	return prev;
}

int main(void) {
	intptr_t max;
	std::cout << "Primes up to what number: " << std::flush;
	std::cin >> max;
	if (!std::cin) return EXIT_FAILURE;
	for (auto i : coroutine_range(primes_upto, max)) {
		std::cout << i << std::endl;
	}
	return EXIT_SUCCESS;
}
