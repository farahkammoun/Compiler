#pragma once

#include <stdio.h>


extern FILE* input;
typedef enum { FALSE, TRUE}BOOL;

typedef enum {
    ID, NUM,
    PROGRAM, VAR, BEGIN, END, INTEGER, CHAR, WHILE, DO, READ, WRITE, READLN, WRITELN, IF, THEN, ELSE,
    LPAREN, RPAREN, EQ, NEQ, LT, GT, LTE, GTE, PLUS, MINUS, OR, AND, MULT, DIV, MOD, ENDFILE, SEMI, ERROR, ASSIGN, TWOPTS, COMMA, POINT
}Symbole;

typedef enum {StmtK,ExpK, ProgramK, DclK, TypeK, LidK} NodeKind;
typedef enum {IfK,WhileK,AssignK,ReadK,WriteK, ReadlnK, WritelnK} StmtKind;
typedef enum {OpK,ConstK,IdK} ExpKind;
typedef enum {intK, charK} TypeKind;


/* ExpType is used for type checking */
typedef enum {Void,Integer,Boolean} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp; TypeKind type;} kind;
     union { Symbole op;
             int val;
             char * name; } attr;
     ExpType type; /* for type checking of exps */
} TreeNode;

extern BOOL Error;
