#include <iostream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <bitset>
#include <array>
#include <vector>
#include <cstring>

using namespace std;

void help();

int main()
{
    string cmd, line;
    int cmdNum;
    string cmds[12] = {"NEW","IS","EDIT","DELETE","TABLE","LOAD","STORE","LIST",
                       "HELP","QUIT","WQUIT","NEWTABLE"};
    vector<string> orig_exp; //original expressions
    vector<string> rev_polish[10][10]; //Boolean Reverse Polish (RPN) format
    string precedence("()~&|@%^");

    bool saved;

    while(cin>>cmd)
    {
        cmdNum=-1;
        if (cmd=="EXIT")
            cmd="QUIT";
        if (cmd=="WEXIT")
            cmd="WQUIT";
        for (int i=0; i<sizeof(cmds); ++i)
            if (cmd==cmds[i])
                cmdNum=i;
        getline(cin,line);
        switch(cmdNum)
        {
            case 0: //NEW expression - replies "EXPRESSION x" (x=index+1 of vector)
                break;
            case 1: //IS x = y? - if (not) identical: expression x "is (not) equivalent to" expression y
                break;
            case 2: //EDIT x - ask for new expression and replace
                break;
            case 3: //DELETE x
                break;
            case 4: //TABLE x
                break;
            case 5: //LOAD filename(.truth)
                break;
            case 6: //STORE filename(.truth) - overwrite or new name
                break;
            case 7: //LIST - display all expressions
                break;
            case 8: //HELP
                break;
            case 9: //QUIT - ask to save iff they have not saved right before command
                break;
            case 10: //WQUIT filename(.truth) - overwrite to file and exit
                break;
            case 11: //NEWTABLE - user enters table + program determines expression & declares storing it (EC!)
                break;
            default:
                cout<<"Cannot read command. Please try again.\n\n";
                help();
        }
    }

    return 0;
}

bool conjunction(bool x, bool y) // &
{

    return 0;
}

bool disjunction(bool x, bool y) // |
{
    return 0;
}

bool knot(bool x) // ~
{
    return 0;
}

bool nand(bool x, bool y) // @
{
    return 0;
}

bool nor(bool x, bool y) // %
{
    return 0;
}

bool exor(bool x, bool y) // ^
{
    return 0;
}

bool implication(bool x, bool y) // =>
{
    return 0;
}

bool biimplication(bool x, bool y) // <=>
{
    return 0;
}

void truthTable(int index)
{
    //Outputs truth table of expression
}

bool remove(int index)
{
    //Removes expression at index
    return 0;
}

void list(vector<string> expressions)
{
    //Outputs list of expressions
}

void help()
{
    cout<<"PROGRAM COMMANDS\n"
        <<"NEW (expression) - Enter a new expression into the program.\n"
        <<"IS x = y? - Check if expression[x] = expression[y] (x & y are indexes).\n"
        <<"EDIT x - Edit an expression at index x\n."
        <<"DELETE x - Delete an expression at index x\n."
        <<"TABLE x - Display the truth table of expression at index x.\n"
        <<"LOAD filename(.truth) - Load expressions from file.\n"
        <<"STORE filename(.truth) - Store expressions to file.\n"
        <<"LIST - Display list of all expressions.\n"
        <<"HELP - Display list of all program commands.\n"
        <<"QUIT/EXIT - Exits the program.\n"
        <<"WQUIT/WEXIT filename(.truth) - Saves expressions to file and exits program.\n"
        <<"(EC!)NEWTABLE - Enter contents of a truth table for program to determine expression\n";
}

/*
2. The user of the program need not necessarily have variables that "run in sequence;" meaning
   A, B, C are good input, A, Q, Z is also good input. (HINT: A boolean array of 26 elements
   can be indexed by (int)(x - 65) where x is a character from A to Z. (Note: this implies that
   all input must be converted to upper case!)
20. Algorithm to Convert Boolean Algebra to RPN:
    Edsger Dijkstra (Links to an external site.), one of the pilars of Computer Science, invented the
    Shunting-yard algorithm (Links to an external site.) to convert infix expressions to postfix (RPN),
    so named because its operation resembles that of a railroad shunting yard. The link above is
    crucial to solving this problem.
    In order to implement the Shunting-yard algorithm, you will be needing  to use the stack header
    file.

Example:

Suppose the following were to be entered as the inputs for each row of a two-variable truth table:

FF F
FT T
TF T
TT F

The program would then respond with ~A&B | A&~B as the equation that would have this  Truth Table
*/
