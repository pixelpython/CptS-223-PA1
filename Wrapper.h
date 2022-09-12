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

struct data {
    string cmdName;
    string description;
};

class Wrapper {
public:
    Wrapper(string commandsFile) {
        mCommandsFile = commandsFile;
        commandsStream.open(commandsFile);
    }

    ~Wrapper()  {
	    commandsStream.close();
    }

    Wrapper(const Wrapper& app) {
        commandsStream.open(app.mCommandsFile);
    }

    void run();
    
private:
    List<data> mCmds;
    string mCommandsFile;
    std::ifstream commandsStream;

    void importCmdList();
    data parseLine(string line, bool master);
    bool checkOpenFiles();
    void printMenu();
    
    void printRules();
    void playGame();
    void loadPreviousGame();
    void addCmd();
    void removeCmd();

    void randomizeIntArray(int intArray[], int length, int min, int max);
};

int randomIntInRange(int min, int max);
int promptIntInRange(int min, int max, string message);
void clrscr();