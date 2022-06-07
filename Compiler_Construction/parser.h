#include "token.h"
#include <iostream> 
#include <string> 
#include <map> 

using namespace std;

void addOtherEntries(map<pair<string, string>, vector<string> >  &otherEntries, string variable, int token_no){
    // cout << "\nCurrent variable: " << variable;
    otherEntries[{"returnStatement", "return"}].push_back("returnStatement ::= return " + variable + " ;");

    otherEntries[{"NT5", "+"}].push_back("NT5 ::= arithmeticOperator " + variable);
    otherEntries[{"NT5", "*"}].push_back("NT5 ::= arithmeticOperator " + variable);
    otherEntries[{"NT5", "and"}].push_back("NT5 ::= logicalOperator " + variable);
    otherEntries[{"NT5", "or"}].push_back("NT5 ::= logicalOperator " + variable);

    if(token_no == 200){
        otherEntries[{"function", "boolean"}].push_back("function ::= type " + variable + " ( arglist ) { statements returnStatement }");
        otherEntries[{"function", "int"}].push_back("function ::= type " + variable + " ( arglist ) { statements returnStatement }");
        otherEntries[{"function", "float"}].push_back("function ::= type " + variable + " ( arglist ) { statements returnStatement }");

        otherEntries[{"arg", "boolean"}].push_back("arg ::= type " + variable);
        otherEntries[{"arg", "int"}].push_back("arg ::= type " + variable);
        otherEntries[{"arg", "float"}].push_back("arg ::= type " + variable);

        otherEntries[{"input", "in"}].push_back("input ::= in ( " + variable + " )");
        otherEntries[{"output", "out"}].push_back("output ::= out ( " + variable + " )");
    }
}

void editTableForVariables(map<pair<string, string>, string> &table, string variable, int token_no){
    // cout << "\nCurrent variable: " << variable;
    table[{"statements", variable}] = "statements ::= statement NT3";

    table[{"NT6", variable}] = "NT6 ::= expression";

    table[{"expression", variable}] = "expression ::= " + variable + " NT5";

    // adding synch 
    table[{"type", variable}] = "synch";
    table[{"arithmeticOperator", variable}] = "synch";
    table[{"relationalOperator", variable}] = "synch";
    table[{"logicalOperator", variable}] = "synch";

    if(token_no == 200) {
        table[{"statement", variable}] = "statement ::= " + variable + " := NT6";
        table[{"argCallingList", variable}] = "argCallingList ::= " + variable + " NT7";
        table[{"NT6", variable}] = "NT6 ::= " + variable + " ( argCallingList )";
        table[{"varlist", variable}] = "varlist ::= " + variable + " NT4";

        
        
    }
}

