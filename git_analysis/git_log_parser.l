%{
  #include <iostream>
  #include <cstdlib>
  #include <stdio.h>
  #include <string>
  #include "unistd.h"
  #include <fstream>
  #include "git_log_parser.tab.hh"
  #include "GitCommitCreator.h"  
  #include "../platform_os/platform_os/WindowsPlatform.h"
  #include <cassert>

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
[\n\r] {return yy::parser::token::GL_NEWLINE; }
[ \t]        {} 

[:] { 
  create_git_log_node(GitLogNode::NodeType::colon); 
  return yy::parser::token::GL_COLON; 
  }

[a-zA-Z0-9/\\<>.@'\"+\-*_=]+ {
  create_git_log_node(GitLogNode::NodeType::text,yytext);   
  return yy::parser::token::GL_STRING;
  }

^"Author:" { 
  create_git_log_node(GitLogNode::NodeType::author);  
  return yy::parser::token::GL_AUTHOR; 
  }

^"Date:" { 
  create_git_log_node(GitLogNode::NodeType::date); 
  return yy::parser::token::GL_DATE;
  }

^"commit " { 
  create_git_log_node(GitLogNode::NodeType::commit);  
  return yy::parser::token::GL_COMMIT;  
  }

^"Merge:" { 
  create_git_log_node(GitLogNode::NodeType::merge);  
  return yy::parser::token::GL_MERGE;  
  }
%%

yyFlexLexer *lexer_ptr{};

int yylex(yy::parser::semantic_type *val) {
  yyval = val;
  return lexer_ptr->yylex();
}

void yy::parser::error (const std::string& msg)
{
  std::cerr << "Error: " << msg << std::endl;
  assert(false);
}

int main(int argc, char** argv) {
  //std::string git_log_filename{ "C:\\development\\nce\\vscxggbwkfnqduxwfnfo" };
  GB::WindowsPlatform os{};
	std::cout << "git log running..." << std::endl;
	std::string batch_filename{ os.get_random_string(20) };
	batch_filename.append(os.get_shell_extension());
	std::ofstream batch_file{ batch_filename.c_str() };
	std::string git_log_filename{ os.get_random_string(20) };	
	batch_file << "git.exe log >";
	batch_file << git_log_filename << std::endl;
  batch_file.close();
  os.run_application("", batch_filename);

  std::ifstream log_in {git_log_filename.c_str()};
  yyFlexLexer lexer{ log_in, std::cout};
  lexer_ptr = &lexer;

  yy::parser parser{};
  parser.parse();
  log_in.close();
  os.delete_file(batch_filename);
  os.delete_file(git_log_filename);
  GitCommitCreator creator {end_node_ptr};
  creator.process(os);
  return 0;
}