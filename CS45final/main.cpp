#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

typedef char byte;

bool getInput(string &line);
void trim(string &line);
bool parseNumber(const string &line, vector<byte> &bigNum);
bool parseOperator(const string &line, char &op);
void processLine(const string &line, vector<byte> memory[], vector<byte> memoryRemainder[]);
void performOperation(vector<byte> bigNum, const char &op, vector<byte> bigNum2, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder);
void add(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns);
void subtract(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns);
void multiply(const vector<byte> &bigNum, const vector<byte> &bigNum2, vector<byte> &bigNumAns);
void divide(const vector<byte> &bigNum, const vector<byte> &bigNum2, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder);
void incrementUp(vector<byte> &bigNumAns);
bool lessThan(const vector<byte> &bigNum, const vector<byte> &bigNum2);
bool greaterThan(const vector<byte> &bigNum, const vector<byte> &bigNum2);
bool equals(const vector<byte> &bigNum, const vector<byte> &bigNum2);
bool isNegative(const vector<byte> &bigNum);
void displayAllMemory(const vector<byte> memory[], const vector<byte> memoryRemainder[]);
void outputNumber(ostream &out, const vector<byte> &bigNum);
bool loadFromFile(const string &fileName, vector<byte> memory[], vector<byte> memoryRemainder[]);
void saveToFile(const string &fileName, const vector<byte> memory[], const vector<byte> memoryRemainder[]);
void clearMemories(vector<byte> memory[], vector<byte> memoryRemainder[]);
void help();

int main()
{
    vector<byte> memory[26];
    vector<byte> memoryRemainder[26];
    string line;
    clearMemories(memory, memoryRemainder);
    while(getInput(line))
    {
//        trim(line);
        processLine(line, memory, memoryRemainder);
    }
    return 0;
}

bool getInput(string &line)
{
    cout << "Expression: ";
    getline(cin, line);
    if(line == "")
    {
        return false;
    }
    for(size_t i = 0; i < line.find_first_of(" ") && i < line.size(); ++i)
    {
        line[i] = toupper(line[i]);
    }
    return true;
}

//void trim(string &line)
//{
//    while(line[0] == ' ')
//    {
//        line.erase(0, 1);
//    }
//    while(line[line.size() - 1] == ' ')
//    {
//        line.erase(line.end());
//    }
//    for(int i = 0; i < line.find_first_of(" "); ++i)
//    {
//        line[i] = toupper(line[i]);
//    }
//}

void processLine(const string &line, vector<byte> memory[], vector<byte> memoryRemainder[])
{
    vector<byte> bigNum;
    vector<byte> bigNum2;
    vector<byte> bigNumAns;
    char op;
    stringstream ss;
    string action;
    ss << line;
    ss >> action;
    cout << action << endl;
    if(action == "LET")
    {
        char variableStore;
        string parse;
        ss >> variableStore;
        variableStore =  toupper(variableStore);
        if(ss.str().find(" = "))
        {
            ss.ignore(3);
            for(unsigned int i = 0; i < 3; ++i)
            {
                if(ss.eof())
                {
                    cout << "Invalid expression.\n";
                    return;
                }
                ss >> parse;
                switch(i)
                {
                case 0:
                {
                    parseNumber(parse, bigNum);
                    break;
                }
                case 1:
                {
                    parseOperator(parse, op);
                    break;
                }
                case 2:
                {
                    parseNumber(parse, bigNum2);
                    break;
                }
                }
            }
            outputNumber(cout, bigNum);
            cout << endl;
            cout << op << endl;
            outputNumber(cout, bigNum2);
            cout << endl;
            performOperation(bigNum, op, bigNum2, memory[variableStore - 'A'], memoryRemainder[variableStore - 'A']);
            cout << "done\n";
        }
        else
        {
            cout << "Invalid expression.\n";
            return;
        }
        return;
    }
    if(action == "SHOW")
    {
        char memLocation;
        ss >> memLocation;
        if(isalpha(memLocation))
        {
            outputNumber(cout, memory[toupper(memLocation) - 'A']);
            cout << " Remainder: ";
            outputNumber(cout, memoryRemainder[toupper(memLocation) - 'A']);
            cout << endl;
        }
        else
        {
            cout << "Invalid location\n";
        }
        return;
    }
    if(action == "LIST")
    {
        displayAllMemory(memory, memoryRemainder);
        return;
    }
    if(action == "LOAD")
    {
        string fileName;
        ss >> ws;
        getline(ss, fileName);
        loadFromFile(fileName, memory, memoryRemainder);
        return;
    }
    if(action == "SAVE")
    {
        string fileName;
        ss >> ws;
        getline(ss, fileName);
        saveToFile(fileName, memory, memoryRemainder);
        return;
    }
    if(action == "EDIT")
    {
        return;
    }
    if(action == "EXIT" || action == "QUIT")
    {
        return;
    }
    if(action == "WEXIT" || action == "WQUIT")
    {
        return;
    }
    else
    {
        help();
    }
//    parseLine(line, bigNum) && parseLine(line, op) && parseLine(line, bigNum2);
}

