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
      cout << "-dash: " << args[0] << ": command not found" << endl;
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
  string path;
  
  // Get the path if defined, otherwise return to home directory
  if (args.size() >= 2)
  {
    path = args[1];
  }
  else
  {
    path = "~";
  }
  
  // Glob home directory
  if (path[0] == '~')
  {
    path = getenv("HOME") + path.substr(1);
  }
  
  // Attempt to change directory
  if (chdir(path.c_str()) == -1)
  {
    cout << "-dash: " << args[0] << ": " << path << ": ";
    
    // Error occured, display appropriate error message
    switch(errno)
    {
    case EACCES:
      cout << "Access denied";
      break;
      
    case EFAULT:
      cout << "Location out of bounds";
      break;
      
    case EIO:
      cout << "An I/O error occured";
      break;
      
    case ELOOP:
      cout << "Too many symbolic links";
      break;
      
    case ENAMETOOLONG:
      cout << "Path too long";
      break;
      
    case ENOENT:
    case ENOTDIR:
      cout << "No such file or directory";
      break;
      
    case ENOMEM:
      cout << "Insufficient kernel memory available";
      break;
      
    default:
      cout << "An error occured";
      break;
    }
    
    return 1;                          // Return 1 for error
  }
  
  return 0;
}
