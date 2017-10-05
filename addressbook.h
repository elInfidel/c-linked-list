#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include "commands.h"

#define LINE_SPLIT "----------------------------------------------\
----------------------"

#define STUDENT_NAME "Liam"
#define STUDENT_NUM "sxxxxxxx"
#define PROGRAM_TITLE "Advanced Programming Techniques"

/* Command line functionality defines*/
#define EXPECTED_ARG_INDEX 1 /* Index that filename passed from commandline is expected */

/* User input defines*/
/* The index at which a command is expected in the tokens array*/
#define CMND_INDEX 0
/* The buffer size for characters in user input array*/
#define BUFFER_SIZE 50
/* The maximum number of tokens to read*/
#define TOKEN_MAX 10
/* Delimiters to use in tokenization */
#define TOKEN_DELIMS ", "

/* Print student details to screen. */
void displayStudentDetails();

/* Reads and calls lookCommand with tokens
   Returns false when exit command is parsed. */
Boolean readCommand(TelephoneBookList** list);

/* Checks the requested command against available commands
   Returns false when exit command is parsed. */
Boolean lookupCommand(TelephoneBookList** list, char* tokens[TOKEN_MAX]);

/* Goes through the token buffer setting each token to null we can then
   use null tokens to check how many tokens were actually entered by the
   user for input validation purposes. */
void initTokenBuffer(char* tokens[TOKEN_MAX]);

/* Checks the number of arguments in the token array and returns, it should
   be noted that the first argument (The command token) will be ignored.*/
Boolean assertArgCount(char* tokens[TOKEN_MAX], int count);

/* Each function validates for a certain command and makes final call.  */
void parseLoad(TelephoneBookList** list, char* fileName);
void parseUnload(TelephoneBookList** list);
void parseDisplay(TelephoneBookList* list);
void parseForward(TelephoneBookList* list, char* moves);
void parseBackward(TelephoneBookList* list, char* moves);
void parseInsert(TelephoneBookList* list, char* id, char* name, char* telephone);
void parseFind(TelephoneBookList* list, char* name);
void parseDelete(TelephoneBookList* list);
void parseReverse(TelephoneBookList* list);
void parseSave(TelephoneBookList* list, char* fileName);
void parseSortName(TelephoneBookList* list);
void parseSortRandom(TelephoneBookList* list);

#endif
