#include <iostream>

// Min macro
#define min(i, j) (((i) < (j)) ? (i) : (j))

// Min template
template<class T> T min_tpl(T i, T j) { return ((i < j) ? i : j); }

template <class T, class U> T tpl(U obj);	// Base template
//template <class U> T tpl<void, U>(U obj);	// Illegal, functions can not be partially specialized
template <class T> T tpl(int obj);			// Overloading is fine and totally legal

int main()
{
	int i = 9; int j = 10;
	int m = min(i++, j);
	std::cout << m << std::endl;

	i = 9; j = 10;
	m = min_tpl(i++, j);
	std::cout << m << std::endl;

	return 0;
}

