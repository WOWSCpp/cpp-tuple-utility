


using namespace std;
#include <tuple>
#include <string>
#include <iostream>

template <unsigned int N = 0, class F, class... T>
typename enable_if<N == sizeof...(T), void>::type for_each(tuple<T...>&, F&& f) {}

template <unsigned int N = 0, class F, class...T>
typename enable_if<N < sizeof...(T), void>::type for_each(tuple<T...>& t, F&& f)
{
	f(get<N>(t));
	for_each<N + 1, F, T...>(t, std::forward<F>(f));
}


template <unsigned int N = 0, class F, class...T>
typename enable_if<N == sizeof...(T), void>::type for_each(const tuple<T...>&, F&& f) {}

template <unsigned int N = 0, class F, class...T>
typename enable_if<N < sizeof...(T), void>::type for_each(const tuple<T...>& t, F&& f)
{
	f(get<N>(t));
	for_each<N + 1, F, T...>(t, std::forward<F>(f));
}


template <unsigned int N = 0, class F, class... T>
typename enable_if<N == sizeof...(T), void>::type for_each(tuple<T...>&&, F&& f) {}

template <unsigned int N = 0, class F, class... T>
typename enable_if<N < sizeof...(T), void>::type for_each(tuple<T...>&& t, F&& f)
{
	f(get<N>(t));
	for_each<N + 1, F, T...>(move(t), std::forward<F>(f));
}

template <unsigned int N, class F, class T>
struct Unpack
{
	template <class... Args>
	static decltype(auto) unpack(F&& f, T&& t, Args&&... args)
	{
		return Unpack<N - 1, F, T>::unpack(
			forward<F>(f),
			forward<T>(t),
			get<N - 1>(forward<T>(t)),
			forward<Args>(args)...);
	}
};

template <class F, class T>
struct Unpack<0, F, T>
{
	template <class... Args>
	static decltype(auto) unpack(F&& f, T&& t, Args&&... args)
	{
		return f(forward<Args>(args)...);
	}
};

template <class F, class... Args>
decltype(auto) wrapper(F&& f, tuple<Args...>& t)
{
	return Unpack<sizeof...(Args), F, const tuple<Args...>&>::unpack(forward<F>(f), t);
}

template <class F, class... Args>
decltype(auto) wrapper(F&& f, const tuple<Args...>& t)
{
	return Unpack<sizeof...(Args), F, tuple<Args...>&>::unpack(forward<F>(f), t);
}

template <class F, class... Args>
decltype(auto) wrapper(F&& f, tuple<Args...>&& t)
{
	return Unpack<sizeof...(Args), F, tuple<Args...>&&>::unpack(forward<F>(f), move(t));
}

void foo(int i, const std::string& s)
{
	cout << "int para: " << i << "\n"
		<< "str para: " << s << "\n";
}

int main()
{
	auto t = make_tuple(1, "2");
	Unpack<2, decltype(foo), decltype(t)&>::unpack(foo, t);
	wrapper(foo, t);
	system("pause");
	return 0;
}