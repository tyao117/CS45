#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

bool getInput(string &line);
void processLine(string line);
void convert(size_t numBits, size_t value, vector<bool> &bits);
void display(const vector<bool> &bits);

int main()
{
    string line;
    while(getInput(line))
        processLine(line);
    vector<bool>bits(4);
    for(size_t i = 0; i < 16; ++i)
    {
        convert(4, i, bits);
        display(bits);
    }
    cout<<"hi"<<endl;

    return 0;
}

bool getInput(string &line)\
{
    cout<<"Input: ";
    getline(cin,line);
    return line.size();
}

bool processLine(string line)
{
    vector<char> operators;
    vector<char> operands;
    static string precedence("~()&|@%^");
    static string special("~()");
    string op;
    string line1 = line;
    string line2 = line;
    int first, second, pos1, pos2;
    first = precedence.find_first_of(line);                                 //looks at which operator
    pos1 = line.find_first_of(precedence[first]);                           //looks at which position of first
    line2.replace(pos1,1," ");                                              //replace the operator
    second = precedence.find_first_of(line2);                               //looks at which operator
    pos2 = line2.find_first_of(precedence[second]);                         //looks at which position of second
    op = first <= second ? "first" : "second";
    cout<<"The "<<op<<" operator has the higher precedence"<<endl;
    cout<<"first="<<pos1<<" "<<"second="<<pos2<<endl;
    cout<<"operands="<<endl;
    for (int i = operands.size(); i>=0; --i)
    {
            cout<<operands[i]<<endl;
    }
    cout<<"operators="<<endl;
    for (int i = operators.size(); i>=0; --i)
    {
            cout<<operators[i]<<endl;
    }
}

void convert(size_t numBits, size_t value, vector<bool> &bits)
{
    size_t i = numBits - 1;
    for(size_t j = 0; j < numBits; ++j)
        bits[j] = 0;
    for(size_t j = 0; j < numBits; ++j)
    {
        bits[i--] = value%2;
        value /= 2;
    }
}

void display(const vector<bool>&bits)
{
    for(int i = 3; i>-1; i--)//has to be int because size_t is unsigned
    {
        cout<<bits[i];
    }
    cout<<endl;
}
