#ifndef ARGS_H
#define ARGS_H
#include <string>
#include <cstring>
#include <cstdio>
#include <vector>
#include <map>

class Args {
private:
	int argc;
	char **argv;
	char state;
	float buf;
	std::vector<std::vector<float>> arr;
	std::string commands;
	int cur, stlen;
	char * states;
public:
	Args(int argc_in, char **argv_in);
	~Args();
	void handle();
	void set_state(char st);
	void get_new_command(char * ch, std::vector<float>& array);
	int count_commands();
};

float fold_an_array(std::vector<float> arr);
float least(std::vector<float> arr);
float greatest(std::vector<float> arr);
float arithmetic_mean(std::vector<float> arr);
std::vector<float> append_to_items(std::vector<float> arr);
float multiply(std::vector<float> arr);

#endif
