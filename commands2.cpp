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
    cerr << "*** ERROR: forking child process failed\n";
    return 1;
  }
  else if (childpid == 0)
  {
    // Excute requested command
    if (execvp(*argsa, argsa) < 0)
    {
      // Handle exec errors
      cerr << "-dash: " << args[0] << ": command not found" << endl;
      return -2;                        // Return -2 to terminate program
    }
  }
  else
  {
    cout << "Child PID: " << childpid << endl;
    
    // Wait for completion
    while (wait(&status) != childpid);
    
    // Retrieve process stats
    struct rusage stats;
    getrusage(RUSAGE_CHILDREN, &stats);
    
    // Output process information
    cout << "Process terminated.\n"
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
    cerr << "-dash: " << args[0] << ": " << path << ": ";
    
    // Error occured, display appropriate error message
    switch(errno)
    {
    case EACCES:
      cerr << "Access denied";
      break;
      
    case EFAULT:
      cerr << "Location out of bounds";
      break;
      
    case EIO:
      cerr << "An I/O error occured";
      break;
      
    case ELOOP:
      cerr << "Too many symbolic links";
      break;
      
    case ENAMETOOLONG:
      cerr << "Path too long";
      break;
      
    case ENOENT:
    case ENOTDIR:
      cerr << "No such file or directory";
      break;
      
    case ENOMEM:
      cerr << "Insufficient kernel memory available";
      break;
      
    default:
      cerr << "An error occured";
      break;
    }
    
    cerr << endl;
    
    return 1;                          // Return 1 for error
  }
  
  return 0;
}

int onCommandSignal(const vector<string>& args)
{
  // Verify number of arguments
  if (args.size() < 3)
  {
    cout << "Usage:\n  " << args[0] << " <signal_num> <pid>" << endl;
    return 0;
  }
  
  int signal;
  int pid;
  
  // Get numbers from arguments
  signal = atoi(args[1].c_str());
  pid = atoi(args[2].c_str());
  
  if (kill(pid, signal) == 0)
  {
    cout << "Signal " << signal << " sent to process " << pid << endl;
  }
  else
  {
    cerr << "-dash: " << args[0] << ": ";
    
    // Error occured, display appropriate error message
    switch(errno)
    {
    case EINVAL:
      cerr << signal << ": Invalid signal";
      break;
      
    case EPERM:
      cerr << pid << ": Access to process denied";
      break;
      
    case ESRCH:
      cerr << pid << ": Process or process group inaccessable";
      break;
      
    default:
      cerr << "An error occured";
      break;
    }
    
    cerr << endl;
    
    return 1;
  }
  
  return 0;
}
