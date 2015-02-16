#include "commands2.h"

int onCommandUnknown(const vector<string>& args)
{
  int childpid;
  int status;
  char **argsa;
  
  // Convert string vector to c string array
  argsa = new char* [args.size()];
  for (int i = 0; i < args.size(); i++)
    argsa[i] = args[i].c_str();
  
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
      return 1;
    }
  }
  else
  {
    // Wait for completion
    while (wait(&status) != pid);
  }
  
  // Return success
  return 0;
}