map<pair<string, string>, string> makeTable(){

    map<pair<string, string>, string>   table;
    
    table[{"S", "boolean"}] = "S ::= program $";
    table[{"S", "int"}] = "S ::= program $";
    table[{"S", "float"}] = "S ::= program $";
    table[{"S", "main"}] = "S ::= program $";

    table[{"program", "boolean"}] = "program ::= functions MAIN";
    table[{"program", "int"}] = "program ::= functions MAIN";
    table[{"program", "float"}] = "program ::= functions MAIN";
    table[{"program", "main"}] = "program ::= MAIN";

    table[{"functions", "boolean"}] = "functions ::= function NT1";
    table[{"functions", "int"}] = "functions ::= function NT1";
    table[{"functions", "float"}] = "functions ::= function NT1";

    table[{"NT1", "boolean"}] = "NT1 ::= functions";
    table[{"NT1", "int"}] = "NT1 ::= functions";
    table[{"NT1", "float"}] = "NT1 ::= functions";
    table[{"NT1", "main"}] = "NT1 ::= `";
    
    table[{"arglist", "boolean"}] = "arglist ::= arg NT2";
    table[{"arglist", "int"}] = "arglist ::= arg NT2";
    table[{"arglist", "float"}] = "arglist ::= arg NT2";
    table[{"arglist", ")"}] = "arglist ::= `";

    table[{"NT2", ")"}] = "NT2 ::= `";
    table[{"NT2", ","}] = "NT2 ::= , arglist";

    table[{"type", "boolean"}] = "type ::= boolean";
    table[{"type", "int"}] = "type ::= int";
    table[{"type", "float"}] = "type ::= float";

    table[{"MAIN", "main"}] = "MAIN ::= main ( ) { statements }";

    table[{"statements", "}"}] = "statements ::= `";
    table[{"statements", "return"}] = "statements ::= `";
    table[{"statements", "boolean"}] = "statements ::= statement NT3";
    table[{"statements", "int"}] = "statements ::= statement NT3";
    table[{"statements", "float"}] = "statements ::= statement NT3";
    table[{"statements", "in"}] = "statements ::= statement NT3";
    table[{"statements", "out"}] = "statements ::= statement NT3";
    table[{"statements", "if"}] = "statements ::= statement NT3";
    table[{"statements", "while"}] = "statements ::= statement NT3";

    table[{"NT3", "}"}] = "NT3 ::= `";
    table[{"NT3", "return"}] = "NT3 ::= `";
    table[{"NT3", ";"}] = "NT3 ::= ; statements";

    table[{"statement", "boolean"}] = "statement ::= declaration";
    table[{"statement", "int"}] = "statement ::= declaration";
    table[{"statement", "float"}] = "statement ::= declaration";
    table[{"statement", "in"}] = "statement ::= input";
    table[{"statement", "out"}] = "statement ::= output";
    table[{"statement", "if"}] = "statement ::= conditional";
    table[{"statement", "while"}] = "statement ::= loop";

    table[{"argCallingList", ")"}] = "argCallingList ::= `";

    table[{"NT7", ")"}] = "NT7 ::= `";
    table[{"NT7", ","}] = "NT7 ::= , argCallingList";

    table[{"declaration", "boolean"}] = "declaration ::= type varlist";
    table[{"declaration", "int"}] = "declaration ::= type varlist";
    table[{"declaration", "float"}] = "declaration ::= type varlist";

    table[{"NT4", "}"}] = "NT4 ::= `";
    table[{"NT4", "return"}] = "NT4 ::= `";
    table[{"NT4", ","}] = "NT4 ::= , varlist";
    table[{"NT4", ";"}] = "NT4 ::= `";

    table[{"NT5", ")"}] = "NT5 ::= `";
    table[{"NT5", "}"}] = "NT5 ::= `";
    table[{"NT5", "return"}] = "NT5 ::= `";
    table[{"NT5", ";"}] = "NT5 ::= `";
    table[{"NT5", "<"}] = "NT5 ::= `";
    table[{"NT5", ">"}] = "NT5 ::= `";
    table[{"NT5", "=="}] = "NT5 ::= `";

    table[{"arithmeticOperator", "+"}] = "arithmeticOperator ::= +";
    table[{"arithmeticOperator", "*"}] = "arithmeticOperator ::= *";
    
    table[{"relationalOperator", "<"}] = "relationalOperator ::= <";
    table[{"relationalOperator", ">"}] = "relationalOperator ::= >";
    table[{"relationalOperator", "=="}] = "relationalOperator ::= ==";

    table[{"logicalOperator", "and"}] = "logicalOperator ::= and";
    table[{"logicalOperator", "or"}] = "logicalOperator ::= or";

    table[{"conditional", "if"}] = "conditional ::= if ( expression relationalOperator expression ) { statements }";

    table[{"loop", "while"}] = "loop ::= while ( expression relationalOperator expression ) { statements }";

    // adding follows set entries for synch error check
    table[{"program", "$"}] = "synch";
    table[{"functions", "main"}] = "synch";
    table[{"function", "main"}] = "synch";
    table[{"returnStatement", "}"}] = "synch";
    table[{"arg", ")"}] = "synch";
    table[{"arg", ","}] = "synch";
    table[{"type", "functionName"}] = "synch";
    table[{"type", "variable"}] = "synch";
    table[{"MAIN", "$"}] = "synch";
    table[{"statement", "return"}] = "synch";
    table[{"statement", "}"}] = "synch";
    table[{"statement", ";"}] = "synch";
    table[{"NT6", "return"}] = "synch";
    table[{"NT6", "}"}] = "synch";
    table[{"NT6", ";"}] = "synch";
    table[{"input", "return"}] = "synch";
    table[{"input", "}"}] = "synch";
    table[{"input", ";"}] = "synch";
    table[{"output", "return"}] = "synch";
    table[{"output", "}"}] = "synch";
    table[{"output", ";"}] = "synch";
    table[{"declaration", "return"}] = "synch";
    table[{"declaration", "}"}] = "synch";
    table[{"declaration", ";"}] = "synch";
    table[{"varlist", "return"}] = "synch";
    table[{"varlist", "}"}] = "synch";
    table[{"varlist", ";"}] = "synch";
    table[{"expression", "return"}] = "synch";
    table[{"expression", "}"}] = "synch";
    table[{"expression", ";"}] = "synch";
    table[{"expression", ")"}] = "synch";
    table[{"expression", "<"}] = "synch";
    table[{"expression", ">"}] = "synch";
    table[{"expression", "=="}] = "synch";
    table[{"arithmeticOperator", "variable"}] = "synch";
    table[{"relationalOperator", "variable"}] = "synch";
    table[{"logicalOperator", "variable"}] = "synch";
    table[{"conditional", "return"}] = "synch";
    table[{"conditional", "}"}] = "synch";
    table[{"conditional", ";"}] = "synch";
    table[{"loop", "return"}] = "synch";
    table[{"loop", "}"}] = "synch";
    table[{"loop", ";"}] = "synch";
    

    return table;

}
