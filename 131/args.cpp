#include "args.h"

Args::Args(int argc_in, char **argv_in) {
	argc = argc_in;
	argv = argv_in;
	states = (char*)"hflgasmpok";
	stlen = strlen(states);
	commands = "";
	cur = 0;
	set_state(0);
}

void Args::handle() {
	char * s;
	for (int i = 0; i < argc; i++) {
		s = argv[i];
		if (!strcmp(s, "--"))
			arr.back().push_back(std::vector<float>(0));
		else if (s[0] == '-' && s[1] >= 'a' && s[1] <= 'z' && s[2] == 0) {
			for (int i = 0; i < stlen; i++)
				if (s[1] == states[i]) {
					set_state(states[i]);
					arr.push_back(std::vector<std::vector<float>>(1,
						std::vector<float>(0)));
					commands += states[i];
					break;
					continue;
				}
		} else if (arr.size() > 0 && arr.back().size() > 0) {
			sscanf(s, "%f", &buf);
			arr.back().back().push_back(buf);
		}
	}
}

Args::~Args() {}

void Args::set_state(char st) {
	state = st;
}

void Args::get_new_command(char * ch, std::vector<std::vector<float>>& array) {
	array = arr[cur];
	*ch = commands[cur];
	cur++;
}

int Args::count_commands() {return commands.size();}
