#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <vector>

void show_array(std::vector<std::vector<float>> arr);

template <typename T>
T fold_an_array(std::vector<T> arr);

template <typename T>
T least(std::vector<T> arr);

template <typename T>
T greatest(std::vector<T> arr);

template <typename T>
T arithmetic_mean(std::vector<T> arr);

template <typename T>
std::vector<T> append_to_items(std::vector<T> arr);

template <typename T>
T multiply(std::vector<T> arr);

template <typename T>
std::vector<T> super_function(std::vector<T> arr);

template <typename T>
std::vector<std::vector<T>> separate_numbers(std::vector<T> arr);

template <typename T>
std::vector<T> add_arrays(std::vector<std::vector<T>> arr);

#include "functions.tpp"
#endif
