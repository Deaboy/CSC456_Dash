#include "commands.h"

/**
 * onCommandCmdnm
 * Author: Daniel Andrus
 *
 * Handles the "cmdnm" command. Fetches the command name for thep rocess that
 * matches the given process ID if one exists.
 *
 * args - string vector of command arguments
 *
 * Returns - 0
**/
int onCommandCmdnm(const vector<string>& args)
{
  // Declare variables
  ifstream fin;
  string cmdnm;
  string path;
  int name_count;
  int pos;
  
  // Verify number of arguments
  if (args.size() != 2)
  {
    cout << "Usage:\n"
            "  cmdnm <pid>" << endl;
    return 0;
  }
  
  // Construct path
  path = "/proc/";
  path += args[1];
  path += "/comm";
  
  // Assume pid is an int. If it's not, it'll throw an error anyway
  fin.open(path.c_str());
  if (!fin)
  {
    cout << "ERROR: One of two things:\n"
            "- No permission to access process files\n"
            "- No process with PID \"" << args[1] << "\"" << endl;
    return 0;
  }
  
  name_count = 0;
  while (getline(fin, cmdnm))
  {
    pos = cmdnm.find_last_of("/");
    if (cmdnm != "")
    {
      if (pos != -1)
        cmdnm = cmdnm.substr(pos + 1);
      cout << "Command name: " << cmdnm << endl;
      name_count++;
    }
  }
  
  if (name_count == 0)
  {
    cout << "No command found for process " << args[1] << endl;
  }
  
  fin.close();
  
  return 0;
}


/**
 * onCommandPid
 * Author: Daniel Andrus
 *
 * Handles the "pid" command. Searches through the /proc directory to find a
 * process whose command matches the search string given by the user and
 * displays any matching process' PIDs.
 *
 * args - string vector of command arguments
 *
 * Returns - 0
**/
int onCommandPid(const vector<string>& args)
{
  // Declare variables
  ifstream fin;
  vector<string> pids;
  DIR *dir;
  struct dirent *pid;
  string temp;
  
  // Verify number of arguments
  if (args.size() != 2)
  {
    cout << "Usage:\n"
            "  pid <command>" << endl;
    return 0;
  }
  
  // Open handler to /proc directory
  if ((dir = opendir("/proc")) == NULL)
  {
    cout << "Error: unable to access /proc folder." << endl;
    return 0;
  }
  
  // Attempt to open cmdline file of each folder
  while ((pid = readdir(dir)) != NULL)
  {
    fin.open((string("/proc/") + pid->d_name + "/cmdline").c_str());
    if (!fin) continue;
    
    while (getline(fin, temp))
    {
      if (temp.find(args[1]) != -1)
      {
        pids.push_back(pid->d_name);
        break;
      }
    }
    
    fin.close();
  }
  
  // Close pointer to directory handler
  closedir(dir);
  
  
  // Output search results
  if (pids.size() == 0)
  {
    cout << "No matching processes" << endl;
  }
  else
  {
    cout << "Matching PIDs:\n";
    for (int i = 0; i < pids.size(); i++)
    {
      cout << "  " << pids[i] << "\n";
    }
    cout.flush();
  }
  
  return 0;
}


/**
 * onCommandSystat
 * Author: Daniel Andrus
 *
 * Handles the "systat" command. Displays system information, such as OS version
 * info, system uptime, memory usage, and cpu information.
 *
 * args - string vector of command arguments
 *
 * Returns - 0
**/
int onCommandSystat(const vector<string>& args)
{
  // Note: ignore arguments
  ifstream fin;
  double tmp1, tmp2, tmp3;    // Temporary variables
  string str1, str2, str3;    // Temporary variables
  
  cout << "System status\n"
          "==========================\n";
  
  // SYSTEM UPTIME
  fin.open("/proc/uptime");
  if (fin)
  {
    // Read in system uptime in seconds (with fractions of a second possible)
    fin >> tmp1;
    
    cout << "Uptime:\n  ";
    
    // Days
    tmp2 = (int) (tmp1 / 86700);
    tmp1 -= tmp2 * 86700;
    cout << (int) tmp2 << (tmp2 == 1 ? " day, " : " days, ");
    
    // Hours
    tmp2 = (int) (tmp1 / 3600);
    tmp1 -= tmp2 * 3600;
    cout << (int) tmp2 << (tmp2 == 1 ? " hour, " : " hours, ");
    
    // Minutes
    tmp2 = (int) (tmp1 / 60);
    tmp1 -= tmp2 * 60;
    cout << (int) tmp2 << (tmp2 == 1 ? " minute, " : " minutes, ");
    
    // Seconds
    tmp2 = (int) (tmp1);
    cout << (int) tmp2 << (tmp2 == 1 ? " second " : " seconds ") << "\n";
    
    fin.close();
  }
  else
  {
    cout << "Unable to retrieve system uptime.\n";
  }
  
  
  // OS AND KERNEL VERSION
  fin.open("/proc/version");
  if (fin)
  {
    cout << "OS/Kernel version info:\n";
    
    // Read in every line and print to console
    while (getline(fin, str1))
    {
      cout << "  " << str1 << "\n";
    }
    
    fin.close();
  }
  else
  {
    cout << "Unable to retrieve os and kernel version.\n";
  }
  
  
  // Memory info
  fin.open("/proc/meminfo");
  if (fin)
  {
    cout << "Memory info:\n";
    
    // Echo first 3 lines (MemTotal, MemFree, MemAvailable)
    for (int i = 0; i < 3; i++)
    {
      getline(fin, str1);
      cout << "  " << str1 << '\n';
    }
    
    fin.close();
  }
  else
  {
    cout << "Unable to retrieve memory info.\n";
  }
  
  
  // Processor info
  fin.open("/proc/cpuinfo");
  if (fin)
  {
    cout << "CPU Info:\n";
    
    // Read in files
    for (int i = 0; getline(fin, str1) && str1 != ""; i++)
    {
      // Only print out specific lines
      switch(i)
      {
      case 1:
      case 4:
      case 7:
      case 8:
        cout << "  " << str1 << "\n";
      
      default:
        break;
      }
    }
    
    fin.close();  
  }
  else
  {
    cout << "Unable to retrieve cpu info.\n";
  }
  
  
  // Clean up and flush output
  if (fin)
    fin.close();
  cout.flush();
  
  return 0;
}


/**
 * onDragonborn
 * Author: Daniel Andrus
 *
 * Prints out the lyrics to "Dragonborn", the theme song of Skyrim to the
 * console in the dragon language! FUS RO DAH!
 *
 * args - string vector of command arguments
 *
 * Returns - 0
**/
int onDragonborn(const vector<string>& args)
{
  ifstream fin;
  string input;
  
  int char_delay = 25 * 1000;
  int line_delay = 250 * 1000;
  
  fin.open(".dragonborn.txt");
  if (!fin)
  {
    cout << "Sorry, unable to open 'dragonborn.txt'!\n";
    return 0;
  }
  
  cout << '\n';
  
  while (getline(fin, input))
  {
    if (input == "")
    {
      cout << endl;
      continue;
    }
    
    cout << "  ";
    
    for (int i = 0; i < input.size(); i++)
    {
      cout << input[i];
      cout.flush();
      usleep(char_delay);
    }
    cout << endl;
    usleep(line_delay);
  }
  
  fin.close();
  
  cout << "\nFUS RO DAH!\n" << endl;
}
