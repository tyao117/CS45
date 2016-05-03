
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
bool parseAndPerform(string line, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder, const string memory[], int memToRetrieveFrom);
bool parseNumber(const string &line, vector<byte> &bigNum, vector<byte> &bigNumRemainder, const string memory[], int memToRetrieveFrom);
bool parseOperator(const string &line, char &op);
void processLine(const string &line, string memory[], vector<byte> memoryRemainder[], bool &save);
bool performOperation(vector<byte> bigNum, vector<byte> bigNumRemainder, const char &op, vector<byte> bigNum2, vector<byte> bigNum2Remainder, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder);
void add(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns);
void subtract(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns);
void multiply(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns);
bool divide(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder);
void incrementUp(vector<byte> &bigNumAns);
bool lessThan(const vector<byte> &bigNum, const vector<byte> &bigNum2);
bool greaterThan(const vector<byte> &bigNum, const vector<byte> &bigNum2);
bool greaterThanEquals(const vector<byte> &bigNum, const vector<byte> &bigNum2);
bool equals(const vector<byte> &bigNum, const vector<byte> &bigNum2);
bool isNegative(const vector<byte> &bigNum);
void removeLeadingZero(vector<byte> &bigNumAns);
void factorial(vector<byte> bigNum, vector<byte> &bigNumAns);
void decrement(vector<byte> &bigNumAns);
void combinations(const vector<byte> &bigNum, const vector<byte> &bigNum2, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder);
void permutations(const vector<byte> &bigNum, const vector<byte> &bigNum2, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder);
void gcd(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns);
void displayAllMemory(const string memory[]);
void outputNumber(ostream &out, const vector<byte> &bigNum);
bool loadFromFile(const string &fileName, string memory[]);
void saveToFile(const string &fileName, const string memory[]);
void clearMemories(string memory[]);
void help();

