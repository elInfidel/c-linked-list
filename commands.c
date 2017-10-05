#include "commands.h"
#include "helpers.h"

/* Important defines used only by this file as such moved here from header.*/

/* File loading defines. */
#define FILE_READ_BUFFER_SIZE 100
#define COMMENT_SYMBOL '#'
#define TOKEN_DELIMS ", "

/* Display command related defines. */
#define DISP_COLUMN_PADDING 2 /* Overall column defines*/
#define DISP_COLUMN_COUNT 5
#define DISP_SECTION_COUNT DISP_COLUMN_COUNT + 1

#define DISP_CUR_WIDTH 3 /* Specific column width defines */
#define DISP_SERIAL_WIDTH 6
#define DISP_ID_WIDTH 4
#define DISP_TEL_WIDTH TELEPHONE_LENGTH - NULL_SPACE

#define DISP_LINE_BREAK "---------------------------------------------------"
#define DISP_SECTION_BREAK '|'
#define DISP_ENTRIES_TXT "Total phone book entries:"

TelephoneBookList * commandLoad(char * fileName)
{
  TelephoneBookList* list;
  FILE* file = fopen(fileName, "r");

  printf("> Opening the file %s.\n", fileName);
  if(file)
  {
    list = createTelephoneBookList();

    printf("> Loading the file ...\n");
    if(parseFile(list, file) == FALSE)
      return NULL;
    printf("> %d phone book entries have been loaded from the file.\n",
     list->size);

    printf("> Closing the file.\n");
    fclose(file);

    list->current = list->head;
    return list;
  }
  else
  {
    printf("> Error: Unable to find the specified file.\n");
    return NULL;
  }
}

Boolean parseFile(TelephoneBookList* list, FILE* file)
{
  char buffer[FILE_READ_BUFFER_SIZE];

  while(fgets(buffer, sizeof(buffer), file ) != NULL)
  {
    TelephoneBookNode* newNode = createTelephoneBookNode();

    if(buffer[0] != COMMENT_SYMBOL)
    {
      int tokenIndex = 0;
      char *curToken = strtok(buffer, TOKEN_DELIMS);

      while(curToken)
      {
        /* Probably a better way to do this without a switch. */
        switch(tokenIndex)
        {
        case 0: newNode->id = atoi(curToken); break;
        case 1:
          strncpy(newNode->name, curToken, sizeof(newNode->name));
          newNode->name[strcspn(newNode->name, "\n")] = '\0'; /* Strip newline. */
          break;
        case 2:
          strncpy(newNode->telephone, curToken, sizeof(newNode->telephone));
          newNode->telephone[strcspn(newNode->telephone, "\n")] = '\0'; /* Strip newline. */
          break;
        default: /* When counter goes over 3 we know the file is malformed */
          printf("> Error: The specified file is in the wrong format and\
cannot be loaded. \n");
          freeTelephoneBookList(list);
          return FALSE;
        }
        curToken = strtok(NULL, TOKEN_DELIMS);
        tokenIndex++;
      }
      insert(list, newNode);
    }
  }
  return TRUE;
}

void commandUnload(TelephoneBookList * list)
{
  freeTelephoneBookList(list);
  printf("> The list is unloaded.\n");
}

void commandDisplay(TelephoneBookList * list)
{
  /* We assume maxNameLen is atleast length of "name" to begin with. */
  int maxNameLen = 4, totalLen = 0, serial = 1, listSizeLen = 0;
  TelephoneBookNode* curNode;
  calculateDisplayLengths(list, &maxNameLen); /* will only if list isn't null*/

  totalLen = DISP_COLUMN_COUNT * DISP_COLUMN_PADDING + DISP_SECTION_COUNT +
  DISP_CUR_WIDTH + DISP_SERIAL_WIDTH + DISP_ID_WIDTH + maxNameLen + DISP_TEL_WIDTH;
  /* Welcome to Hogwarts. This used to be full of magic nums..
     more of a muggle function now*/
  printf("%.*s\n", totalLen, DISP_LINE_BREAK);
  printf("| %-*s | %-*s | %-*s | %-*s | %-*s | \n", DISP_CUR_WIDTH, "Pos",
  DISP_SERIAL_WIDTH, "Serial",
  DISP_ID_WIDTH, "ID", maxNameLen ,"Name", DISP_TEL_WIDTH, "Telephone");
  printf("%.*s\n", totalLen, DISP_LINE_BREAK);

  if(list != NULL)
  {
    curNode = list->head;
    while(curNode != NULL)
    {
      printf("| %-*s | %-*d | %-*d | %-*s | %-*s |\n",
      DISP_CUR_WIDTH, (curNode == list->current)?"CUR":"   ",
      DISP_SERIAL_WIDTH, serial++,
      DISP_ID_WIDTH, curNode->id,
      maxNameLen , curNode->name,
      DISP_TEL_WIDTH, curNode->telephone);

      curNode = curNode->nextNode;
    }
  }
  else
    printf("|%*s|\n", totalLen - 2, " ");

  printf("%.*s\n", totalLen, DISP_LINE_BREAK);
  printf("| %-s %-d %.*s\n", DISP_ENTRIES_TXT, (list != NULL)?list->size:0, 25, " ");
  printf("%.*s \n", totalLen, DISP_LINE_BREAK);
}

