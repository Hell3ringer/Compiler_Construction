#include "parser.h"
#include <iostream> 
#include <string> 
#include <sstream> 
#include <vector> 
#include <map> 
#include <stack> 
#include <algorithm> 
#include <fstream> 

using namespace std; 

vector <string> T;
const vector <string> NT = {"program", "functions", "function", "returnStatement", "argList", "arg", "type", "MAIN", "statements", "statement", "input", "output", "declaration", "varlist", "expression", "arithmeticOperator", "relationalOperator", "logicalOperator", "conditional", "loop", "NT1", "NT2", "NT3", "NT4", "NT5", "NT6"};


void printStackContents(stack<string> s){
  cout << "Stack contents:\t";
  while(!s.empty()){
    cout << s.top() << " ";
    s.pop();
  }
  cout << endl;
}

bool checkIfStackEmpty(stack<string> &s, vector<pair<string, int> > &input, int it){
  int end = input.size();
  if(s.empty()){
    cout << "Syntax error: Stack got empty before full input is parsed.\n";
    cout << "Input left to be parsed:\n";
    for(int i=it; i<end; i++){
      cout << input[i].first << " ";
    }
    cout << endl;
    return true;
  }
}

bool parseInput(vector<pair<string, int> > &input, map<pair<string, string>, string> &table, map<pair<string, string>, vector<string> >  &otherEntries, map <string, int> &tokenAlreadyAdded, vector<string> &matched){
  stack<string> s;
  s.push("S");
  input.push_back(make_pair("$", 0));
  int it = 0, end = input.size();

  while(it < end){
    if(checkIfStackEmpty(s, input, it)) return false;

    if(it == end-1){
      if(s.top() == "$" && input[it].first == "$"){
        s.pop();
        it++;
        return true;
      }
    }
    if(input[it].first == "`"){
      it++;
      continue;
    }

    if(s.top() == "`")  s.pop();
    if(checkIfStackEmpty(s, input, it)) return false;

    int stackChanges = 0;
    while(it < end && s.top() == input[it].first){
      if(!s.empty()){
        s.pop();
        stackChanges = 1;
        cout << endl << input[it].first << " matched.\n";
        matched.push_back(input[it].first);
        it++;
      } else {
        cout << "Syntax error: Stack got empty before full input is parsed.\n";
        return false;
      }
    }
    if(stackChanges)
      printStackContents(s);

    string line = "";
    int flag = 0;
    if(s.top() == "NT6" && tokenAlreadyAdded.find(input[it].first) != tokenAlreadyAdded.end()){
      for(int i=it; input[i].first!=";" && i<end; i++){
        if(input[i].first=="("){
          flag=1;
          break;
        }
      }
      if(flag==0) line = "NT6 ::= expression";
    }
    if(table.find({s.top(), input[it].first}) == table.end() || flag == 1){
      string nextVariable = "";
      for(int i=it; i<end; i++){
        if(tokenAlreadyAdded.find(input[i].first) != tokenAlreadyAdded.end()){
          nextVariable = input[i].first;
          break;
        }
      }
      if(nextVariable == ""){
        cout << "\n ******** Syntax error 1: No corresponding entry at line number " << input[it].second << " found in the parse table for lexeme " << input[it].first <<"\n";
        cout << "Popping top of the stack " << s.top() << "\n";
        s.pop();
        // cout << "Skipping this input\n\n";
        // it++;
        continue;
        // return false;
      }
      for(string i: otherEntries[{s.top(), input[it].first}]){
        int n = nextVariable.length(), n1 = i.length();
        for(int j=0; j<n1-n; j++){
          if((i.substr(j, n+1) == " " + nextVariable) || (i.substr(j, n+1) == nextVariable + " ")){
        // for(int j=0; j<n1-n; j++){
        //     if(i.substr(j, n) == nextVariable){
            line = i;
            goto g;
          }
        }
      }
      g:{}
    }

    if(line == "")  line = table[{s.top(), input[it].first}];
    if(line == "") {
      cout << "\n ******** Syntax error 2: No corresponding entry at line number " << input[it].second << " found in the parse table for lexeme " << input[it].first << "\n";
      cout << "Skipping this input\n\n";
      it++;
      continue;
      // return false;
    }
    if(line == "")  line = table[{s.top(), input[it].first}];
    if(line == "synch") {
      cout << "\n ******** Syntax error 3: Synch error at line number " << input[it].second << " found in the parse table for lexeme " << input[it].first << "\n";
      cout << "Popping top of the stack " << s.top() << "\n";
      s.pop();
      // it++;
      continue;
      // return false;
    }
    int check=0;
    vector<string> rhs;
    string lhs;
    stringstream check1(line);
    string intermediate;

    // Tokenizing w.r.t. space ' '
    while(getline(check1, intermediate, ' '))
    {
      check++;
      if(check==1){
        lhs = intermediate;
      }
      else if(check > 2)
        rhs.push_back(intermediate);
    }
    reverse(rhs.begin(), rhs.end());
    s.pop();
    printStackContents(s);
    for(string toPush: rhs){
      s.push(toPush);
    }
    if(checkIfStackEmpty(s, input, it)) return false;
    printStackContents(s);
    if(s.top() == "`")  s.pop();

    stackChanges = 0;
    while(it < end && s.top() == input[it].first){
      if(!s.empty()){
        s.pop();
        stackChanges = 1;
        cout << endl << input[it].first << " matched.\n";
        matched.push_back(input[it].first);
        it++;
      } else {
        cout << "Syntax error: Stack got empty before full input is parsed.\n";
        return false;
      }
    }
    if(stackChanges)
      printStackContents(s);
  }

  if(!s.empty()){
    cout << "Syntax error: Stack still has some elements though input is parsed.\n";
    printStackContents(s);
    return false;
  }
  return true;
}

int main(){
  int lexer = scan();
  vector<token> listOfTokens = getListOfTokens();
  vector<string> matched;
  map<pair<string, string>, string>   table = makeTable();
  map<pair<string, string>, vector<string> >  otherEntries;
  T = {"boolean", "int", "float", "{", "}", "(", ")", "+", "-", "*", ">", "<", "==", "and", "or", "if", "while"};
  map <string, int> tokenAlreadyAdded;
  vector<pair<string, int> > input;
  for(token t: listOfTokens){
    // cout << t.line_no << "\n";
    input.push_back(make_pair(t.lexeme, t.line_no));
    if((t.token_no == 200 || t.token_no == 102) && tokenAlreadyAdded[t.lexeme] == 0) {
      T.push_back(t.lexeme);
      tokenAlreadyAdded[t.lexeme] = 1;
      editTableForVariables(table, t.lexeme, t.token_no);
      addOtherEntries(otherEntries, t.lexeme, t.token_no);
      // cout << t.lexeme << " ";
    }
  }

  // for(auto i: tokenAlreadyAdded){
  //   cout<<i.first<<" ";
  // }
  // cout<<endl;
// for(pair<int,int> it2:v){
//     cout<<"Car "<<it2.first<<" , "<<it2.second<<endl;
// }

  cout << "\nInput to parser: ";
  for(pair<string,int> it: input){
    cout << it.first << " ";
  }
  cout << endl;
  if(!parseInput(input, table, otherEntries, tokenAlreadyAdded, matched))
    cout << "\nParsing stopped due to syntax error.\n";
  else 
    cout << "\nParsing completed.\n";
  cout << "\nMatched Input: ";
  for (string str:  matched){
    cout << str << " ";
  }
  cout << "\n";
  return 0;
}
