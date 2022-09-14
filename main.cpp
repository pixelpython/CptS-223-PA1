#include <iostream>

#include "Wrapper.h"

int main() {
    // initialize randomness
    srand((unsigned)time(0));
	Wrapper app( "commands.csv",  "scores.csv" );
	app.run();

    return 0;
}