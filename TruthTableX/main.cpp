#include <iostream>     // std::cin, std::cout
#include <string>       // std::string
#include <array>
#include <bitset>       // allows the usage of bit set
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <deque>
#include <stack>
#include <iomanip>

using namespace std;

string getVariables(string);
vector<bool> calculateExp(string);

int main ()
{
        string input;
        input = "D~B&BA~&<=>";

        cout<<setfill('-')<<setw(21)<<"-"<<setfill(' ')<<endl
           <<setw(2)<<left<<"|"<<right<<"value  | PQRS |   |"<<endl
          <<setfill('-')<<setw(21)<<"-"<<setfill(' ')<<endl;

        cout<<setfill('-')<<setw(21)<<"-"<<setfill(' ')<<endl;

        string variables = getVariables(input);
        cout<<"variables="<<variables<<endl;
        vector<bool> hi = calculateExp(input);
        for (size_t i = 0; i < hi.size(); ++i )
            cout<<hi[i]<<endl;

  return 0;
}

string getVariables(string input)
{
    size_t pos = 0;
    string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string vtable = "";
    while((pos = input.find_first_not_of(letters, pos)) < string::npos)
    {
        input.erase(pos, 1);
    }
    for (size_t i = 0; i < input.length(); ++i)
        if (!(vtable.find_first_of(input[i]) < string::npos))
            vtable += input[i];
    sort(vtable.begin(),vtable.end());
    return vtable;
}

vector<bool> calculateExp(string line)
{
    bool calc;
    vector<bool> ans;
    bitset<26> bits;
    string vtable = getVariables(line);
    string operators("~&|@%^=<");
    string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string temp;
    stringstream ss;
    size_t num = pow(2,vtable.length());
    deque<bool> express;
    deque<string> op;

    for (size_t i = 0; i < num; ++i)
    {
        bits = i;
        string parse = line; //makes a temporary variable for each iteration
//        cout<<"vtable = "<<vtable<<endl;
//        cout<<"bits = "<<bits<<endl;
//        cout<<"line="<<parse<<endl;
        while(parse != "")
        {
            ss.clear();
            ss<<parse[0];
            if (parse.find("=>") == 0)
                ss<<parse[1];
            else if (parse.find("<=>") == 0)
                ss<<parse[1]<<parse[2];
            ss>>temp;
            parse.erase(0, temp.length());

            if (isalpha(temp[0]))
            {
                express.push_back(bits[vtable.find(temp[0])]);
            }
            else
            {
                op.push_back(temp);
            }
        }
//        cout<<"operands:"<<endl;
//        for (size_t i = 0; i < express.size(); ++i)
//            cout<<express[i]<< " ";
//        cout<<endl;
//        cout<<"operators:"<<endl;
//        for (size_t i = 0; i < op.size(); ++i)
//            cout<<op[i]<<" ";
//        cout<<endl;
//        cout<<"Trying out the equation"<<endl;
        while (op.size())
        {
//            cout<<"---------------"<<endl;
            if (op.front() == "~")
            {
//                cout<<"Doing ~"<<endl;//D~B&BA~&<=>
                express.front() = !(express.front());
            }
            else if (op.front() == "&")
            {
//                cout<<"Doing &"<<endl;//D~B&BA~&<=>
                calc = express.front();
                express.pop_front();
                express.front() = (calc & express.front());
            }
            else if (op.front() == "|")
            {
//                cout<<"Doing |"<<endl;//D~B&BA~&<=>
                calc = express.front();
                express.pop_front();
                express.front() = (calc | express.front());
            }
            else if (op.front() == "@")
            {
//                cout<<"Doing @"<<endl;//D~B&BA~&<=>
                calc = express.front();
                express.pop_front();
                express.front() = !(calc & express.front());
            }
            else if (op.front() == "%")
            {
//                cout<<"Doing %"<<endl;//D~B&BA~&<=>
                calc = express.front();
                express.pop_front();
                express.front() = !(calc | express.front());
            }
            else if (op.front() == "^")
            {
//                cout<<"Doing ^"<<endl;//D~B&BA~&<=>
                calc = express.front();
                express.pop_front();
                express.front() = (calc ^ express.front());
            }
            else if (op.front() == "=>")
            {
//                cout<<"Doing =>"<<endl;
                express.pop_front();
            }
            else if (op.front() == "<=>")
            {
//                cout<<"Doing <=>"<<endl;//D~B&BA~&<=>
                calc = express.front();
                express.pop_front();
                express.front() = (calc == express.front());
            }
            op.pop_front();
//            cout<<"---------------"<<endl;
//            cout<<"Expression"<<endl;
//            for (size_t i = 0; i < express.size(); ++i)
//                cout<<express[i]<< " ";
//            cout<<endl;
//            cout<<"operators:"<<endl;
//            for (size_t i = 0; i < op.size(); ++i)
//                cout<<op[i]<<" ";
//            cout<<endl;
        }
        ans.push_back(express.front());
        express.clear(); //clears the deque after using it
    }
return ans;
}
