/* CSC456 Prog 1 - "dash"
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

using namespace std;

int onCommand(string command);

int main()
{
  cout << "Welcome to dash. Written by Daniel Andrus\n"
          "Type \"help\" for command help." << endl;
  
  string input;
  int status;
  bool exit = false;
  
  // Enter main program loop
  while (!exit)
  {
    // Output prompt
    cout << "dash>";
    
    // Input user command
    getline(cin, input);
    
    // Send command to parsing function
    status = onCommand(input);
    
    switch (status)
    {
    case 1:
      exit = true;
      break;
      
    default:
      break;
    }
  }
  
  cout << "Goodbye!" << endl;
  
  return 0;
}

int onCommand(string command)
{
  if (command == "exit")
  {
    return 1;
  }
  else if (command == "")
  {
    return 0;
  }
  else
  {
    cout << "Unknown command \"" << command << "\"" << endl;
    return 0;
  }
}

