%{
  #include <iostream>
  #include <cstdlib>
  #include <stdio.h>
  #include <string>
  #include "unistd.h"
  #include <fstream>
  #include "git_log_parser.tab.hh"
  using namespace std;
  //extern int yylex();
%}
%option noyywrap
%option noline
%option c++
%%
"Author" { std::cout << "Found AUTHOR " << yytext << std::endl;}
"Date" { std::cout << "Found DATE " << yytext << std::endl;}
"commit" { std::cout << "Found COMMIT " << yytext << std::endl;}
[ \t\n]         ;
[0-9]+\.[0-9]+  { cout << "Found a floating-point number:" << yytext << endl; }
[0-9]+          { cout << "Found an integer:" << yytext << endl; }
[a-zA-Z0-9]+    { cout << "Found a string: " << yytext << endl; }
[a-z0-9 _]+@[a-z0-9]+.[a-z]+ { std::cout << "email: " << yytext << std::endl; }
%%

yyFlexLexer *lexer_ptr{};

int yylex(yy::parser::semantic_type *val) {
  return lexer_ptr->yylex();
}

int main(int argc, char** argv) {
  std::string git_log_filename{ "C:\\development\\nce\\vscxggb" };
  std::ifstream log_in {git_log_filename.c_str()};
  yyFlexLexer lexer{ log_in, std::cout};
  lexer_ptr = &lexer;

  while (lexer.yylex())
  {

  }
  log_in.close();

  return 0;
}