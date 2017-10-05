#include "addressbook_list.h"

TelephoneBookList* createTelephoneBookList()
{
	TelephoneBookList* newListPtr;

	/* Allocate required memory. */
	newListPtr = malloc(sizeof(TelephoneBookList));

	if(newListPtr == NULL)
		exit(EXIT_FAILURE);

	/* Instantiate data. */
	newListPtr->head = NULL;
	newListPtr->tail = NULL;
	newListPtr->current = NULL;
	newListPtr->size = 0;

	return newListPtr;
}

void freeTelephoneBookList(TelephoneBookList* list)
{
	TelephoneBookNode* node;
	/* Free all nodes before freeing list itself*/
	node = list->head;
	while(node != NULL)
	{
		TelephoneBookNode* tmp = node->nextNode;
		freeTelephoneBookNode(node);
		node = tmp;
	}

	free(list);
}

TelephoneBookNode * createTelephoneBookNode()
{
	TelephoneBookNode* newNodePtr;

	/* Allocate required memory. */
	newNodePtr = (TelephoneBookNode*) malloc(sizeof(TelephoneBookNode));

	if(newNodePtr == NULL)
		exit(EXIT_FAILURE);

	/* Instantiate data. */
	newNodePtr->id = 0;
	newNodePtr->name[0] = '\0';
	newNodePtr->telephone[0] = '\0';
	newNodePtr->previousNode = NULL;
	newNodePtr->nextNode = NULL;

  return newNodePtr;
}

void freeTelephoneBookNode(TelephoneBookNode * node)
{
	free(node);
}

Boolean insert(TelephoneBookList* list, TelephoneBookNode* node)
{
	if(findByID(list, node->id) != NULL)
		return FALSE;

	if(list->size == 0)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		node->previousNode = list->tail; /* Set new nodes previous to original tail. */
		list->tail->nextNode = node; /* Set original tails next node to new node. */
		list->tail = node; /* Set new tail to new node. */
		node->nextNode = NULL; /* Set new nextNode to NULL */
	}

	list->current = node;
	list->size++;

  return TRUE;
}

Boolean forward(TelephoneBookList * list, int forward)
{
  int i;
  TelephoneBookNode* originalCurPtr = list->current;

  if(list->current == NULL || (forward < 0))
    return FALSE;

  /* Advance curPtr while checking for end of list*/
  for(i = 0; i < forward; ++i)
  {
    if(list->current->nextNode == NULL)
    {
      list->current = originalCurPtr;
      return FALSE;
    }
    else
      list->current = list->current->nextNode;
  }

  return TRUE;
}

Boolean backward(TelephoneBookList * list, int backward)
{
  int i;
  TelephoneBookNode* originalCurPtr = list->current;

  if(list == NULL || list->current == NULL || (backward < 0))
    return FALSE;

  /* Reverse curPtr while checking for end of list*/
  for(i = 0; i < backward; ++i)
  {
    if(list->current->previousNode == NULL)
    {
      list->current = originalCurPtr;
      return FALSE;
    }
    else
      list->current = list->current->previousNode;
  }

  return TRUE;
}

Boolean delete(TelephoneBookList * list)
{
	TelephoneBookNode* newCurPtr, *previous, *next;

  if(list == NULL || list->current == NULL) return FALSE;

	previous = list->current->previousNode;
	next = list->current->nextNode;

	if(previous != NULL && next != NULL) /* Middle case. */
	{
		next->previousNode = previous;
		previous->nextNode = next;
		newCurPtr = previous;
	}
	else if(previous == NULL && next != NULL) /* Tail case. */
	{
		next->previousNode = NULL;
		list->head = next;
		newCurPtr = next;
	}
	else if(previous != NULL && next == NULL) /* Head case. */
	{
		previous->nextNode = NULL;
		list->tail = previous;
		newCurPtr = previous;
	}
	else if(previous == NULL && next == NULL) /* Single case. */
	{
		list->head = NULL;
		list->tail = NULL;
		newCurPtr = NULL;
	}

	freeTelephoneBookNode(list->current);
	list->size--;
	list->current = newCurPtr;

  return TRUE;
}

TelephoneBookNode * findByID(TelephoneBookList * list, int id)
{
  TelephoneBookNode* curNodePtr;

  if(list == NULL || list->head == NULL)
    return NULL;

  curNodePtr = list->head;

  while(curNodePtr != NULL)
  {
	if(curNodePtr->id == id)
	{
		list->current = curNodePtr;
		return curNodePtr;
	}
	else
		curNodePtr = curNodePtr->nextNode;
  }

  return NULL;
}

TelephoneBookNode* findByName(TelephoneBookList* list, char* name)
{
  TelephoneBookNode* curNodePtr;

  if(list == NULL || list->head == NULL)
    return NULL;

  curNodePtr = list->head;

  while(curNodePtr != NULL)
  {
    if(strcmp(curNodePtr->name, name) == 0)
		{
			list->current = curNodePtr;
			return curNodePtr;
		}
		else
			curNodePtr = curNodePtr->nextNode;
  }

  return NULL;
}