void commandForward(TelephoneBookList * list, int moves)
{
  if(forward(list, moves) == FALSE)
    printf("> Error: Move command would go past end of list or list is empty.\n");
}

void commandBackward(TelephoneBookList * list, int moves)
{
  if(backward(list, moves) == FALSE)
    printf("> Error: Move command would go past front of list or list is empty.\n");
}

void commandInsert(TelephoneBookList * list, int id, char * name, char * telephone)
{
  TelephoneBookNode* node;

  node = createTelephoneBookNode();
  node->id = id;
  strncpy(node->name, name, sizeof(node->name));
  strncpy(node->telephone, telephone, sizeof(node->telephone));

  if(insert(list, node) == FALSE)
    printf("> Error: Node with ID %d already exists.\n", id);
}

void commandFind(TelephoneBookList * list, char * name)
{
  if(findByName(list, name) == NULL)
    printf("> Error: Unable to find node.\n");
}

void commandDelete(TelephoneBookList * list)
{
  if(delete(list) == FALSE)
    printf("> Error: Unable to delete node, perhaps the list is empty?\n");
}

void commandReverse(TelephoneBookList * list)
{
  TelephoneBookNode* node, *next, *tmp;

  if(list->size == 0) return;

  node = list->tail;

  /* Reassign list head and tail*/
  tmp = list->head;
  list->head = list->tail;
  list->tail = tmp;

  while(node != NULL)
  {
    next = node->previousNode; /* We are reversing so store prev as next node*/

    /* Swap previous node with next node. */
    tmp = node->nextNode;
    node->nextNode = node->previousNode;
    node->previousNode = tmp;

    node = next;
  }
}

void commandSave(TelephoneBookList * list, char * fileName)
{
  FILE* file;
  TelephoneBookNode* node;

  printf("> Opening the file %s.\n", fileName);
  file = fopen(fileName, "w");

  if(file == NULL)
    fprintf(stderr, "> Error: Unable to create/open file: %s\n", fileName);

  printf("> Writing list content to file ...\n");

  node = list->head;
  while(node != NULL)
  {
    fprintf(file, "%d, %s, %s\n", node->id, node->name, node->telephone);
    node = node->nextNode;
  }

  printf("> Closing the file.\n");
  fclose(file);
}

void commandSortName(TelephoneBookList * list)
{
  TelephoneBookNode* node;
  Boolean isSorted = FALSE;

  if(list->head == NULL || list->size <= 1)
    return;

  /* We pass over the list as many times as required to sort. */
  do
  {
    node = list->head;
    isSorted = TRUE; /* We assume the list is sorted until we find contradiction*/
    while(node != NULL)
    {
      TelephoneBookNode* tmpNext;

      if(node->nextNode == NULL)
      {
        list->tail = node;
        break;
      }

      /* Compare node to next and see if they need swapping. */
      if(strcmp(node->name, node->nextNode->name) > 0)
      {
        isSorted = FALSE;
        swapNodeWithNext(list, node);
        break; /* We break because we don't want to skip a node below*/
      }

      node = node->nextNode;
    }

  } while(isSorted != TRUE);
}

/* Understandably the main sort code is duplicated between both sort
   functions.*/
void commandSortRandom(TelephoneBookList * list)
{
  TelephoneBookNode* node;
  Boolean isSorted = FALSE;
  int i;
  time_t t;
  if(list->head == NULL || list->size <= 1)
    return;

  /*Gen unique ID for each node*/
  node = list->head;
  while(node != NULL)
  {
    node->id = rand() % 9999 + 1; /*9999 being ID_MAX*/
    node = node->nextNode;
  }

  /* We pass over the list as many times as required to sort. */
  do
  {
    node = list->head;
    isSorted = TRUE; /* We assume the list is sorted until we find contradiction*/
    while(node != NULL)
    {
      TelephoneBookNode* tmpNext;

      if(node->nextNode == NULL)
      {
        list->tail = node;
        break;
      }

      /* Compare node to next and see if they need swapping. */
      if(node->id > node->nextNode->id)
      {
        isSorted = FALSE;
        swapNodeWithNext(list, node);
        break; /* We break because we don't want to skip a node below*/
      }

      node = node->nextNode;
    }

  } while(isSorted != TRUE);
}

void calculateDisplayLengths(TelephoneBookList* list, int* nameLen)
{
  TelephoneBookNode* curNode;

  if(list == NULL) return;

  curNode = list->head;
  while(curNode != NULL)
  {
    /* Calculating variable length columns */
    int nodeDataLen = strlen(curNode->name);
    if(nodeDataLen > *nameLen)
      *nameLen = nodeDataLen;

    curNode = curNode->nextNode;
  }
}

void swapNodeWithNext(TelephoneBookList* list, TelephoneBookNode* node)
{
  TelephoneBookNode* tmpNext;

  tmpNext = node->nextNode->nextNode;

  /* Catch head/tail cases. */
  if(list->head == node)
    list->head = node->nextNode;
  if(node->nextNode == list->tail)
    list->tail = node;

  /* Swap nodes. */
  node->nextNode->previousNode = node->previousNode; /*Node 2*/
  node->nextNode->nextNode = node;
  if(node->previousNode != NULL)
    node->previousNode->nextNode = node->nextNode;

  node->previousNode = node->nextNode;/* Node 1*/
  node->nextNode = tmpNext;
  if(node->nextNode != NULL)
    node->nextNode->previousNode = node;
}
