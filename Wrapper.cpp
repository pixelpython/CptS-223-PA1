#include "Wrapper.h"

bool Wrapper::checkOpenFiles() {
	// check that commands file opened successfully
	if (!commandsStream.is_open()) {
		cout << "Unable to open " << mCommandsFile;
		return false;
	}

	if (!gameStream.is_open()) {
		cout << "Unable to open " << mGameFile;
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
			printRules();
			break;
		case 2:
			playGame();
			break;
		case 3:
			loadPreviousGame();
			break;
		case 4:
			addCmd();
			break;
		case 5:
			removeCmd();
			break;
		case 6:
			exit = true;
			if (this->modified) {
				saveFiles();
			}
			break;
		}
	}
}

void Wrapper::printMenu() {
	string options[6] = { "Game Rules", "Play Game", "Load Previous Game",
        "Add Command", "Remove Command", "Exit"};

	for (int i = 1; i < 7; i++) {
		cout << "[" << i << "] " << options[i - 1] << std::endl;
	}
}

void Wrapper::printRules() {
	cout << "The objective of the game is to match Linux commands to appropriate descriptions of those commands. If a command is matched, then the player earns 1 point. The if the command is not matched, then the player loses a point. Yes, negative point totals are possible. The player selects the number of match questions at the beginning of the game. The game continue until the number is reached.";
	cout << endl;
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

void Wrapper::playGame() {
	int score = 0;
	int totalGuesses = 0;
	// prompt user for goal score to end game
	int goal = promptIntInRange(1, 1024, "What score will you play to? ");
	int cmdList[3];
	int printOrder[3];
	int usedCmds[mCmds.getLength()] = {0};

	while (score < goal) {
		// get three random commands
		
		// if the user exhausts all the commands
		// then need to reset usedCmds to prevent infinite loop
		if (totalGuesses == mCmds.getLength()) {
			// with help from https://stackoverflow.com/questions/9146395/
			memset(usedCmds, 0, sizeof(usedCmds));
		}

		do {
			randomizeIntArray(cmdList, 3, 0, mCmds.getLength() - 1);
		} while (usedCmds[ cmdList[0]]);
		
		// update list of used commands
		usedCmds[ cmdList[0]] = 1;

		// print in random order to screen
		randomizeIntArray(printOrder, 3, 0, 2);

		int correct = cmdList[0];
		cout << mCmds.getNodeAtPosition(correct)->data.cmdName << endl;

		int desc;
		for (int i = 0; i < 3; i++) {
			desc = printOrder[i];
			cout << "[" << i << "]";
			cout << mCmds.getNodeAtPosition( cmdList[desc] )->data.description << endl;
		}

		// get user guess and check adjust score
		int guess = promptIntInRange(0, 2, "Which description does this command correspond to? ");
		totalGuesses++;

		clrscr();
		if (printOrder[guess] == 0) {
			score++;
			cout << "Correct! Your score is now " << score << endl;
		}
		else {
			score--;
			cout << "Wrong! Your score is now " << score << endl;
			cout << mCmds.getNodeAtPosition(0)->data.cmdName << " - " << mCmds.getNodeAtPosition(0)->data.description << endl;
		}
	}
}

void Wrapper::loadPreviousGame() {

}

void Wrapper::addCmd() {
	string name, description;
	cout << "Enter the command name to add: ";
	cin >> name;
	cout << "Enter the command description: ";
	cin >> description;
	cout << "Command '" << name << "' successfully added." << endl;

	this->modified = true;
}

void Wrapper::removeCmd() {
	// list must not be empty to remove any commands
	if (!mCmds.isEmpty()) {
		string name;
		bool removed = false;
		// get command to remove from user
		cout << "Enter the command name to remove: ";
		cin >> name;

		node<data>* pCurrent = mCmds.getHead();
		// check whether head matches
		if (pCurrent->data.cmdName == name) {
			// list remains intact by keeping a head node
			mCmds.setHead(pCurrent->next);
			// free memory
			delete pCurrent;
			removed = true;
		}
		else {
			node<data>* pPrev = pCurrent;
			pCurrent = pCurrent->next;

			// iterate over list to search for matching name
			while (pCurrent != nullptr) {
				if (pCurrent->data.cmdName == name) {
					// reconnect list to avoid matching node
					pPrev->next = pCurrent->next;
					// free memory
					delete pCurrent;
					removed = true;
				}
			}
		}

		// inform user whether command was successfully removed
		if (removed) {
			this->modified = true;
			cout << "Command '" << name << "' successfully removed." << endl;
		}
		else {
			cout << "Unable to find command '" << name << "' to remove." << endl;
		}
	}
	else {
		cout << "Command list is empty, unable to remove." << endl;
	}
}

void Wrapper::saveFiles() {
	
}

void Wrapper::randomizeIntArray(int intArray[], int length, int min, int max) {
	int fill = 0;
	int newInt;
	bool valid;

	// fill array with random integers in range
	while (fill < length) {
		// generate new int
		newInt = randomIntInRange(min, max);
		
		valid = true;
		// ensure newInt isn't in array already
		for (int i = 0; i < fill; i++) {
			// found in array
			if (intArray[i] == newInt) {
				valid = false;
			}
		}

		// add newInt to end of array
		if (valid) {
			intArray[fill] = newInt;
			fill++;
		}
	}
}

int randomIntInRange(int min, int max) {
	return (rand()%(max-min+1))+min;
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