void performOperation(vector<byte> bigNum, const char &op, vector<byte> bigNum2, vector<byte> &bigNumAns, vector<byte> &binNumAnsRemainder)
{
    switch(op)
    {
    case '+':
    {
        add(bigNum, bigNum2, bigNumAns);
        break;
    }
    case '-':
    {
        subtract(bigNum, bigNum2, bigNumAns);
        break;
    }
    case '*':
    {
        multiply(bigNum, bigNum2, bigNumAns);
        break;
    }
    case '/':
    {
        divide(bigNum, bigNum2, bigNumAns, binNumAnsRemainder);
        break;
    }
    default:
    {
        cout << "Unknown operator.\n";
    }
    }
}

bool parseNumber(const string &line, vector<byte> &bigNum)
{
    for(unsigned int i = line.length(); i > 0;)
    {
        if(!isdigit(line[--i]))
        {
            return false;
        }
        bigNum.push_back(line[i] - '0');
    }
    return true;
}

bool parseOperator(const string &line, char &op)
{
    if(line.length() == 1)
    {
        op = line[0];
        return true;
    }
    return false;
}

void add(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns)
{
    bigNumAns.resize(0);
    byte carry = 0;
    unsigned int minSize = min(bigNum.size(), bigNum2.size());
    for(unsigned int i = 0; i < minSize; ++i)
    {
        bigNumAns.push_back((bigNum[i] + bigNum2[i] + carry) % 10);
        carry = (bigNum[i] + bigNum2[i]) / 10;
    }
    for(unsigned int i = minSize; i < bigNum.size(); ++i)
    {
        bigNumAns.push_back((bigNum[i] + carry) % 10);
        carry = (bigNum[i] + carry) / 10;
    }
    for(unsigned int i = minSize; i < bigNum2.size(); ++i)
    {
        bigNumAns.push_back((bigNum2[i] + carry) % 10);
        carry = (bigNum2[i] + carry) / 10;
    }
}

void subtract(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns)
{
    bigNumAns.resize(0);
    byte carry = 0;
    bool negative = false;
    unsigned int minSize = min(bigNum.size(), bigNum2.size());
    if(lessThan(bigNum, bigNum2))
    {
        negative = true;
    }
    for(unsigned int i = 0; i < minSize; ++i)
    {
        bigNum2[i] += carry--;
        if(bigNum[i] < bigNum2[i])
        {
            bigNum[i] += 10;
            carry = 1;
        }
        bigNumAns.push_back((bigNum[i] - bigNum2[i]));
    }
    for(unsigned int i = minSize; i < bigNum.size(); ++i)
    {
        if(carry)
        {
            bigNumAns.push_back(bigNum[i] - carry--);
        }
        else
        {
            bigNumAns.push_back(bigNum[i]);
        }
    }
    if(negative)
    {
        bigNumAns[bigNumAns.size() - 1] *= -1;
    }
}

void multiply(const vector<byte> &bigNum, const vector<byte> &bigNum2, vector<byte> &bigNumAns)
{
    bigNumAns.resize(0);
    byte carry = 0;
    byte total = 0;
    for(size_t i = 0; i < bigNum2.size(); ++i)
    {
        for(size_t j = min(bigNum.size() - 1, i); j-- > 0;)
        {
            total = bigNum[i] * bigNum2[j];
        }
        total = total + bigNum[0] * bigNum2[i] + carry;
        carry = total / 10;
        bigNumAns.push_back(total % 10);
    }
    for(unsigned int i = 1, last = bigNum2.size() - 1; i < bigNum.size() - 1; ++i)
    {
        total = bigNum2[last] * bigNum[i] + carry % 10;
        carry = total / 10;
        bigNumAns.push_back(total);
    }
    if(carry)
    {
        bigNumAns.push_back(carry);
    }
}

