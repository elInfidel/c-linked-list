#ifndef ADDRESSBOOK_LIST_H
#define ADDRESSBOOK_LIST_H

#include "helpers.h"

#define NAME_LENGTH (20 + NULL_SPACE)
#define TELEPHONE_LENGTH (10 + NULL_SPACE)

#define NODE_MINIMUM_ID 1

typedef struct telephoneBookNode
{
    int id;
    char name[NAME_LENGTH];
    char telephone[TELEPHONE_LENGTH];
    struct telephoneBookNode * previousNode; /* The previous node in the linked-list */
    struct telephoneBookNode * nextNode; /* The next node in the linked-list */
} TelephoneBookNode;

typedef struct telephoneBookList
{
    TelephoneBookNode * head;
    TelephoneBookNode * tail;
    TelephoneBookNode * current;
    unsigned size;
} TelephoneBookList;

TelephoneBookList * createTelephoneBookList();
void freeTelephoneBookList(TelephoneBookList * list);

TelephoneBookNode * createTelephoneBookNode();
void freeTelephoneBookNode(TelephoneBookNode * node);

Boolean insert(TelephoneBookList * list, TelephoneBookNode * node);
Boolean forward(TelephoneBookList * list, int forward);
Boolean backward(TelephoneBookList * list, int backward);
Boolean delete(TelephoneBookList * list);

TelephoneBookNode * findByID(TelephoneBookList * list, int id);
TelephoneBookNode * findByName(TelephoneBookList * list, char * name);

#endif
