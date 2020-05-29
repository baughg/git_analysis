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

  /*snazzle:
  GL_AUTHOR snazzle      {
      cout << "bison found an author: " << $1 << endl;
      delete[] $1;
    }
  | GL_DATE snazzle  {
      cout << "bison found a date: " << $1 << endl;
      delete[] $1;
    }
  | GL_COMMIT snazzle {
      cout << "bison found a commit: " << $1<< endl; 
      delete[] $1;
    }
  | GL_NUMBER            {
      cout << "bison found an number: " << $1 << endl;
    }
  | GL_STRING          {
      cout << "bison found a string: " << $1 << endl;
      delete[] $1;
    }
  | GL_EMAIL         {
      cout << "bison found an email: " << $1 << endl; 
      delete[] $1;
    }
  ;*/
%}
%language "c++"

%union {
  int ival;
  char* sval;
}

%token <ival> GL_NUMBER
%token <sval> GL_AUTHOR
%token <sval> GL_DATE
%token <sval> GL_COMMIT
%token <sval> GL_STRING
%token <sval> GL_EMAIL
%token <sval> GL_TIME
%token <sval> GL_TIME_ZONE
%token GL_SPACE

%%
prog :  | commit_entries 
                  ;
string_list : GL_STRING {std::cout << "str " << $1 << std::endl;}
                  | GL_STRING string_list {std::cout << "str_lst " << $1 << std::endl;}
                  ; 
commit_entry :  GL_COMMIT 
                string_list 
                GL_AUTHOR space_list 
                string_list space_list 
                GL_EMAIL space_list 
                GL_DATE space_list 
                string_list 
                number_list GL_TIME
                number_list GL_TIME_ZONE string_list
                  ;  
commit_entries :  commit_entry 
                  | commit_entry commit_entries
                  ; 

number_list : GL_NUMBER 
                  | GL_NUMBER number_list
                  ;

space_list : GL_SPACE 
                  | GL_SPACE space_list
                  ;
%%