#include "Wrapper.h"

Wrapper::Wrapper(string commandsFile) {
    mCommandsFile = commandsFile;
    commandsStream.open(commandsFile);
}

Wrapper::~Wrapper() {
	commandsStream.close();
}

Wrapper::Wrapper(const Wrapper& app) {
	commandsStream.open(app.mCommandsFile);
}

bool Wrapper::checkOpenFiles() {
	// check that commands file opened successfully
	if (!commandsStream.is_open()) {
		cout << "Unable to open " << mCommandsFile;
		return false;
	}

	return true;
}

void Wrapper::run() {
	int option;
	bool exit = !checkOpenFiles();

    // import initial list of commands from csv
    importCmdList();

	// on first run: as long as files were opened successfully
	// thereafter: as long as user doesn't enter exit function
	while (!exit) {
		// display menu and prompt user for a menu option to execute
		printMenu();
		option = promptIntInRange(1, 6, "Enter an option : ");
		// clear the screen
		clrscr();

		switch (option) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			exit = true;
			break;
		}
	}
}

void Wrapper::printMenu() {
	string options[6] = { "Game Rules", "Play Game", "Load Previous Game",
        "Add Command", "Remove Command", "Exit"};

	for (int i = 0; i < 6; i++) {
		cout << "[" << i << "] " << options[i] << std::endl;
	}
}

void Wrapper::importCmdList() {
	string line;
	mCmds.clearList();
	data newData;

	// with help from https://stackoverflow.com/questions/5343173/
	commandsStream.clear();
	commandsStream.seekg(0);

	// discard title line
	std::getline(commandsStream, line);
	
	// iterate 2nd line through end
	while (std::getline(commandsStream, line)) {
		newData = parseLine(line, false);
		mCmds.insertAtFront(newData);
	}
}

data Wrapper::parseLine(string line, bool master) {
	string item, date;
	std::stringstream lineStream, nameStream;
	data newData;

	// load line into string stream
	lineStream.str(line);
	nameStream.str(string());

	getline(lineStream, item, ',');
	newData.cmdName = item;

	getline(lineStream, item, ',');
	newData.description = item;

	return newData;
}

/*************************************************************
 * Function: promptIntInRange ()                             *
 * Description: prompts user with given message for an int   *
 *        within the range provided                            *
 * Input parameters: int min, int max, string message        *
 * Returns: int                                              *
 * Preconditions: min <= max                                 *
 * Postconditions: min <= int <= max                         *
 *************************************************************/
int promptIntInRange(int min, int max, string message) {
    int prompt;

    do {
        // requires cin to load an int, clears input stream if it is not
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1024, '\n');
        }

        // print message to user and get their option input
        cout << message;
        cin >> prompt;

        // check that input is an int and is between min and max
    } while (cin.fail() || (prompt < min || prompt > max));

    return prompt;
}

void clrscr() {
    // cross platform special characters that represent the clear screen cmd
    // with help from https://stackoverflow.com/questions/17335816/clear-screen-using-c
    cout << "\033[2J\033[1;1H";
}