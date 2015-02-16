#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

using namespace std;

int onCommandCmdnm(const vector<string>& args);
int onCommandPid(const vector<string>& args);
int onCommandSystat(const vector<string>& args);
int onDragonborn(const vector<string>& args);
