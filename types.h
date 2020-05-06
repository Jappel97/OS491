#ifndef TYPES_H
#define TYPES_H

typedef struct tab{
	int numEntries;
	char** entries;
	void* (*functions)();
} menuTab;

typedef struct menu{
	unsigned char set;
	int numTabs;
	menuTab *tabs;
} menu;

typedef struct stack{
	int maxsize;
	int top;
	double* items;
} stack;

typedef struct thing{
	int type;
	double value;
} thing;

typedef struct queue{
	int maxsize;
	int tail;
	thing* items;
} queue;

typedef struct pair{
	char key;
	int value;
} KVPair;

typedef struct twosome{
	double value1;
	double value2;
} UnorderedPair;

#endif
