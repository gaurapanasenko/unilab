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
	std::vector<float> arr;
	char * states;
public:
	Args(int argc_in, char **argv_in);
	~Args();
	void handle();
	void set_state(char st);
	void end_state();
	void fold_an_array();
	void least();
	void greatest();
	void arithmetic_mean();
	void append_to_items();
	void multiply();
};

#endif