int main()
{
//    vector<byte> memory[26]; //change this into an array of string
    vector<byte> memoryRemainder[26]; //change this into an array of string
    string memory[26];
    string line;
    bool saved = true;
    clearMemories(memory);
    while(true)
    {
        getInput(line);
        if(!line.empty())
        {
            processLine(line, memory, memoryRemainder, saved);
        }
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

void processLine(const string &line, string memory[], vector<byte> memoryRemainder[], bool &saved)
{
    vector<byte> bigNumAns;
    vector<byte> bigNumAnsRemainder = vector<byte>(1, 0);
    stringstream ss;
    string action;
    int recursionCounter = 0;
    if(parseAndPerform(line, bigNumAns, bigNumAnsRemainder, memory, recursionCounter))
    {
        cout << "Output: ";
        outputNumber(cout, bigNumAns);
        cout << " remainder: ";
        outputNumber(cout, bigNumAnsRemainder);
        cout << endl;
        return;
    }
    ss << line;
    ss >> action;
    if(action == "LET")
    {
        char variableStore;
        string parse;
        size_t equalSignPos;
        ss >> variableStore;
        if(!isalpha(variableStore))
        {
            cout<<"There was no letter"<<endl;
            return;
        }
        variableStore =  toupper(variableStore);
        if((equalSignPos = ss.str().find(" = ")) != string::npos)
        {
            ss.ignore(2);
            getline(ss, parse);
            if(!memory[variableStore - 'A'].empty())
            {
                cout << "Memory already occupied. In order to edit or erase use EDIT.\n";
                return;
            }
            else if(parseAndPerform(parse, bigNumAns, bigNumAnsRemainder, memory, recursionCounter))
            {
                memory[variableStore - 'A'] = parse;
                cout << "Output: ";
                outputNumber(cout, bigNumAns);
                cout << " remainder: ";
                outputNumber(cout, bigNumAnsRemainder);
                cout << endl;
                saved = false;
                return;
            }
        }
        cout << "Invalid expression.\n";
        return;
    }
    if(action == "SHOW")
    {
        char memLocation;
        ss >> memLocation;
        if(isalpha(memLocation))
        {
            if(parseAndPerform(memory[toupper(memLocation) - 'A'], bigNumAns, bigNumAnsRemainder, memory, recursionCounter))
            {
                cout << "Output: ";
                outputNumber(cout, bigNumAns);
                cout << " Remainder: ";
                outputNumber(cout, bigNumAnsRemainder);
                cout << endl;
            }
        }
        else
        {
            cout << "Invalid location\n";
        }
        return;
    }
    if(action == "LIST")
    {
        displayAllMemory(memory);
        return;
    }
    if(action == "LOAD")
    {
        string fileName;
        ss >> ws;
        getline(ss, fileName);
        loadFromFile(fileName, memory);
        return;
    }
    if(action == "SAVE")
    {
        string fileName;
        ss >> ws;
        getline(ss, fileName);
        saveToFile(fileName, memory);
        saved = true;
        return;
    }
    if(action == "EDIT")
    {
        string newExpression;
        char memoryToEdit;
        ss >> memoryToEdit;
        if(!isalpha(memoryToEdit))
        {
            cout<<"There was no letter"<<endl;
            return;
        }
        ss >> ws;
        if(!ss.eof() || !isalpha(memoryToEdit))
        {
            cout << "Invalid Expression.\n";
            help();
            return;
        }
        cout << "Expression: ";
        cout << memory[toupper(memoryToEdit) - 'A'] << endl;
        cout << "New expression: \n";
        getline(cin, newExpression);
        fflush(stdin);
        memory[toupper(memoryToEdit) - 'A'] = newExpression;
        return;
    }
    if(action == "EXIT" || action == "QUIT")
    {
        if(!saved)
        {
            string reply;
            cout << "Would you like to save the values stored in memory before exiting?(Yes/No)\n";
            cin >> reply;
            while(toupper(reply[0]) != 'Y' && toupper(reply[0]) != 'N')
            {
                cout << "Invalid input, please try again.\n";
                cin >> reply;
            }
            if(toupper(reply[0]) == 'Y')
            {
                cout << "What would you like to save the file name as?\n";
                cin.ignore();
                getline(cin, reply);
                saveToFile(reply, memory);
            }
        }
        exit(0);
        return;
    }
    if(action == "WEXIT" || action == "WQUIT")
    {
        string fileName;
        ss >> ws;
        getline(ss, fileName);
        saveToFile(fileName, memory);
        exit(0);
        return;
    }
    help();
    return;
}

bool performOperation(vector<byte> bigNum, vector<byte> bigNumRemainder, const char &op, vector<byte> bigNum2, vector<byte> bigNum2Remainder, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder)
{
    removeLeadingZero(bigNum);
    removeLeadingZero(bigNum2);
    if(greaterThan(bigNumRemainder, vector<byte>(1, 0)) || greaterThan(bigNum2Remainder, vector<byte>(1, 0)))
    {
        return false;
    }
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
        if(!divide(bigNum, bigNum2, bigNumAns, bigNumAnsRemainder/*, vector<byte>(1, 1)*/))
        {
            return false;
        }
        break;
    }
    case '!':
    {
        factorial(bigNum, bigNumAns);
        break;
    }
    case 'C':
    {
        combinations(bigNum, bigNum2, bigNumAns, bigNumRemainder);
        break;
    }
    case 'P':
    {
        permutations(bigNum, bigNum2, bigNumAns, bigNumRemainder);
        break;
    }
    case 'G':
    {
        gcd(bigNum, bigNum2, bigNumAns);
        break;
    }
    default:
    {
        cout << "Unknown operator.\n";
        return false;
    }
    }
    return true;
}

bool parseAndPerform(string line, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder, const string memory[], int memToRetrieveFrom)
{
    vector<byte> bigNum;
    vector<byte> bigNumRemainder;
    vector<byte> bigNum2;
    vector<byte> bigNum2Remainder;
    stringstream ss;
    string action;
    char op = 0;
    size_t functionFound;
    ss << line;
    ss >> action;
    if((functionFound = line.find_first_of("(")) != string::npos)
    {
        size_t pos = 0;
        op = line.substr(0, functionFound)[0];
        if((pos = line.find(',')) == string::npos)
        {
            return false;
        }
        line.replace(line.find(','), 1, 1, ' ');
        if((pos = line.find_last_of(')')) == string::npos)
        {
            return false;
        }
        line.erase(line.find_last_of(')'), 1);
        ss.clear();
        ss.str("");
        ss << line;
        ss.ignore(functionFound + 1);
        ss >> action;
    }
    else
    {
        if((functionFound = action.find_first_of("!")) != string::npos)
        {
            action = action.substr(0, functionFound);
            ss.putback('!');
        }
    }
    if(parseNumber(action, bigNum, bigNumRemainder, memory, memToRetrieveFrom))
    {
        if(ss.eof())
        {
            bigNumAns = bigNum;
            return true;
        }
        string parse;
        bool invalidInput = false;
        for(unsigned int i = 0; i < 2; ++i)
        {
            if(ss.eof() && op != '!' && !isalpha(op))
            {
                cout << "Invalid expression.\n";
                help();
                return false;
            }
            ss >> parse;
            switch(i)
            {
            case 0:
            {
                if(op == 0)
                {
                    invalidInput = !parseOperator(parse, op);
                }
                break;
            }
            case 1:
            {
                invalidInput = !parseNumber(parse, bigNum2, bigNum2Remainder, memory, memToRetrieveFrom);
                if(invalidInput)
                {
                    invalidInput = !invalidInput && (op == '!' || isalpha(op));
                }
                break;
            }
            }
            if(invalidInput)
            {
                cout << "Invalid Operator or Operand.\n";
                help();
                return false;
            }
        }
        if(!ss.eof())
        {
            cout << "Invalid expression.\n";
            help();
            return false;
        }
        if(!performOperation(bigNum, bigNumRemainder, op, bigNum2, bigNum2Remainder, bigNumAns, bigNumAnsRemainder))
        {
            cout << "Invalid Operand or Operator, unable to calculate.\n";
            return false;
        }
        return true;
    }
    return false;
}

