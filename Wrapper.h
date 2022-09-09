#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "List.h"

using std::cout;
using std::cin;
using std::string;

struct data {
    string cmdName;
    string description;
};

class Wrapper {
public:
    Wrapper(string commandsFile);
    ~Wrapper();
    Wrapper(const Wrapper& app);

    void run();
    
private:
    List<data> mCmds;
    string mCommandsFile;
    std::ifstream commandsStream;

    bool checkOpenFiles();

    void printMenu();
    void importCmdList();
    data parseLine(string line, bool master);

};

int promptIntInRange(int min, int max, string message);
void clrscr();