#include <tuple>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <iostream>
#include <iterator>
#include <type_traits>


/*
Now support:
1. Single Dimention
vector<T>
list<T>
tuple<T...>
map<T, U>, unordered_map<T, U>
set<T>, unordered_set<T>

2. Multiple Dimentions
N-D vector
map<T, N-D vector>, etc.
*/


template <unsigned int N = 0, class F, class...T>
typename std::enable_if<N == sizeof...(T), void>::type tuple_for_each(const std::tuple<T...>&, F&& f) {}

template <unsigned int N = 0, class F, class...T>
typename std::enable_if<N < sizeof...(T), void>::type tuple_for_each(const std::tuple<T...>& t, F&& f){
	f(std::get<N>(t));
	tuple_for_each<N + 1, F, T...>(t, std::forward<F>(f));
}


template <unsigned int N = 0, class F, class... T>
typename std::enable_if<N == sizeof...(T), void>::type tuple_for_each(std::tuple<T...>&&, F&& f) {}

template <unsigned int N = 0, class F, class... T>
typename std::enable_if<N < sizeof...(T), void>::type tuple_for_each(std::tuple<T...>&& t, F&& f){
	f(std::get<N>(t));
	tuple_for_each<N + 1, F, T...>(std::move(t), std::forward<F>(f));
}

auto tuple_print = [](auto&& x){std::cout << x << " ";};


template<template<class, class...> class Container, class Value, class... Args>
void print(const Container<Value, Args...>& c){
    for (const auto& v : c){
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

template <class T>
typename std::enable_if<!std::is_pod<T>::value, void>::type print(const std::vector<T>& v) { 
    for (const auto& e : v){
        print(e);
    }
}

template <class T> void print(const T& t){}

template<>
void print<>(const std::string& s){
    std::cout << s << std::endl;
} 
template <class T>
typename std::enable_if<std::is_pod<T>::value, void>::type print(const T& e) {
    std::cout << e << std::endl; 
}


template<class... T>
void print(std::tuple<T...>& t){
    tuple_for_each(t, tuple_print);
    std::cout << std::endl;
}

template<class First, class Second>
void print(const std::pair<First, Second>& p){
    std::cout << "key is: ";
    print(p.first);
    std::cout << "value is: ";
    print(p.second);
}

template<class Key, class Value>
void print(const std::unordered_map<Key, Value>& c){
    for (const std::pair<Key, Value>& p : c){
        print(p);
    }
}

template<class Key, class Value>
void print(const std::map<Key, Value>& c){
    for (const std::pair<Key, Value>& p : c){
        print(p);
    }
}




int main(){
	auto t = std::make_tuple(1, "3", "abc", 2);
	print(t);

    std::vector<int> v = {1, 2, 3, 4, 5};
    print(v);

    std::list<int> l = {1, 2, 3, 4, 5};
    print(l);

    std::unordered_map<std::string, std::vector<std::vector<int>>> um = {{"one", {{1,2}, {3,4}}}, {"two", {{5,6}, {7,8}}}};
    print(um);

    std::map<std::string, std::vector<std::vector<int>>> m = {{"one", {{1,2}, {3,4}}}, {"two", {{5,6}, {7,8}}}};
    print(m);

    std::vector<std::vector<std::vector<int>>> vv = {{{1,2}, {3,4}}, {{1,2}, {3,4}}};
    print(vv);

    std::unordered_set<std::string> us = {"one", "two", "three"};
    print(us);

    std::set<std::string> s = {"one", "two", "three"};
    print(s);

	return 0;
}