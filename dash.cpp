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
#include <signal.h>

using namespace std;

void onSignalReceive(int signal);
int getCommand(ostream& out, istream& in, vector<string>& args);
int onCommand(const vector<string>& args);
int onCommandExit(const vector<string>& args);
int onCommandHelp(const vector<string>& args);

#include "commands.h"
#include "commands2.h"


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
  
  // Register signal handler
  for (int i = 1; i <= 64; i++)
  {
    signal(i, onSignalReceive);
  }
  
  // Enter main program loop
  while (!exit)
  {
    // Read in command
    args.clear();
    getCommand(cout, cin, args);
    
    // Send command to parsing function
    status = onCommand(args);
    
    switch (status)
    {
    case -2:        // kill program
    case -1:        // exit program
      exit = true;
      break;
      
    default:        // error
      cout << "\nAn error occured while executing command\n"
              "  " << input << endl;
      break;
      
    case 0:
      break;        // continue as normal
    }
  }
  
  if (status != -2)
    cout << "Goodbye!" << endl;
  
  return 0;
}


void onSignalReceive(int signal)
{
  cout << "\nReceived signal " << signal << "\ndash> ";
  cout.flush();
}


int getCommand(ostream& out, istream& in, vector<string>& args)
{
  string input;
  string temp;
  int quot;
  int bksl;
  
  // display prompt
  out << "dash> ";
  
  // initialize values
  quot = 0;
  temp = "";
  
  do
  {
    // Reset backslash flag
    bksl = 0;
    
    // Input user command
    getline(in, input);
  
    // Parse command
    for (int i = 0; i < input.size(); i++)
    {
      // If previous character was a backslash, blindly add current char to args
      if (bksl)
      {
        bksl = false;
        temp += input[i];
      }

      // Otherwise, if whitespace (and not in quotes), split command
      else if (isspace(input[i]) && quot == 0)
      {
        if (temp != "")
        {
          args.push_back(temp);
          temp = "";
        }
      }

      // Otherwise, act depending on current cahr
      else
      {
        switch (input[i])
        {
        case '\\':
          bksl = true;
          break;

        case '"':
          // Toggle double-quote mode
          if (quot == '"')
          {
            quot = 0;
          }
          else if (quot == 0)
          {
            quot = '"';
          }
          else
          {
            // Quote mode is activated for other quote type
            temp += input[i];
          }
          break;

        case '\'':
          // Toggle single-quote mode
          if (quot == '\'')
          {
            quot = 0;
          }
          else if (quot == 0)
          {
            quot = '\'';
          }
          else
          {
            // Quote mode is activated for other quote type
            temp += input[i];
          }
          break;

        default:
          temp += input[i];
        }
      }
    }
    
    // Add newline to argument if quoted
    if (quot != 0 && bksl == false)
      temp += '\n';
    
    // Display prompt
    if (bksl == true || quot != 0)
      out << "dash> ";
  }
  while (bksl == true || quot != 0);
  
  // Push remaining text into args
  if (temp != "")
    args.push_back(temp);
  
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
  else if (args[0] == "cd")
  {
    return onCommandCd(args); 
  }
  else if (args[0] == "signal")
  {
    return onCommandSignal(args);
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
    return onCommandUnknown(args);
  }
  
  // Return 0 to continue as normal
  return 0;
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
