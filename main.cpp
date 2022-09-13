#include <iostream>

#include "Wrapper.h"

int main() {
    srand((unsigned)time(0));
    string cmds = "commands.csv", scores = "scores.csv";
	Wrapper app(cmds, scores);
	app.run();

    return 0;
}