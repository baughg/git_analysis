%{
  #include <iostream>
  #include <cstdlib>
  #include <stdio.h>
  #include <string>
  #include "unistd.h"
  #include <fstream>
  #include "GitLogNode.h"
  using namespace std;
 #include "git_log_parser.tab.hh"
  extern int yylex(yy::parser::semantic_type *val);
  extern int yyparse();
  
%}
%language "c++"
%code requires {   
  #include "GitLogNode.h" 
  }
%union {
  int ival;
  char* sval;
  GB::GitLogNode* node_ptr;
}

%token <node_ptr> GL_AUTHOR
%token <node_ptr> GL_DATE
%token <node_ptr> GL_COMMIT
%token <node_ptr> GL_STRING
%token <node_ptr> GL_MERGE
%token <node_ptr> GL_COLON
%token <node_ptr> GL_MERGE_PULL

%token GL_NEWLINE
%token GL_WHITESPACE

%%
prog :  | commit_entries 
                  ;
substring : GL_STRING
            | GL_WHITESPACE                      
            ;
string_list : substring
                  | substring string_list                  
                                    ; 
merge_option :    | GL_MERGE string_list new_line
                  ;   
commit_entry :  GL_COMMIT 
                string_list new_line 
                merge_option
                GL_AUTHOR  
                string_list new_line
                GL_DATE  
                string_list                 
                GL_COLON string_list 
                GL_COLON string_list new_line               
                mult_line_commit_msg
                  ;  
commit_entries :  commit_entry 
                  | commit_entry commit_entries
                  ; 

new_line : GL_NEWLINE 
                  | GL_NEWLINE new_line
                  ;

new_commit_prefix : GL_WHITESPACE GL_WHITESPACE GL_WHITESPACE GL_WHITESPACE
                  ;


mult_line_commit_msg : commit_msg new_line
                      | commit_msg new_line mult_line_commit_msg;

colon_list : GL_COLON
            | GL_COLON colon_list
            ;

string_with_colon : string_list
                    | string_list colon_list string_with_colon;

commit_msg :      new_commit_prefix                 
                  | new_commit_prefix string_with_colon                 
                  ;
%%