bool parseNumber(const string &line, vector<byte> &bigNum, vector<byte> &bigNumRemainder, const string memory[], int memToRetrieveFrom)
{
    if(line.length() == 1 && isalpha(line[0]))
    {
        int memoryLocation = toupper(line[0]) - 'A';
        int mask = 1 << memoryLocation;
        if((memToRetrieveFrom & mask) == 0)
        {
            memToRetrieveFrom |= mask;
            return parseAndPerform(memory[memoryLocation], bigNum, bigNumRemainder, memory, memToRetrieveFrom);
        }
        cout << "Infinite recursion!\n";
        return false;
    }
    for(unsigned int i = line.length(); i > 0;)
    {
        --i;
        if(line[i] == '-' && !i)
        {
            bigNum[bigNum.size() - 1] *= -1;
            continue;
        }
        if(!isdigit(line[i]))
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
    byte result = 0;
    byte carry = 0;
    unsigned int minSize = min(bigNum.size(), bigNum2.size());
    for(unsigned int i = 0; i < minSize; ++i)
    {
        result = bigNum[i] + bigNum2[i] + carry;
        bigNumAns.push_back(result % 10);
        carry = result / 10;
    }
    for(unsigned int i = minSize; i < bigNum.size(); ++i)
    {
        result = bigNum[i] + carry;
        bigNumAns.push_back(result % 10);
        carry = result / 10;
    }
    for(unsigned int i = minSize; i < bigNum2.size(); ++i)
    {
        result = bigNum2[i] + carry;
        bigNumAns.push_back(result % 10);
        carry = result / 10;
    }
    if(carry)
    {
        bigNumAns.push_back(carry);
    }
}

void subtract(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns)
{
    bigNumAns.resize(0);
    byte carry = 0;
    unsigned int minSize = min(bigNum.size(), bigNum2.size());
    if(lessThan(bigNum, bigNum2))
    {
        subtract(bigNum2, bigNum, bigNumAns);
        bigNumAns[bigNumAns.size() - 1] *= -1;
    }
    else
    {
        for(unsigned int i = 0; i < minSize; ++i)
        {
            bigNum2[i] += carry;
            carry = 0;
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
                if(bigNum[i] < carry)
                {
                    bigNum[i] += 10;
                    bigNumAns.push_back(bigNum[i] - carry);
                }
                else
                {
                    bigNumAns.push_back(bigNum[i] - carry--);
                }
            }
            else
            {
                bigNumAns.push_back(bigNum[i]);
            }
        }
    }
    removeLeadingZero(bigNumAns);
}

