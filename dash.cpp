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
int getCommand(ostream& out, istream& in, vector<vector<string> >& args,
              string& instream, string& outstream);
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
  string infile;
  string outfile;
  vector<vector<string> > args;
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
    infile = "";
    outfile = "";
    status = getCommand(cout, cin, args, infile, outfile);
    
    // Validate piping
    if (!status)
    {
      for (int i = 0; i < args.size(); i++)
      {
        if (args[i].size() == 0)
        {
          status = -2;
          break;
        }
      }
    }
    
    // Check for command errors
    if (status)
    {
      cerr << "-dash: ";
      
      switch(status)
      {
      case -1:
        cerr << "syntax error near unexpected token 'newline'";
        break;
        
      case -2:
        cerr << "syntax error near unexpected token '|'";
        break;
      }
      
      cerr << endl;
      exit = 0;
      continue;
    }
    
    // Validate input redirection
    if (infile != "")
    {
      ifstream fin(infile.c_str());
      if (!fin)
      {
        cerr << "-dash: " << infile << ": Unable to open file" << endl;
        exit = 0;
        continue;
      }
      else
      {
        fin.close();
      }
    }
    
    // Validate output redirection
    if (outfile != "")
    {
      ofstream fout(outfile.c_str());
      if (!fout)
      {
        cerr << "-dash: " << outfile << ": Unable to open file" << endl;
        exit = 0;
        continue;
      }
      else
      {
        fout.close();
      }
    }
    
    // Handle piping if necessary
    if (args.size() > 1)
    {
      int arg = 0;
      int pid;
      int** pipes;
      int pipefd[2];
      
      // Allocate arrays for pipes
      pipes = new int* [args.size()-1];
      for (arg = 0; arg < args.size()-1; arg++)
        pipes[arg] = new int [2];
      
      // Attempt to open pipes
      for (arg = 0; arg < args.size() - 1; arg++)
      {
        if (pipe(pipes[arg]) != 0)
        {
          cerr << "-dash: ";
          
          switch(errno)
          {
          case EFAULT:
            cerr << "pipefd is not valid";
            break;
            
          case EMFILE:
            cerr << "too many file descriptors are in use by process";
            break;
            
          case ENFILE:
            cerr << "system limit reached";
            break;
          }
          
          cerr << endl;
          arg = -1;
          break;
        }
      }
      
      // Check if pipe opening failed
      if (arg == -1)
      {
        status = 0;
        continue;
      }
      
      arg = 0;
      pid = fork();
      
      if (pid == 0)                     // child
      {
        // Fork processes!
        for (arg = 0; arg < args.size(); arg++)
        {
          // Fork if necessary
          if (arg < args.size()-1)
          {
            pid = fork();
          }
          else
          {
            pid = 1;
          }
          
          if ( pid == 0 )
          {
            continue;
          }
          else
          {
            // Redirect standard output
            if (arg < args.size()-1)
            {
              close(1);             // close output
              dup(pipes[arg][1]);   // redirect output
              close(pipes[arg][0]);
              close(pipes[arg][1]);
            }
            
            // Redirect standard input
            if (arg > 0)
            {
              close(0); // close input
              dup(pipes[arg-1][0]);
              close(pipes[arg-1][0]);
              close(pipes[arg-1][1]);
            }
            
            onCommand(args[arg]);
            
            if (arg < args.size() - 1)
              while (wait(&status) != pid);
            
            break;
          }
        }
        
        status = -2;
      }
      else                              // parent
      {
        while (wait(&status) != pid);
        status = 0;
      }
      
    }
    
    // Single command
    else
    {
      // Send command to parsing function
      status = onCommand(args[0]);
    }

    switch (status)
    {
    case -2:        // kill program
    case -1:        // exit program
      exit = true;
      break;
      
    default:        // error
      cerr << "\nAn error occured while executing command\n"
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
  
  if (signal == 2) exit(1);
}


int getCommand(ostream& out, istream& in, vector<vector<string> >& args,
               string& infile, string& outfile)
{
  string input;
  string temp;
  int quot;
  int bksl;
  int pipe;
  int arg;
  
  // display prompt
  out << "dash> ";
  
  // initialize values
  quot = 0;
  pipe = 0;
  temp = "";
  arg = args.size();
  args.push_back(vector<string>());
  
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
        pipe = 0;
        temp += input[i];
      }

      // Otherwise, if whitespace (and not in quotes), split command
      else if ((isspace(input[i]) || input[i] == '|') && quot == 0)
      {
        if (temp != "")
        {
          args[arg].push_back(temp);
          temp = "";
        }
        if (input[i] == '|')
        {
          pipe = 1;
          arg++;
          args.push_back(vector<string>());
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
          pipe = 0;
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
          pipe = 0;
          break;
          
        default:
          pipe = 0;
          temp += input[i];
          break;
        }
      }
    }
    
    // Add newline to argument if quoted
    if (quot != 0 && bksl == false)
      temp += '\n';
    
    // Display prompt
    if (bksl == true || quot != 0 || pipe != 0)
      out << "dash> ";
  }
  while (bksl == true || quot != 0 || pipe != 0);
  
  // Push remaining text into args
  if (temp != "")
    args[arg].push_back(temp);
  
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
