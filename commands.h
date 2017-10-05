#ifndef COMMANDS_H
#define COMMANDS_H

#include "addressbook_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define COMMAND_LOAD "load"
#define COMMAND_UNLOAD "unload"
#define COMMAND_DISPLAY "display"
#define COMMAND_FORWARD "forward"
#define COMMAND_BACKWARD "backward"
#define COMMAND_INSERT "insert"
#define COMMAND_FIND "find"
#define COMMAND_DELETE "delete"
#define COMMAND_REVERSE "reverse"
#define COMMAND_SAVE "save"
#define COMMAND_QUIT "quit"
#define COMMAND_SORT "sort"
#define COMMAND_SORT_NAME "name"
#define COMMAND_SORT_RANDOM "random"

TelephoneBookList * commandLoad(char * fileName);
void commandUnload(TelephoneBookList * list);
void commandDisplay(TelephoneBookList * list);
void commandForward(TelephoneBookList * list, int moves);
void commandBackward(TelephoneBookList * list, int moves);
void commandInsert(TelephoneBookList * list, int id, char * name, char * telephone);
void commandFind(TelephoneBookList * list, char * name);
void commandDelete(TelephoneBookList * list);
void commandReverse(TelephoneBookList * list);
void commandSave(TelephoneBookList * list, char * fileName);
void commandSortName(TelephoneBookList * list);
void commandSortRandom(TelephoneBookList * list);

/*
  Calculates lengths for display function
  places result into pointers.
*/
void calculateDisplayLengths(TelephoneBookList* list, int* nameLen);

/* Takes a node and swaps it with its next pointer.
   We also take the list as an arg to update head/tail etc if required.*/
void swapNodeWithNext(TelephoneBookList* list, TelephoneBookNode* node);

/* Handles opening of a file and parsing addresses. */
Boolean parseFile(TelephoneBookList* list, FILE* file);

#endif
