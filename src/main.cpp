#include <iostream>
#include <sstream>
#include "AVL.h"
using namespace std;
// Extracts the Name within Quotes
string extractName(istringstream& in)
{
    if (in.peek() != '\"')
        return "";
    string name;
    getline(in, name,'\"');
    getline(in, name,'\"');
    regex validWord = regex("^[a-zA-Z0-9\\s]{1,1000}$");
    if (!regex_match(name, validWord))
        return "";
    return name;
}
// Only verifies if the input is alphanumerical,
// insert method ensures it adheres to specifications
string verifyNextWord(istringstream& in)
{
    string word;
    if(!(in >> word))
        word = "";
    // Verifies if it has a-z,A-Z. 0-9, and if it is length 10000
    regex validWord = regex("^[a-zA-Z0-9_ ]{1,1000}$");
    if (!regex_match(word, validWord))
        return "";
    return word;
}
void TreeCreation(string& line, int& commandCount)
{
    AVL tree;
    for (int i = 0; i < commandCount; i++)
    {
        // Line will be the current command
        getline(cin, line);
        istringstream cmdLine(line);

        string command; // Token for each part of the command
        // First Token should be instruction
        getline(cmdLine,command, ' ');
        if (command == "insert")
        {
            string name, ID;
            name = extractName(cmdLine);
            ID = verifyNextWord(cmdLine);
            if (name == "" || ID == "")
                // Results in unsuccessful if it doesnt have either a name or ID
                tree.insert("", "");
            else
                tree.insert(name, ID);
        }
        else if (command == "remove")
        {
            string ID = verifyNextWord(cmdLine);
            tree.remove(ID);
        }
        else if (command == "search")
        {
            char checker = '\"';
            string target;
            // Target Has a " Separator, at least in the beginning
            // If it only has it in the beginning, extract name will return blank string
            if(cmdLine.peek() == checker)
                target = extractName(cmdLine);
            else
                target = verifyNextWord(cmdLine);
            tree.search(target);
        }
        else if (command == "printInorder")
        {
            auto nodes = tree.inorder();
            tree.print_order(nodes);
        }
        else if (command == "printPreorder")
        {
            auto nodes = tree.preorder();
            tree.print_order(nodes);
        }
        else if (command == "printPostorder")
        {
            auto nodes = tree.postorder();
            tree.print_order(nodes);
        }
        else if (command == "printLevelCount")
        {
            tree.printLevelCount();
        }
        else if (command == "removeInorder")
        {
            int index;
            // Checks to see if it inputted the index correctly
            if (cmdLine >> index)
                tree.removeInorder(index);
            else
                tree.removeInorder(-1);
        }
        else
            tree.insert("",""); // unsuccessful


    }
    tree.printOutput();
}
int main()
{

    string line; getline(cin, line);
    istringstream in(line);
    // Gets Number of Commands
    int commandCount; in >> commandCount;
    TreeCreation(line, commandCount);

    return 0;
}
