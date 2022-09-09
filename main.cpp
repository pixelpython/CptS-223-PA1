#include <iostream>

#include "Wrapper.h"

int main() {
    string csv = "commands.csv";
	Wrapper app(csv);
	app.run();

    return 0;
}