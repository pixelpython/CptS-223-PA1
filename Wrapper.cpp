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
	int option, score = 0;
	bool exit = !checkOpenFiles();

    // import initial list of commands from csv
    importCmdList();
	// import previous game scores
	importProfiles();

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
			playGame(score);
			break;
		case 3:
			loadPreviousGame(score);
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
	cmdData newData;

	// with help from https://stackoverflow.com/questions/5343173/
	commandsStream.clear();
	commandsStream.seekg(0);
	
	// iterate over each line of file
	while (getline(commandsStream, line)) {
		newData = parseCmdLine(line);
		mCmds.insertAtFront(newData);
	}
}

void Wrapper::importProfiles() {
	string line;
	for (int i = 0; i < 16; i++) {
		players[i].name = "";
		players[i].score = 0;
	}
	playerCount = 0;

	// with help from https://stackoverflow.com/questions/5343173/
	gameStream.clear();
	gameStream.seekg(0);
	
	// iterate over each line of file or until profile limit
	while (getline(gameStream, line) && playerCount < 16) {
		players[playerCount] = parseProfile(line);
		playerCount++;
	}
}

cmdData Wrapper::parseCmdLine(string line) {
	string item;
	std::stringstream lineStream;
	cmdData newData;

	// load line into string stream
	lineStream.str(line);

	// name
	getline(lineStream, item, ',');
	newData.name = item;

	// next portion is quoted to allow commas within description
	if (lineStream.peek() == '"') {
		// ingest first quotation mark
		getline(lineStream, item, '"');
		// actually get description
		getline(lineStream, item, '"');
	} else {
		getline(lineStream, item, ',');
	}
	newData.description = item;

	return newData;
}

profile Wrapper::parseProfile(string line) {
	string item;
	std::stringstream lineStream;
	profile newData;

	// load line into string stream
	lineStream.str(line);

	// name
	getline(lineStream, item, ',');
	newData.name = item;

	// score
	getline(lineStream, item, ',');
	newData.score = stoi(item);

	return newData;
}

void Wrapper::playGame(int startScore) {
	int score = startScore;
	int totalGuesses = 0;
	// prompt user for questions score to end game
	int questions = promptIntInRange(5, 30, "How many questions will you answer? ");
	int cmdList[3];
	int printOrder[3];
	int usedCmds[mCmds.getLength()] = {0};

	for (int i = 0; i < questions; i++) {
		// get three random commands
		do {
			randomizeIntArray(cmdList, 3, 0, mCmds.getLength() - 1);
		} while (usedCmds[ cmdList[0] ]);
		
		// update list of used commands
		usedCmds[ cmdList[0]] = 1;

		// print in random order to screen
		randomizeIntArray(printOrder, 3, 0, 2);

		int correct = cmdList[0];
		cout << mCmds.getNodeAtPosition(correct)->data.name << endl;

		int desc;
		for (int i = 0; i < 3; i++) {
			desc = printOrder[i];
			cout << "[" << i << "] ";
			cout << mCmds.getNodeAtPosition( cmdList[desc] )->data.description << endl;
		}

		// get user guess and check adjust score
		int guess = promptIntInRange(0, 2, "Which command does this correspond to? ");
		totalGuesses++;

		clrscr();
		if (printOrder[guess] == 0) {
			score++;
			cout << "Correct! Your score is now " << score << endl;
		}
		else {
			score--;
			cout << "Wrong! Your score is now " << score << endl;
			cout << mCmds.getNodeAtPosition(0)->data.name << " - " << mCmds.getNodeAtPosition(0)->data.description << endl;
		}
	}
}

void Wrapper::loadPreviousGame(int &score) {
	string name;
	bool found = false;
	// get name from user
	cout << "Enter your name: ";
	cin >> name;

	// find match for player profile
	for (int i = 0; i < playerCount; i++) {
		if (players[i].name == name) {
			score = players[i].score;
			found = true;
			break;
		}
	}

	if (found) {
		cout << "Welcome back " << name << ". Your score is " << score << "." << endl; 
	} else {
		cout << "Unable to find matching profile for '" << name << "'." << endl;
	}
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

		node<cmdData>* pCurrent = mCmds.getHead();
		// iterate over list to find match
		while (pCurrent != nullptr) {
			if (pCurrent->data.name == name) {
				removed = mCmds.removeNode(pCurrent);
				break;
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

bool operator==(const cmdData& lhs, const cmdData& rhs) {
    return (lhs.name == rhs.name && lhs.description == rhs.description);
}
