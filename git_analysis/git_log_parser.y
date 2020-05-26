%{
  #include <iostream>
  #include <cstdlib>
  #include <stdio.h>
  #include <string>
  #include "unistd.h"
  #include <fstream>
  using namespace std;
 
  extern int yylex(yy::parser::semantic_type *val);
  extern int yyparse();
%}
%language "c++"

%union {
  int ival;
  float fval;
  char *sval;
}

%token GL_AUTHOR GL_DATE GL_COMMIT GL_NUMBER GL_STRING GL_EMAIL   
// Define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the %union:
%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING

%%
// This is the actual grammar that bison will parse, but for right now it's just
// something silly to echo to the screen what bison gets from flex.  We'll
// make a real one shortly:
snazzle:
  INT snazzle      {
      cout << "bison found an int: " << $1 << endl;
    }
  | FLOAT snazzle  {
      cout << "bison found a float: " << $1 << endl;
    }
  | STRING snazzle {
      cout << "bison found a string: " << $1 << endl; free($1);
    }
  | INT            {
      cout << "bison found an int: " << $1 << endl;
    }
  | FLOAT          {
      cout << "bison found a float: " << $1 << endl;
    }
  | STRING         {
      cout << "bison found a string: " << $1 << endl; free($1);
    }
  ;
%%