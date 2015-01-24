/* CSC456 Prog 1 - "Dash"
 * Author:
 *   Daniel Andrus
 * 
 * Description:
 * Command interface for viewing and managing system processes
 *
 * TODO
 * - Command: cmdnm <pid>
 * - Command: pid <command>
 * - Command: systat
 * - Command: exit
 * - Handle errors
 * - Seperate commands into individual files
 * - Command: help
 * - Tab-completion (tab) 
 * - Command history (up-arrow)
 * 
 * Links
 * - http://cc.byexamples.com/2008/06/16/gnu-readline-implement-custom-auto-complete/
 * - http://www.mcs.sdsmt.edu/ckarlsso/csc456/spring15/homeworks/csc456_HW1.pdf
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void commandPreprocess(string& command, vector<string>& args);
int onCommand(const vector<string>& args);
int onCommandExit(const vector<string>& args);
int onCommandCmdnm(const vector<string>& args);
int onCommandPid(const vector<string>& args);
int onCommandSystat(const vector<string>& args);
int onCommandHelp(const vector<string>& args);

int main()
{
  cout << "Welcome to Dash, written by Daniel Andrus\n"
          "Type \"help\" for command help." << endl;
  
  int status;
  string input;
  string command;
  vector<string> args;
  bool exit = false;
  
  // Enter main program loop
  while (!exit)
  {
    // Output prompt
    cout << "dash>";
    
    // Input user command
    getline(cin, input);
    
    // Preprocess command and arguments
    command = input;
    commandPreprocess(command, args); 
    
    // Send command to parsing function
    status = onCommand(args);
    
    switch (status)
    {
    case -1:        // exit program
      exit = true;
      break;
      
    case 1:         // error
      cout << "\nAn error occured while executing command\n"
              "  " << input << endl;
      break;
      
    default:        // continue as normal
      break;
    }
  }
  
  cout << "Goodbye!" << endl;
  
  return 0;
}

int onCommand(const vector<string>& args)
{
  // Distribute control to appropriate function
  if (args.size() == 0)
  {
    // Do nothing
  }
  else if (args[0] == "exit")
  {
    return onCommandExit(args);
  }
  else if (args[0] == "help")
  {
    return onCommandHelp(args);
  }
  else if (args[0] == "cmdnm")
  {
    return onCommandCmdnm(args);
  }
  else if (args[0] == "pid")
  {
    return onCommandPid(args);
  }
  else if (args[0] == "systat")
  {
    return onCommandSystat(args);
  }
  else
  {
    cout << "Unknown command \"" << args[0] << "\"" << endl;
  }
  
  // Return 0 to continue as normal
  return 0;
}

void commandPreprocess(string& command, vector<string>& args)
{
  args.clear();
  string tmp = "";
  
  // Split command into subcommands
  for (int i = 0; i <= command.size(); i++)
  {
    if (command[i] == ' ' || command[i] == '\t' 
        || command == "\n" || i == command.size())
    {
      if (!tmp.empty())
      {
        args.push_back(tmp);
        tmp = "";
      }
    }
    else
    {
      tmp += command[i];
    }
  }
  
  return;
}

int onCommandExit(const vector<string>& args)
{
  return -1;
}

int onCommandHelp(const vector<string>& args)
{
  return 0;
}

int onCommandCmdnm(const vector<string>& args)
{
  return 0;
}

int onCommandPid(const vector<string>& args)
{
  return 0;
}

int onCommandSystat(const vector<string>& args)
{
  return 0;
}



