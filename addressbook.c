#include "addressbook.h"
#include "helpers.h"

int main(int argc, char ** argv)
{
  TelephoneBookList* list = NULL;

  displayStudentDetails();

  /* Load file from command line if available. */
  if(argc > 1)
  {
    printf("> File name specified through command-line argument.\n");
    parseLoad(&list, argv[EXPECTED_ARG_INDEX]);
    if(argc > 2)
      printf("> Additional args were ignored.\n");
  }

  while(readCommand(&list) == TRUE);

  printf("> Goodbye. \n\n");

  return EXIT_SUCCESS;
}

void displayStudentDetails()
{
  printf("%s \n", LINE_SPLIT);
  printf("%s: %s \n", "Student name", STUDENT_NAME);
  printf("%s: %s \n", "Student number", STUDENT_NUM);
  printf("%s \n", PROGRAM_TITLE);
  printf("%s \n\n", LINE_SPLIT);
}

Boolean readCommand(TelephoneBookList** list)
{
  char buffer[BUFFER_SIZE];
  char* tokens[TOKEN_MAX];
  char* curToken;
  int i;

  /* Obtain user input. */
  printf("Enter your command: ");
  fgets(buffer, sizeof(buffer), stdin);

  /* Handle overflow */
  if(buffer[strlen(buffer) - 1] != '\n')
    readRestOfLine();

  /* Tokenize command */
  initTokenBuffer(tokens); /*We clear the token buffer*/
  curToken = strtok(buffer, TOKEN_DELIMS);
  for(i = 0; i < TOKEN_MAX && curToken != NULL; ++i)
  {
    tokens[i] = curToken;
    tokens[i][strcspn(tokens[i], "\n")] = '\0'; /* Strip newline. */

    curToken = strtok (NULL, TOKEN_DELIMS);
  }

  return lookupCommand(list, tokens);
}

Boolean lookupCommand(TelephoneBookList** list, char* tokens[TOKEN_MAX])
{
  char* command = tokens[CMND_INDEX];
  /*Execute relevant function based on command*/
  if(strcmp(command, COMMAND_QUIT) == 0 && tokens[1] == NULL)
  {
    parseUnload(list);
    return FALSE;
  }
  /* We check tokens[] in each if statement to ensure correct arg count input*/
  if(strcmp(command, COMMAND_LOAD) == 0 && tokens[2] == NULL)
    parseLoad(list, tokens[1]);
  else if(strcmp(command, COMMAND_UNLOAD) == 0 && tokens[1] == NULL)
    parseUnload(list);
  else if(strcmp(command, COMMAND_DISPLAY) == 0 && tokens[1] == NULL)
    parseDisplay(*list);
  else if(strcmp(command, COMMAND_FORWARD) == 0 && tokens[2] == NULL)
    parseForward(*list, tokens[1]);
  else if(strcmp(command, COMMAND_BACKWARD) == 0 && tokens[2] == NULL)
    parseBackward(*list, tokens[1]);
  else if(strcmp(command, COMMAND_INSERT) == 0 && tokens[4] == NULL)
    parseInsert(*list, tokens[1], tokens[2], tokens[3]);
  else if(strcmp(command, COMMAND_FIND) == 0 && tokens[2] == NULL)
    parseFind(*list, tokens[1]);
  else if(strcmp(command, COMMAND_DELETE) == 0 && tokens[1] == NULL)
    parseDelete(*list);
  else if(strcmp(command, COMMAND_REVERSE) == 0 && tokens[1] == NULL)
    parseReverse(*list);
  else if(strcmp(command, COMMAND_SAVE) == 0 && tokens[2] == NULL)
    parseSave(*list, tokens[1]);
  else if(strcmp(command, COMMAND_SORT) == 0 && tokens[1] != NULL &&
   tokens[2] == NULL)
  {
    if(strcmp(tokens[1], COMMAND_SORT_NAME) == 0)
      parseSortName(*list);
    else if(strcmp(tokens[1], COMMAND_SORT_RANDOM) == 0)
      parseSortRandom(*list);
    else
      printf("> Error: Invalid command - try again.\n");
  }
  else printf("> Error: Invalid command - try again.\n");

  return TRUE;
}

void initTokenBuffer(char* tokens[TOKEN_MAX])
{
  int i;

  for(i = 0; i < TOKEN_MAX; ++i)
  {
    tokens[i] = NULL;
  }
}

