#include "commands2.h"

int onCommandUnknown(const vector<string>& args)
{
  int childpid;
  int status;
  char **argsa;
  
  // Convert string vector to c string array
  argsa = new char* [args.size()];
  for (int i = 0; i < args.size(); i++)
  {
    argsa[i] = new char [args[i].length()+1];
    strcpy(argsa[i], args[i].c_str());
  }
  
  // Fork process
  childpid = fork();
  
  if (childpid < 0)
  {
    // Handle forking errors
    cout << "*** ERROR: forking child process failed\n";
    return 1;
  }
  else if (childpid == 0)
  {
    // Excute requested command
    if (execvp(*argsa, argsa) < 0)
    {
      // Handle exec errors
      cout << "ERROR: failed to exec\n";
      return -2;                        // Return -2 to terminate program
    }
  }
  else
  {
    // Wait for completion
    while (wait(&status) != childpid);
    
    // Retrieve process stats
    struct rusage stats;
    getrusage(RUSAGE_CHILDREN, &stats);
    
    // Output process information
    cout << "Process terminated.\n"
         << "          PID: " << childpid << '\n'
         << "    User time: " << stats.ru_utime.tv_usec << '\n'
         << "  System time: " << stats.ru_stime.tv_usec << '\n'
         << "  Page faults: " << stats.ru_minflt << '\n'
         << "   Page swaps: " << stats.ru_nswap << endl;
  }
  
  // Return success
  return 0;
}

int onCommandCd(const vector<string>& args)
{
  return 0;
}