void divide(const vector<byte> &bigNum, const vector<byte> &bigNum2, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder)
{
    bigNumAns.resize(1);
    bigNumAns[0] = 0;
    bigNumAnsRemainder.resize(0);
    int negative = 1;
    if(isNegative(bigNum))
    {
        negative *= -1;
    }
    if(isNegative(bigNum2))
    {
        negative *= -1;
    }
    bigNumAnsRemainder = bigNum;
    while(greaterThan(bigNumAnsRemainder, bigNum2) || equals(bigNumAnsRemainder, bigNum2))
    {
        subtract(bigNumAnsRemainder, bigNum2, bigNumAnsRemainder);
        outputNumber(cout, bigNumAnsRemainder);
        incrementUp(bigNumAns);
    }
    bigNumAns[bigNumAns.size() - 1] *= negative;
}

void incrementUp(vector<byte> &bigNumAns)
{
    if(!bigNumAns.size())
    {
        bigNumAns.push_back(0);
    }
    for(unsigned int i = 0; !(++bigNumAns[i] %= 10); ++i);
}

bool lessThan(const vector<byte> &bigNum, const vector<byte> &bigNum2)
{
    if(bigNum.size() == bigNum2.size())
    {
        for(unsigned int i = bigNum.size(); i > 0;)
        {
            if(bigNum[--i] == bigNum2[i])
            {
                continue;
            }
            return bigNum[i] < bigNum2[i];
        }
    }
    return bigNum.size() < bigNum2.size();
}

bool greaterThan(const vector<byte> &bigNum, const vector<byte> &bigNum2)
{
    if(bigNum.size() == bigNum2.size())
    {
        for(unsigned int i = bigNum.size(); i > 0;)
        {
            if(bigNum[--i] == bigNum2[i])
            {
                continue;
            }
            return bigNum[i] > bigNum2[i];
        }
    }
    return bigNum.size() > bigNum2.size();
}

bool equals(const vector<byte> &bigNum, const vector<byte> &bigNum2)
{
    if(bigNum.size() == bigNum2.size())
    {
        for(unsigned int i = bigNum.size(); i > 0;)
        {
            if(bigNum[--i] != bigNum2[i])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool isNegative(const vector<byte> &bigNum)
{
    return bigNum[bigNum.size() - 1] < 0;
}

void displayAllMemory(const vector<byte> memory[], const vector<byte> memoryRemainder[])
{
    for(int i = 0; i < 26; ++i)
    {
        cout << char(i+65) << " = ";
        outputNumber(cout, memory[i]);
        cout << " Remainder: ";
        outputNumber(cout, memoryRemainder[i]);
        cout << endl;
    }
}

void outputNumber(ostream &out, const vector<byte> &bigNum)
{
//    if(bigNum.size())
//    {
        for(int i = bigNum.size(); i > 0;)
        {
            out << (short int)bigNum[--i];
            if(!(i % 3) && i)
            {
                out << ",";
            }
        }
//    }
//    else
//    {
//        cout << "Nothing is here";
//    }
}

bool loadFromFile(const string &fileName, vector<byte> memory[], vector<byte> memoryRemainder[])
{
    clearMemories(memory, memoryRemainder);
    ifstream infile;
    string number;
    size_t remainderPos;
    infile.open(fileName.c_str());
    if(infile.fail())
    {
        cout << "Unable to find the file\n";
        return false;
    }
    for(int i = 0; !infile.eof() && i < 26; ++i)
    {
        getline(infile, number);
        remainderPos = number.find_first_of(" ");
        parseNumber(number.substr(0, remainderPos), memory[i]);
        if(remainderPos < string::npos)
        {
            parseNumber(number.substr(remainderPos), memoryRemainder[i]);
        }
    }
    infile.close();
    return true;
}

void saveToFile(const string &fileName, const vector<byte> memory[], const vector<byte> memoryRemainder[])
{
    ofstream outfile;
    outfile.open(fileName.c_str());
    for(int i = 0; i < 26; ++i)
    {
        outputNumber(outfile, memory[i]);
        outfile << " ";
        outputNumber(outfile, memoryRemainder[i]);
    }
    outfile.close();
    return;
}

void clearMemories(vector<byte> memory[], vector<byte> memoryRemainder[])
{
    for(unsigned int i = 0; i < 26; ++i)
    {
        memory[i].resize(1);
        memory[i][0] = 0;
        memoryRemainder[i].resize(1);
        memoryRemainder[i][0] = 0;
    }
}

void help()
{
    cout << "Help!!!\n";
    cout <<"List of actions:\n"
         <<endl;
}
