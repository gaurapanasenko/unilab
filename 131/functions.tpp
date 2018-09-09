void show_array(std::vector<std::vector<float>> arr) {
	for (int i = 0; i < arr.size(); i++) {
		printf("Array %i:\n", i + 1);
		for (int j = 0; j < arr[i].size(); j++) {
			printf("%g ", arr[i][j]);
		}
		printf("\n");
	}
}

template <typename T>
T fold_an_array(std::vector<T> arr) {
	T sum = 0;
	for (int i = 0; i < arr.size(); i++)
		sum += arr[i];
	return sum;
}

template <typename T>
T least(std::vector<T> arr) {
	if (arr.size() <= 0) return 0;
	T min = arr[0];
	for (int i = 1; i < arr.size(); i++)
		if (min > arr[i]) min = arr[i];
	return min;
}

template <typename T>
T greatest(std::vector<T> arr) {
	if (arr.size() <= 0) return 0;
	T max = arr[0];
	for (int i = 1; i < arr.size(); i++)
		if (max < arr[i]) max = arr[i];
	return max;
}

template <typename T>
T arithmetic_mean(std::vector<T> arr) {
	T sum = 0;
	for (int i = 0; i < arr.size(); i++)
		sum += arr[i];
	return sum/((T)arr.size());
}

template <typename T>
std::vector<T> append_to_items(std::vector<T> arr) {
	if (arr.size() <= 0) return std::vector<T>(0);
	T num = arr[0];
	std::vector<T> buf(arr.size());
	for (int i = 1; i < arr.size(); i++)
		buf[i] = arr[i] + num;
	return buf;
}

template <typename T>
T multiply(std::vector<T> arr) {
	if (arr.size() <= 0) return 0;
	T mul = arr[0];
	for (int i = 1; i < arr.size(); i++)
		mul *= arr[i];
	return mul;
}

template <typename T>
std::vector<T> super_function(std::vector<T> arr) {
	std::vector<T> out;
	out.push_back(fold_an_array(arr));
	out.push_back(least(arr));
	out.push_back(greatest(arr));
	out.push_back(arithmetic_mean(arr));
	out.push_back(multiply(arr));
	return out;
}

template <typename T>
std::vector<std::vector<T>> separate_numbers(std::vector<T> arr) {
	std::vector<std::vector<T>> buf(2);
	for (int i = 0; i < arr.size(); i++)
		if (arr[i] < 0) buf[1].push_back(arr[i]);
		else buf[0].push_back(arr[i]);
	return buf;
}

template <typename T>
std::vector<T> add_arrays(std::vector<std::vector<T>> arr) {
	int max = 0, i, j;
	std::vector<T> out;
	for (i = 0; i < arr.size(); i++)
		if (arr[i].size() > arr[max].size()) max = i;
	out = arr[max];
	arr.erase(arr.begin() + max);
	for (i = 0; i < arr.size(); i++)
		for (j = 0; j < arr[i].size(); j++)
			out[j] += arr[i][j];
	return out;
}
