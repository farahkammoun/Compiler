
#include "globals.h"
#include "util.h"
#include "lexical.h"
#include "parse.h"
#include <stdlib.h>


static Symbole token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode* program(void);
static TreeNode* dcl(void);
static TreeNode* liste_id(void);
static TreeNode* type(void);
static TreeNode* inst_composee(void);
static TreeNode* liste_inst(void);
static TreeNode* statement(void);
static TreeNode* exp(void);
static TreeNode* simple_exp(void);
static TreeNode* term(void);
static TreeNode* factor(void);
static TreeNode * if_stmt(void);
static TreeNode * while_stmt(void);
static TreeNode * assign_stmt(void);
static TreeNode * read_stmt(void);
static TreeNode * write_stmt(void);
static TreeNode * readln_stmt(void);
static TreeNode * writeln_stmt(void);

static void syntaxError(const char * message)
{ printf("\n>>> ");
  printf("Syntax error at line %d: %s",lineno,message);
  Error = TRUE;
}

static void match(Symbole expected)
{ if (token == expected) token = getSymbole();
  else {
    syntaxError("unexpected token -> ");
    printSymbole(token,tokenString);
    printf(" >> expected token: ");
    printSymbole(expected,"");

   printf("      ");
  }
}

TreeNode * program(void)
{
    TreeNode* node = newProgramNode();
    match(PROGRAM);
    node->attr.name = copyString(tokenString);
    match(ID);
    match(SEMI);
    TreeNode * t = dcl();
    TreeNode * u = inst_composee();
    node->child[0] = t;
    node->child[1] = u;
    match(POINT);
    return node;
}

TreeNode* dcl(void)
{
    TreeNode *node = NULL;
    TreeNode* tete = node;
    while(token != BEGIN) {
        node = newDclNode();
        match(VAR);
        TreeNode *lid = liste_id();
        match(TWOPTS);
        TreeNode* t = type();
        node->child[0]=lid;
        node->child[1]=t;
        node = node->sibling;
        match(SEMI);
    }
    return (tete);
}


TreeNode* type(void)
{
    switch(token) {
    case INTEGER :
        match(INTEGER);
        return newTypeNode(intK);
        break;
    case CHAR :
        match(CHAR);
        return newTypeNode(charK);
        break;
    default:
        break;
    }
    return newTypeNode(charK);
}

TreeNode* liste_id(void)
{
    TreeNode *node = newLidKNode();
    TreeNode* tete = node;
    node->attr.name = copyString(tokenString);
    match(ID);
    while(token == COMMA) {
        match(COMMA);
        node = node->sibling;
        node = newLidKNode();
        node->attr.name = copyString(tokenString);
        match(ID);
    }
    return (tete);
}

TreeNode* inst_composee(void)
{
    TreeNode* node = NULL;
    match(BEGIN);
    if(token != END) {
        node = liste_inst();
    }
    match(END);
    return node;
}

TreeNode* liste_inst(void)
{
  TreeNode * t = statement();
  TreeNode * p = t;
  while ((token!=END))
  { TreeNode * q;
    match(SEMI);
    q = statement();
    if (q!=NULL) {
      if (t==NULL) t = p = q;
      else /* now p cannot be NULL either */
      { p->sibling = q;
        p = q;
      }
          if (EOF) break;

    }
  }
  return t;
}

TreeNode * statement(void)
{ TreeNode * t = NULL;
  switch (token) {
    case IF : t = if_stmt(); break;
    case WHILE : t = while_stmt(); break;
    case ID : t = assign_stmt(); break;
    case READ : t = read_stmt(); break;
    case WRITE : t = write_stmt(); break;
    case READLN : t = readln_stmt(); break;
    case WRITELN : t = writeln_stmt(); break;
    default : syntaxError("unexpected token -> ");
              printSymbole(token,tokenString);
              token = getSymbole();
              break;

  } /* end case */
  return t;
}

TreeNode * if_stmt(void)
{ TreeNode * t = newStmtNode(IfK);
  match(IF);
  if (t!=NULL) t->child[0] = exp();
  match(THEN);
  if (t!=NULL) t->child[1] = statement();
  match(ELSE);
  if (t!=NULL) t->child[2] = statement();
  return t;
}

TreeNode * while_stmt(void)
{ TreeNode * t = newStmtNode(WhileK);
  match(WHILE);
  if (t!=NULL) t->child[0] = exp();
  match(DO);
  if (t!=NULL) t->child[1] = statement();
  return t;
}

TreeNode * assign_stmt(void)
{ TreeNode * t = newStmtNode(AssignK);
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  match(ASSIGN);
  if (t!=NULL) t->child[0] = simple_exp();
  return t;
}

TreeNode * read_stmt(void)
{ TreeNode * t = newStmtNode(ReadK);
  match(READ);
  match(LPAREN);
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  match(RPAREN);
  return t;
}

TreeNode * readln_stmt(void)
{ TreeNode * t = newStmtNode(ReadlnK);
  match(READLN);
  match(LPAREN);
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  match(RPAREN);
  return t;
}

TreeNode * write_stmt(void)
{ TreeNode * t = newStmtNode(WriteK);
  match(WRITE);
  match(LPAREN);
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  match(RPAREN);
  return t;
}

TreeNode * writeln_stmt(void)
{ TreeNode * t = newStmtNode(WritelnK);
  match(WRITELN);
  match(LPAREN);
  if ((t!=NULL) && (token==ID))
    t->attr.name = copyString(tokenString);
  match(ID);
  match(RPAREN);
  return t;
}

TreeNode * exp(void)
{ TreeNode * t = simple_exp();
  if ((token==LT)||(token==EQ)||(token==NEQ)|| (token==LTE) || (token==GT) || (token==GTE)) {
    TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
    }
    match(token);
    if (t!=NULL)
      t->child[1] = simple_exp();
  }
  return t;
}

TreeNode * simple_exp(void)
{ TreeNode * t = term();
  while ((token==PLUS)||(token==MINUS) || (token==OR))
  { TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token);
      t->child[1] = term();
    }
  }
  return t;
}

TreeNode * term(void)
{ TreeNode * t = factor();
  while ((token==MULT)||(token==DIV) || (token == MOD) || (token == AND))
  { TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token);
      p->child[1] = factor();
    }
  }
  return t;
}

TreeNode * factor(void)
{ TreeNode * t = NULL;
  switch (token) {
    case NUM :
      t = newExpNode(ConstK);
      if ((t!=NULL) && (token==NUM))
        t->attr.val = atoi(tokenString);
      match(NUM);
      break;
    case ID :
      t = newExpNode(IdK);
      if ((t!=NULL) && (token==ID))
        t->attr.name = copyString(tokenString);
      match(ID);
      break;
    case LPAREN :
      match(LPAREN);
      t = simple_exp();
      match(RPAREN);
      break;
    default:
      syntaxError("unexpected token -> ");
      printSymbole(token,tokenString);
      token = getSymbole();
      break;
    }
  return t;
}

/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
 * constructed syntax tree
 */
TreeNode * parse(void)
{ TreeNode * t;
  token = getSymbole();
  t = program();
  if (token!=ENDFILE)
    syntaxError("Code ends before file\n");
  return t;
}
