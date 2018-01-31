#ifndef _UTIL_H_
#define _UTIL_H_
#include "lexical.h"
#include "globals.h"

extern int lineno;

TreeNode * newLinstNode();

TreeNode * newLidKNode();

TreeNode * newTypeNode(TypeKind);

TreeNode * newDclNode();

TreeNode * newProgramNode();

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind);

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind);

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString( char * );

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * );

#endif
