#include <iostream>

#include "Wrapper.h"

int main() {
    srand((unsigned)time(0)); 
    string csv = "commands.csv";
	Wrapper app(csv);
	app.run();

    return 0;
}