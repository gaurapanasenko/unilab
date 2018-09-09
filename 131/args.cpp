#include "args.h"

Args::Args(int argc_in, char **argv_in) {
	argc = argc_in;
	argv = argv_in;
	states = (char*)"hflgasm";
	stlen = strlen(states);
	commands = "";
	cur = 0;
	set_state(0);
}

void Args::handle() {
	char * s;
	for (int i = 0; i < argc; i++) {
		s = argv[i];
		if (!strcmp(s, "--")) set_state(0);
		else if (s[0] == '-' && s[2] == 0) {
			for (int i = 0; i < stlen; i++)
				if (s[1] == states[i]) {
					set_state(states[i]);
					arr.push_back(std::vector<float>(0));
					commands += states[i];
					break;
					continue;
				}
		} else if (state && arr.size() > 0) {
			sscanf(s, "%f", &buf);
			arr.back().push_back(buf);
		}
	}
}

Args::~Args() {}

void Args::set_state(char st) {
	state = st;
}

void Args::get_new_command(char * ch, std::vector<float>& array) {
	array = arr[cur];
	*ch = commands[cur];
	cur++;
}

int Args::count_commands() {return commands.size();}

float fold_an_array(std::vector<float> arr) {
	float sum = 0;
	for (int i = 0; i < arr.size(); i++)
		sum += arr[i];
	return sum;
}

float least(std::vector<float> arr) {
	if (arr.size() <= 0) return 0;
	float min = arr[0];
	for (int i = 1; i < arr.size(); i++)
		if (min > arr[i]) min = arr[i];
	return min;
}

float greatest(std::vector<float> arr) {
	if (arr.size() <= 0) return 0;
	float max = arr[0];
	for (int i = 1; i < arr.size(); i++)
		if (max < arr[i]) max = arr[i];
	return max;
}

float arithmetic_mean(std::vector<float> arr) {
	float sum = 0;
	for (int i = 0; i < arr.size(); i++)
		sum += arr[i];
	return sum/((float)arr.size());
}

std::vector<float> append_to_items(std::vector<float> arr) {
	if (arr.size() <= 0) return std::vector<float>(0);
	float num = arr[0];
	std::vector<float> buf(arr.size());
	for (int i = 1; i < arr.size(); i++)
		buf[i] = arr[i] + num;
	return buf;
}

float multiply(std::vector<float> arr) {
	if (arr.size() <= 0) return 0;
	float mul = arr[0];
	for (int i = 1; i < arr.size(); i++)
		mul *= arr[i];
	return mul;
}