void multiply(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns)
{
    bigNumAns.resize(0);
    byte carry = 0;
    byte total = 0;
    int negative = 1;
    vector<vector<byte> > subAns;
    if(isNegative(bigNum))
    {
        bigNum[bigNum.size() - 1] *= -1;
        negative *= -1;
    }
    if(isNegative(bigNum2))
    {
        bigNum2[bigNum2.size() - 1] *= -1;
        negative *= -1;
    }
    if(lessThan(bigNum, bigNum2))
    {
        vector<byte> temp = bigNum;
        bigNum = bigNum2;
        bigNum2 = temp;
    }
    for(size_t i = 0, end = bigNum2.size(); i < end; ++i)
    {
        subAns.push_back(vector<byte>());
        for(size_t j = 0; j < bigNum.size(); ++j)
        {
            total = bigNum[j] * bigNum2[i] + carry;
            carry = total / 10;
            subAns[i].push_back(total % 10);
        }
        if(carry)
        {
            subAns[i].push_back(carry);
            carry = 0;
        }
    }
    for(size_t i = 0; i < subAns.size(); ++i)
    {
        subAns[i].insert(subAns[i].begin(), i, (char)0);
        add(bigNumAns, subAns[i], bigNumAns);
    }
    if(bigNumAns.size())
    {
        bigNumAns[bigNumAns.size() - 1] *= negative;
    }
}

bool divide(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder)
{
    vector<byte> test;
    bigNumAns.resize(1);
    bigNumAns[0] = 0;
    int negative = 1;
    if(isNegative(bigNum))
    {
        bigNum[bigNum.size() - 1] *= -1;
        negative *= -1;
    }
    if(isNegative(bigNum2))
    {
        bigNum2[bigNum2.size() - 1] *= -1;
        negative *= -1;
    }
    if(equals(bigNum2, vector<byte>(1, 0)))
    {
        cout << "Error, division by zero.\n";
        return false;
    }
    bigNumAnsRemainder.resize(0);
    bigNumAnsRemainder = bigNum;
    if(greaterThanEquals(bigNumAnsRemainder, bigNum2))
    {
        multiply(bigNum2, vector<byte>(1, 2), test);
        divide(bigNumAnsRemainder, test, bigNumAns, bigNumAnsRemainder);
        multiply(bigNumAns, vector<byte>(1, 2), bigNumAns);
    }
    while(greaterThanEquals(bigNumAnsRemainder, bigNum2))
    {
        subtract(bigNumAnsRemainder, bigNum2, bigNumAnsRemainder);
        incrementUp(bigNumAns);
    }
    bigNumAns[bigNumAns.size() - 1] *= negative;
    return true;
}

