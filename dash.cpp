/* CSC456 Prog 1 - "Dash"
 * Author:
 *   Daniel Andrus
 * 
 * Description:
 * Command interface for viewing and managing system processes. Includes a
 * special easter egg!
 * 
 * Links
 * - http://cc.byexamples.com/2008/06/16/gnu-readline-implement-custom-auto-complete/
 * - http://www.mcs.sdsmt.edu/ckarlsso/csc456/spring15/homeworks/csc456_HW1.pdf
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

using namespace std;

void commandPreprocess(const string& command, vector<string>& args);
int onCommand(const vector<string>& args);
int onCommandExit(const vector<string>& args);
int onCommandHelp(const vector<string>& args);

#include "commands.h"

/**
 * Main
 * Author: Daniel Andrus
 *
 * Entry point for the program. Contains the main program loop.
 *
 * Returns - 0
**/
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
    cout << "dash> ";
    
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


/**
 * onCommand
 * Author: Daniel Andrus
 *
 * Takes a command and acts as a command switchboard, forwarding the arguments
 * to their appropriate handling functions.
 *
 * args - vector of strings representing command arguments.
 *
 * Returns - success code, 0 if successful, -1 to exit the program.
**/
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
  else if (args.size() > 2
          && args[0] == "FUS"
          && args[1] == "RO"
          && args[2] == "DAH")
  {
    return onDragonborn(args);
  }
  else
  {
    cout << "Unknown command \"" << args[0] << "\"" << endl;
  }
  
  // Return 0 to continue as normal
  return 0;
}


/**
 * commandPreprocess
 * Author: Daniel Andrus
 *
 * Preprocesses commands, splitting the raw strings by spaces and putting them
 * into a vector.
 *
 * command - the raw command string read in from console
 * args - vector of strings that will contain the output of this function
**/
void commandPreprocess(const string& command, vector<string>& args)
{
  args.clear();
  string tmp = "";
  
  // Split command into subcommands
  for (int i = 0; i <= command.size(); i++)
  {
    if (command[i] == ' ' || command[i] == '\t' 
        || command[i] == '\n' || i == command.size())
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


/**
 * onCommandExit
 * Author: Daniel Andrus
 *
 * Handles the "exit" command. Returns the program's exit code.
 *
 * args - string vector of command arguments
 *
 * Returns - -1, the program's exit code.
**/
int onCommandExit(const vector<string>& args)
{
  return -1;
}


/**
 * onCommandHelp
 * Author: Daniel Andrus
 *
 * Handles the "help" command. Displays help information to the user.
 *
 * args - string vector of command arguments
 *
 * Returns - 0
**/
int onCommandHelp(const vector<string>& args)
{
  cout << "Available commands:\n"
          "\n"
          "cmdnm <pid>\n"
          "  Get the command string that started the process for a given "
          "  process ID,\nwhere <pid> is the process ID.\n"
          "\n"
          "pid <command>\n"
          "  Get the process IDs for a given command string. Returns PIDs of "
          "  all partially\nmatching processes.\n"
          "\n"
          "systat\n"
          "  Get system information, such as version number, uptime, memory "
          "  usage, and cpu\ninformation.\n"
          "\n"
          "exit\n"
          "  Exit Dash.\n"
          "\n"
          "FUS RO DAH\n"
          "  A seranade for the hero of Tamriel."<< endl;
  
  return 0;
}
