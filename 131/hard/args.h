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
	std::vector<std::vector<std::vector<float>>> arr;
	std::string commands;
	int cur, stlen;
	char * states;
public:
	Args(int argc_in, char **argv_in);
	~Args();
	void handle();
	void set_state(char st);
	void get_new_command(char * ch, std::vector<std::vector<float>>& array);
	int count_commands();
};

#endif
