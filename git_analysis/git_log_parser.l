%{
  #include <iostream>
  #include <cstdlib>
  #include <stdio.h>
  #include <string>
  #include "unistd.h"
  #include <fstream>
  #include "git_log_parser.tab.hh"
  using namespace std;
  using namespace GB;
  //extern int yylex();
  yy::parser::semantic_type *yyval {};
  void get_string(char* yytext){
    yyval->sval = new char[strlen(yytext)+1];
    sprintf(yyval->sval,"%s",yytext);  
  }

  GB::GitLogNode * end_node_ptr{};

  void create_git_log_node(const GitLogNode::NodeType &type, char* yytext = NULL, int num = 0){
    if(!yytext) {
      yyval->node_ptr = new GitLogNode(type);
    }
    else if(type == GitLogNode::NodeType::number){
      yyval->node_ptr = new GitLogNode(GitLogNode::NodeType::number);
      yyval->node_ptr->set_number(num);
    }
    else if(type == GitLogNode::NodeType::email || 
    type == GitLogNode::NodeType::time ||
    type == GitLogNode::NodeType::time_zone){
      yyval->node_ptr = new GitLogNode(type);
      yyval->node_ptr->set_text(yytext);
    }
    else {
      yyval->node_ptr = new GitLogNode(yytext);
    }

    if(end_node_ptr){
      end_node_ptr->set_next_node(yyval->node_ptr);  
    }

    end_node_ptr =  yyval->node_ptr;    
  }  
%}
%option noyywrap
%option noline
%option c++
%%

"Author" { 
  create_git_log_node(GitLogNode::NodeType::author);  
  return yy::parser::token::GL_AUTHOR; 
  }

"Date" { 
  create_git_log_node(GitLogNode::NodeType::date); 
  return yy::parser::token::GL_DATE;
  }

"commit" { 
  create_git_log_node(GitLogNode::NodeType::commit);  
  return yy::parser::token::GL_COMMIT;  
  }
  
[\n\r] {return yy::parser::token::GL_NEWLINE; }
[ \t]        {} 
[<>:]        { return yy::parser::token::GL_SPACE;} 

[0-9]+ { 
  create_git_log_node(GitLogNode::NodeType::number,NULL,atoi(yytext));  
  return yy::parser::token::GL_NUMBER;
  }

[a-zA-Z0-9]+ {
  create_git_log_node(GitLogNode::NodeType::text,yytext);   
  return yy::parser::token::GL_STRING;
  }

[a-z0-9 _]+@[a-z0-9]+.[a-z]+ { 
  create_git_log_node(GitLogNode::NodeType::email,yytext);
  return yy::parser::token::GL_EMAIL;
  }
[0-9]+:[0-9]+:[0-9]+ { 
  create_git_log_node(GitLogNode::NodeType::time,yytext);
  return yy::parser::token::GL_TIME;
  }
[+0-9]+ { 
  create_git_log_node(GitLogNode::NodeType::time_zone,yytext);
  return yy::parser::token::GL_TIME_ZONE;
  }
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