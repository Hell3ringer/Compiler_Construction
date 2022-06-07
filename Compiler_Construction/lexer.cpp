
#include "token.h"
#include <iostream> 
#include <string> 
#include <vector> 
#include <fstream> 

#define call(function) function(line, &pos, lineNumber); break

using namespace std; 
vector<token> listOfTokens = {};

const vector <string> keywords = {"main", "boolean", "int", "float", "while", "if", "and", "or", "return", "in", "out"};

void error(char ch){
  cout << "Lexical Error: \"" << ch << "\" is not a part of the character set of the language, skipping..." << endl;
}

void error(string lexeme, int *pos){
  cout << "Lexical Error: \"" << lexeme << "\" did not match any regular expression, skipping to the next character..." << endl;
  (*pos)++;
}

void getComment(string line, int *pos, int lineNumber){
  // cout << "handling comment" << endl;
  (*pos)++;
  if (line[*pos] == '/') {
    while(line[(*pos)]) (*pos)++;
  }
  else{
    (*pos)--;
    error("/",pos);
    return;
  }
  
}

void getArithmeticOperator(string line, int *pos, int lineNumber){
  if(line[(*pos)] == '+') {
    listOfTokens.push_back(newToken(111, "+", lineNumber));
  }
  if(line[(*pos)] == '*') {
    listOfTokens.push_back(newToken(112, "*", lineNumber));
  }
  (*pos)++;
}

void getRelationalOperator(string line, int *pos, int lineNumber){
  char ch =  line[*pos]; 
  token temp;
  if (ch == '<') {
    temp = newToken(123, "<", lineNumber);
  }
  else if (ch == '>') {
    temp = newToken(124, ">", lineNumber);
  }
  else if (ch == '=') {
    (*pos)++;
    if(line[*pos] == '=') {
      temp = newToken(125, "==", lineNumber);
    }
    else if (line[*pos] == ' '){
      error("=", pos);
      return;
    }
    else {
      (*pos)--;
      error("=", pos);
      return;
    }
  }
  (*pos)++;
  listOfTokens.push_back(temp);
}

void getName(string line, int *pos, int lineNumber){
  string cur_lexeme = "";
  while(line[(*pos)] && ((line[(*pos)]>='a' && line[(*pos)]<='z') || (line[(*pos)]>='A' && line[(*pos)]<='Z') || (line[(*pos)]>='0' && line[(*pos)]<='9'))){
    cur_lexeme += line[(*pos)];
    (*pos)++;
  }    
  int variable = 1, n = keywords.size();
  for(int i=0; i<n; i++){
    if(cur_lexeme == keywords[i]) {
      listOfTokens.push_back(newToken(201+i, cur_lexeme, lineNumber));
      variable = 0;
      break;
    }
  }
  if(variable == 1){
    listOfTokens.push_back(newToken(200, cur_lexeme, lineNumber));
  }
}

void getNumber(string s, int *i, int line_no){
  // cout << "handling number" << endl;
  int state = 0;
  string cur_lexeme = "";
  int flag = 0;
  if(s[*i] == '-')
  {
    cur_lexeme += s[*i];
    (*i)++;
    flag = 1;
  }
  if(s[*i] < '0' || s[*i] > '9')
  {
    state = 404;
    error(cur_lexeme, i);
    (*i)--;
    return;
    (*i)++;
  }
  if(state == 0)
  {
    //cout << "a " << s[*i] << "\n";
    if(s[*i] == '0')
      state = 1;
    else 
      state = 2;
    cur_lexeme += s[*i];
    (*i)++;
  }
  if(state == 1)
  {
    //cout << "b "<< s[*i] << "\n";
    if(s[*i] == '.')
    {
      cur_lexeme += s[*i];
      (*i)++;
      state = 3; 
    }
    else
      listOfTokens.push_back(newToken(102, cur_lexeme, line_no));
  }
  if(state == 2)
  {
    //cout << "c " << s[*i] << "\n";
    while(s[*i] >= '0' && s[*i] <= '9')
      cur_lexeme += s[*i], (*i)++;
    if(s[*i] == '.')
    {
      cur_lexeme += s[*i];
      (*i)++;
      state = 3;
    }
    else
      listOfTokens.push_back(newToken(102, cur_lexeme, line_no));
  }
  if(state == 3)
  {
    if(s[*i] < '0' || s[*i] > '9'){
      state = 404;
      error(cur_lexeme, i);
      (*i)--;
      return;
    }
    else
    {
      state = 4; 
      cur_lexeme += s[*i];
      (*i)++;
    }
  }
  if(state == 4)
  {
    while(s[*i] >= '0' && s[*i] <= '9')
      cur_lexeme += s[*i], (*i)++;
    listOfTokens.push_back(newToken(102, cur_lexeme, line_no));
  }
}

void getAssignmentOperator(string line, int *pos, int lineNumber){
  (*pos)++;
  if (line[*pos] == '='){
    listOfTokens.push_back(newToken(120, ":=", lineNumber));
  }
  else if (line[*pos] == ' '){
    error(":", pos);
    return;
  }
  else{
    (*pos)--;
    error(":", pos);
    return;
  }
  (*pos)++;
}

void getDelimiter(string line, int *pos, int lineNumber){
  // cout << "handling delimiter" << endl;
  if(line[(*pos)] == '{') {
    listOfTokens.push_back(newToken(131, "{", lineNumber));
  }
  if(line[(*pos)] == '}') {
    listOfTokens.push_back(newToken(132, "}", lineNumber));
  }
  if(line[(*pos)] == '(') {
    listOfTokens.push_back(newToken(133, "(", lineNumber));
  }
  if(line[(*pos)] == ')') {
    listOfTokens.push_back(newToken(134, ")", lineNumber));
  }
  if(line[(*pos)] == ',') {
    listOfTokens.push_back(newToken(135, ",", lineNumber));
  }
  (*pos)++;
}
vector<token> getListOfTokens(){
  return listOfTokens;
}
int scan(){
  ifstream fin;
  ofstream fout;
  // 1 5 6
  fin.open("tests/6in.txt");
  fout.open("tests/6out-lexer.txt");
  string line;
  int lineNumber = 0;
  while (getline(fin, line)) { 
    lineNumber++;
    // cout << "line" << lineNumber << endl;
    int pos = 0;
    while(line[pos]){
      if((line[pos]<='9' && line[pos]>='0') || line[pos]=='-'){
        getNumber(line, &pos, lineNumber);
      }
      else if((line[pos]>='a' && line[pos]<='z') || (line[pos]>='A' && line[pos]<='Z')){
        getName(line, &pos, lineNumber);     //variable name, function name etc
      }
      else{
        switch(line[pos]){
          case ' ':  
          case '\t': pos++; break;
          case '/':  call(getComment);
          case '+':
          case '*':  call(getArithmeticOperator);
          case '<':
          case '>':
          case '=':  call(getRelationalOperator);
          case ':':  call(getAssignmentOperator);
          case '(':
          case ')':
          case '{':
          case '}':
          case ',':  call(getDelimiter);
          case '.': error(".", &pos); break;
          case ';': listOfTokens.push_back(newToken(500, ";", lineNumber)); pos++; break;
          //some other cases
          default:   error(line[pos]); pos++; break;  //error - character is invalid
        }
      }
    }
  }

  for(token temp: listOfTokens){
    fout << "Token "<< temp.token_no <<", string \""<< temp.lexeme <<"\", line number "<<temp.line_no << "\n";
  }

  fin.close();
  fout.close();
  return 0;
}
