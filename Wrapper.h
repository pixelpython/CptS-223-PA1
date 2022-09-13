#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <string.h>

#include "List.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

struct cmdData {
    string name;
    string description;
};

struct profile {
    string name;
    int score;
};

class Wrapper {
public:
    Wrapper(string commandsFile, string gameFile) {
        mCommandsFile = commandsFile;
        commandsStream.open(commandsFile);
        mGameFile = gameFile;
        gameStream.open(gameFile);

        modified = false;
    }

    ~Wrapper()  {
	    commandsStream.close();
        gameStream.close();
    }

    Wrapper(const Wrapper& app) {
        commandsStream.open(app.mCommandsFile);
    }

    void run();
    
private:
    List<cmdData> mCmds;
    profile players[16];
    int playerCount;
    string mCommandsFile;
    string mGameFile;
    std::ifstream commandsStream;
    std::ifstream gameStream;

    void importCmdList();
    void importProfiles();
    cmdData parseCmdLine(string line);
    profile parseProfile(string line);
    bool checkOpenFiles();
    void printMenu();
    
    void printRules();
    void playGame(int startScore);
    void loadPreviousGame(int &score);
    void addCmd();
    void removeCmd();

    bool modified;
    void saveFiles();

    void randomizeIntArray(int intArray[], int length, int min, int max);
};

int randomIntInRange(int min, int max);
int promptIntInRange(int min, int max, string message);
void clrscr();
bool operator==(const cmdData& lhs, const cmdData& rhs);