Boolean assertArgCount(char* tokens[TOKEN_MAX], int count)
{
  int i, internalCount = 0;

  for(i = 1; i < TOKEN_MAX; ++i)
  {
    if(tokens[i] == NULL)
      internalCount++;
  }

  if(internalCount == count)
   return TRUE;
  else
    return FALSE;
}

void parseLoad(TelephoneBookList** list, char* fileName)
{
  if(fileName == NULL || fileName[0] == '\0')
  {
    printf("> Error: load <string> expected.\n");
    return;
  }

  /* We need to unload any existing memory otherwise we could lose ptr
     to memory.*/
  parseUnload(list);

  *list = commandLoad(fileName);
}

void parseUnload(TelephoneBookList** list)
{
  if(*list == NULL)
    return;

  commandUnload(*list);
  *list = NULL; /* We null the ptr as free doesn't. */
}

void parseDisplay(TelephoneBookList* list)
{
  commandDisplay(list);
}

void parseForward(TelephoneBookList* list, char* moves)
{
  int m;

  if(list == NULL)
  {
    printf("> Error: Load a list before trying to forward.\n");
    return;
  }

  if(moves == NULL)
  {
    printf("> Error: forward <int> expected.\n");
    return;
  }

  m = atoi(moves);

  commandForward(list, m);
}

void parseBackward(TelephoneBookList* list, char* moves)
{
  int movesConverted;

  if(list == NULL)
  {
    printf("> Error: Load a list before trying to backward.\n");
    return;
  }

  if(moves == NULL)
  {
    printf("> Error: backward <int> expected.\n");
    return;
  }

  movesConverted = atoi(moves);
  commandBackward(list, movesConverted);
}

void parseInsert(TelephoneBookList* list, char* id, char* name, char* telephone)
{
  int idConverted;

  if(list == NULL)
  {
    printf("> Error: Load a list before trying to insert.\n");
    return;
  }

  if(id == NULL || name == NULL || telephone == NULL)
  {
    printf("> Error: insert <int> <string> <int> expected.\n");
    return;
  }

  idConverted = atoi(id);

  if(strlen(telephone) != 10)
  {
    printf("> Error: Telephone must be 10 digits in length.\n");
    return;
  }
  else
  {
    int i = 0, len = strlen(telephone);
    for(; i < len; ++i)
    {
      if(!isdigit(telephone[i]))
      {
        printf("> Error: Telephone must be digits.\n");
        return;
      }
    }
  }

  if(idConverted < 0 || idConverted > 9999) /* Acceptable ID range. */
  {
    printf("> Error: ID should be in the range 0 - 9999.\n");
    return;
  }

  commandInsert(list, idConverted, name, telephone);
}

void parseFind(TelephoneBookList* list, char* name)
{
  if(list == NULL)
  {
    printf("> Error: Load a list before trying to find.\n");
    return;
  }

  if(name == NULL)
  {
    printf("> Error: find <string> expected.\n");
    return;
  }

  commandFind(list, name);
}

void parseDelete(TelephoneBookList* list)
{
  if(list == NULL)
  {
    printf("> Error: Load a list before trying to delete.\n");
    return;
  }

  if(list->current == NULL)
  {
    printf("> Error: Use find to select a node first.\n");
    return;
  }

  commandDelete(list);
}

void parseReverse(TelephoneBookList* list)
{
  if(list == NULL)
  {
    printf("> Error: Load a list before trying to reverse.\n");
    return;
  }

  commandReverse(list);
}
void parseSave(TelephoneBookList* list, char* fileName)
{
  if(list == NULL)
  {
    printf("> Error: Load a list before trying to save.\n");
    return;
  }

  if(fileName == NULL || strcmp(fileName, "") == 0)
  {
    printf("> Error: save <string> expected.\n");
    return;
  }

  commandSave(list, fileName);
}

void parseSortName(TelephoneBookList* list)
{
  if(list == NULL)
  {
    printf("> Error: Load a list before trying to sort name.\n");
    return;
  }

  commandSortName(list);
}

void parseSortRandom(TelephoneBookList* list)
{
  if(list == NULL)
  {
    printf("> Error: Load a list before trying to sort random.\n");
    return;
  }

  commandSortRandom(list);
}
