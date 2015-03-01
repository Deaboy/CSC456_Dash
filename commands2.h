#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>

using namespace std;

int onCommandUnknown(const vector<string>& args);
int onCommandCd(const vector<string>& args);
