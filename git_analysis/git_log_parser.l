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
  yy::parser::semantic_type *yyval {};
  void get_string(char* yytext){
    yyval->sval = new char[strlen(yytext)+1];
    sprintf(yyval->sval,"%s",yytext);  
  }
%}
%option noyywrap
%option noline
%option c++
%%

"Author" { get_string(yytext); return yy::parser::token::GL_AUTHOR; }
"Date" { get_string(yytext); return yy::parser::token::GL_DATE;}
"commit" { get_string(yytext); return yy::parser::token::GL_COMMIT;  }
[ \t\n\r]        {} ;
[0-9]+          { yyval->ival = atoi(yytext); return yy::parser::token::GL_NUMBER;}
[a-zA-Z0-9]+    { get_string(yytext); return yy::parser::token::GL_STRING;}
[a-z0-9 _]+@[a-z0-9]+.[a-z]+ { get_string(yytext); return yy::parser::token::GL_EMAIL;}
%%

yyFlexLexer *lexer_ptr{};

int yylex(yy::parser::semantic_type *val) {
  yyval = val;
  return lexer_ptr->yylex();
}

void yy::parser::error (const std::string& msg)
{

}

int main(int argc, char** argv) {
  std::string git_log_filename{ "C:\\development\\nce\\vscxggb" };
  std::ifstream log_in {git_log_filename.c_str()};
  yyFlexLexer lexer{ log_in, std::cout};
  lexer_ptr = &lexer;

  yy::parser parser{};

  parser.parse();

  // while (lexer.yylex())
  // {

  // }
  log_in.close();

  return 0;
}