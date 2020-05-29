%{
  #include <iostream>
  #include <cstdlib>
  #include <stdio.h>
  #include <string>
  #include "unistd.h"
  #include <fstream>
  using namespace std;
 #include "git_log_parser.tab.hh"
  extern int yylex(yy::parser::semantic_type *val);
  extern int yyparse();
%}
%language "c++"

%union {
  int ival;
  char* sval;
}

//%token GL_AUTHOR GL_DATE GL_COMMIT GL_NUMBER GL_STRING GL_EMAIL   
// Define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the %union:
%token <ival> GL_NUMBER
%token <sval> GL_AUTHOR
%token <sval> GL_DATE
%token <sval> GL_COMMIT
%token <sval> GL_STRING
%token <sval> GL_EMAIL


%%
// This is the actual grammar that bison will parse, but for right now it's just
// something silly to echo to the screen what bison gets from flex.  We'll
// make a real one shortly:
snazzle:
  GL_AUTHOR snazzle      {
      cout << "bison found an author: " << $1 << endl;
      delete[] $1;
    }
  | GL_DATE snazzle  {
      cout << "bison found a date: " << $1 << endl;
    }
  | GL_COMMIT snazzle {
      cout << "bison found a commit: " << $1<< endl; 
    }
  | GL_NUMBER            {
      cout << "bison found an number: " << $1 << endl;
    }
  | GL_STRING          {
      cout << "bison found a string: " << $1 << endl;
    }
  | GL_EMAIL         {
      cout << "bison found an email: " << $1 << endl; 
    }
  ;
%%