void incrementUp(vector<byte> &bigNumAns)
{
    if(!bigNumAns.size())
    {
        bigNumAns.push_back(0);
    }
    size_t i = 0;
    size_t end = bigNumAns.size();
    for(; i < end && !(++bigNumAns[i] %= 10); ++i);
    if(i == end)
    {
        bigNumAns.push_back(1);
    }
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

bool greaterThanEquals(const vector<byte> &bigNum, const vector<byte> &bigNum2)
{
    if(bigNum.size() == bigNum2.size())
    {
        for(unsigned int i = bigNum.size(); i > 0;)
        {
            if(bigNum[--i] == bigNum2[i] && i)
            {
                continue;
            }
            return bigNum[i] >= bigNum2[i];
        }
    }
    return bigNum.size() > bigNum2.size();
}


bool isNegative(const vector<byte> &bigNum)
{
    return bigNum[bigNum.size() - 1] < 0;
}

void removeLeadingZero(vector<byte> &bigNumAns)
{
    for(int i = bigNumAns.size(); i > 1;)
    {
        --i;
        if(bigNumAns[i] == 0)
        {
            bigNumAns.pop_back();
        }
        else
        {
            break;
        }
    }
}

void factorial(vector<byte> bigNum, vector<byte> &bigNumAns)
{
    if(equals(bigNum, vector<byte>(1, 0)))
    {
        bigNumAns = vector<byte>(1, 1);
    }
    bigNumAns = bigNum;
    while(greaterThan(bigNum, vector<byte>(1, 1)))
    {
        decrement(bigNum);
        multiply(vector<byte>(bigNumAns), bigNum, bigNumAns);
    }
}

void decrement(vector<byte> &bigNumAns)
{
    for(unsigned int i = 0, end = bigNumAns.size(); i < end; ++i)
    {
        if(bigNumAns.size())
        {
            if(i == bigNumAns.size() - 1 && bigNumAns[i] == 1)
            {
                bigNumAns.pop_back();
            }
            if(bigNumAns[i])
            {
                --bigNumAns[i];
                break;
            }
            else
            {
                bigNumAns[i] = 9;
            }
        }
    }
}

void combinations(const vector<byte> &bigNum, const vector<byte> &bigNum2, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder)
{
    if (!greaterThanEquals(bigNum, bigNum2))
    {
        cout<<"[1] has to be bigger than or equal to [2]"<<endl;
        return;
    }
    vector<byte> numerator;
    vector<byte> denominator;
    vector<byte> denominator2;
    factorial(bigNum, numerator);
    subtract(bigNum, bigNum2, denominator);
    factorial(denominator, denominator);
    factorial(bigNum2, denominator2);
    multiply(denominator2, denominator, denominator);
    divide(numerator, denominator, bigNumAns, bigNumAnsRemainder);
}

void permutations(const vector<byte> &bigNum, const vector<byte> &bigNum2, vector<byte> &bigNumAns, vector<byte> &bigNumAnsRemainder)
{
    if (!greaterThanEquals(bigNum, bigNum2))
    {
        cout<<"[1] has to be bigger than or equal to [2]"<<endl;
        return;
    }
    vector<byte> numerator;
    vector<byte> denominator;
    factorial(bigNum, numerator);
    subtract(bigNum, bigNum2, denominator);
    factorial(denominator, denominator);
    divide(numerator, denominator, bigNumAns, bigNumAnsRemainder);
}

void gcd(vector<byte> bigNum, vector<byte> bigNum2, vector<byte> &bigNumAns)
{
    if (!greaterThanEquals(bigNum, bigNum2))
    {
        cout<<"[1] has to be bigger than or equal to [2]"<<endl;
        return;
    }
    vector<byte> numbers[2] = {bigNum, bigNum2};
    vector<byte> junk;
    unsigned int i = 0;
    if(greaterThan(numbers[0], vector<byte>(1,0)) && greaterThan(numbers[1], vector<byte>(1,0)))
    {
        for(; greaterThan(numbers[(i + 1) % 2], vector<byte>(1,0)); i = ++i % 2)
        {
            divide(numbers[i], numbers[(i + 1) % 2], junk, numbers[i]);
        }
        bigNumAns = numbers[i];
    }
    return;
}

void displayAllMemory(const string memory[])
{
    for(int i = 0; i < 26; ++i)
    {
        cout << char(i+65) << " = ";
        cout << memory[i] << endl;
    }
}

void outputNumber(ostream &out, const vector<byte> &bigNum)
{
    for(int i = bigNum.size(); i > 0;)
    {
        out << (short int)bigNum[--i];
        if(!(i % 3) && i)
        {
            out << ",";
        }
    }
}

bool loadFromFile(const string &fileName, string memory[])
{
    clearMemories(memory);
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
        getline(infile, memory[i]);
//        getline(infile, number);
//        remainderPos = number.find_first_of(" ");
//        parseNumber(number.substr(0, remainderPos), memory[i], memory);
//        if(remainderPos < string::npos)
//        {
//            parseNumber(number.substr(remainderPos), memoryRemainder[i], memory, memoryRemainder);
//        }
    }
    infile.close();
    return true;
}

void saveToFile(const string &fileName, const string memory[])
{
    ofstream outfile;
    outfile.open(fileName.c_str());
    for(int i = 0; i < 26; ++i)
    {
        outfile << memory[i] << endl;
//        outputNumber(outfile, memory[i]);
//        outfile << " ";
//        outputNumber(outfile, memoryRemainder[i]);
    }
    outfile.close();
    return;
}

void clearMemories(string memory[])
{
    for(unsigned int i = 0; i < 26; ++i)
    {
        memory[i] = "";
    }
}

void help()
{
    cout << "**Welcome to the help menu**\n"
         <<"List of actions:\n"
         <<"[Expression]: Compute out an expression\n"
         <<"LET [A-Z] [Expression]: Set a memory not already present\n"
         <<"LIST: List all the save memorys\n"
         <<"SET [A-Z] [Expression]: Sets the value of the memory\n"
         <<"EDIT [A-Z] [Expression]: Edits a current expression\n"
         <<"SHOW [A-Z]: Shows the current value of the memory\n"
         <<"SAVE [filename]: saves the memories to a filename listed\n"
         <<"LOAD [filename]: loads the memories from the filename to the program\n"
         <<"The following operators can be used:"
         <<"[1]_+_[2]\n"
         <<"[1]_-_[2]\n"
         <<"[1]_*_[2]\n"
         <<"[1]_/_[2]\n"
         <<"[1]!\n"
         <<"P([1],[2])| [1] has to be bigger than or equal to [2]\n"
         <<"C([1],[2])| [1] has to be bigger than or equal to [2]\n"
         <<"GCD([1],[2])| [1] has to be bigger than or equal to [2]\n"
         <<"\n"
         <<endl;
}

