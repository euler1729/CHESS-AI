#include "ArrayList.h"



SPArrayList *ArrayListCreate(int maxSize)
{
    //checks array size validity
    if (maxSize <= 0)
    {
        return NULL;
    }

    //allocate memory for array(uses for move list)
    SPArrayList *arr = (SPArrayList *)malloc(sizeof(SPArrayList));
    if (!arr)
    { //check validity of allocation
        free(arr);
        return NULL;
    }

    //allocate memory for array(uses for history)
    elem **element = (elem **)malloc(maxSize * (sizeof(elem *)));
    if (!element)
    { //check validity
        free(arr);
        return NULL;
    }

    for (int i = 0; i < maxSize; ++i)
    {
        element[i] = (elem *)malloc(sizeof(elem));
        if (!element[i])
        {
            for (int j = 0; j < i; ++j)
            {
                free(element[j]);
            }
            free(element);
            free(arr);
            return NULL;
        }
    }
    arr->maxSize = maxSize;
    arr->elements = element;
    arr->actualSize = 0;

    return arr;
}





void copyParams(SPArrayList *src, SPArrayList *arr)
{
    // copies elements - each element represents one move
    for (int i = 0; i < src->actualSize; ++i)
    {
        arr->elements[i]->destinationCol = src->elements[i]->destinationCol;
        arr->elements[i]->destinationRow = src->elements[i]->destinationRow;
        arr->elements[i]->startCol = src->elements[i]->startCol;
        arr->elements[i]->startRow = src->elements[i]->startRow;
        arr->elements[i]->fig = src->elements[i]->fig;
        arr->elements[i]->prevFig = src->elements[i]->prevFig;
        arr->elements[i]->gameStatus = src->elements[i]->gameStatus;
        arr->elements[i]->player = src->elements[i]->player;
    }
}
bool ArrayListIsFull(SPArrayList *src)
{
    if ((!src) || ((src->actualSize) < (src->maxSize)))
    {
        return false;
    }
    return true;
}
bool ArrayListIsEmpty(SPArrayList *src)
{
    if ((!src) || ((src->actualSize) != 0))
    {
        return false;
    }
    return true;
}
int ArrayListSize(SPArrayList *src)
{
    if (!src)
    {
        return -1;
    }
    return src->actualSize;
}
int ArrayListMaxCapacity(SPArrayList *src)
{
    if (!src)
    {
        return -1;
    }
    return src->maxSize;
}


SPArrayList *ArrayListCopy(SPArrayList *src)
{
    if (src == NULL)
    {
        return NULL;
    }
    //Creates new array list
    SPArrayList *arr = ArrayListCreate(src->maxSize);
    if (!arr)
    {
        return NULL;
    }
    arr->actualSize = src->actualSize;
    copyParams(src, arr);
    return arr;
}

void ArrayListDestroy(SPArrayList *src)
{
    if (src != NULL)
    {
        for (int i = 0; i < src->maxSize; ++i)
        {
            free(src->elements[i]);
        }
        free(src->elements);
        free(src);
    }
}
ARRAY_LIST_MESSAGE ArrayListClear(SPArrayList *src)
{
    if (!src)
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    src->actualSize = 0;
    return SP_ARRAY_LIST_SUCCESS;
}
//gets index between zero to actualSize-1
ARRAY_LIST_MESSAGE ArrayListAddAt(SPArrayList *src, elem *element, int index)
{
    if (!src || (index < 0 || index > (src->maxSize) - 1) || index > (src->actualSize))
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    if (src->actualSize == src->maxSize)
    {
        return SP_ARRAY_LIST_FULL;
    }
    for (int i = src->actualSize; i > index; --i)
    {
        src->elements[i]->destinationCol = src->elements[i - 1]->destinationCol;
        src->elements[i]->destinationRow = src->elements[i - 1]->destinationRow;
        src->elements[i]->startCol = src->elements[i - 1]->startCol;
        src->elements[i]->startRow = src->elements[i - 1]->startRow;
        src->elements[i]->fig = src->elements[i - 1]->fig;
        src->elements[i]->prevFig = src->elements[i - 1]->prevFig;
        src->elements[i]->gameStatus = src->elements[i - 1]->gameStatus;
        src->elements[i]->player = src->elements[i - 1]->player;
    }
    src->elements[index]->destinationCol = element->destinationCol;
    src->elements[index]->destinationRow = element->destinationRow;
    src->elements[index]->startCol = element->startCol;
    src->elements[index]->startRow = element->startRow;
    src->elements[index]->fig = element->fig;
    src->elements[index]->prevFig = element->prevFig;
    src->elements[index]->gameStatus = element->gameStatus;
    src->elements[index]->player = element->player;
    src->actualSize = src->actualSize + 1;
    
    return SP_ARRAY_LIST_EMPTY;
}
ARRAY_LIST_MESSAGE ArrayListAddFirst(SPArrayList *src, elem *element)
{
    if (!src)
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    return ArrayListAddAt(src, element, 0);
}
ARRAY_LIST_MESSAGE ArrayListAddLast(SPArrayList *src, elem *element)
{
    if (!src)
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    if (ArrayListIsFull(src))
    {
        return SP_ARRAY_LIST_FULL;
    }
    return ArrayListAddAt(src, element, src->actualSize);
}

ARRAY_LIST_MESSAGE ArrayListRemoveAt(SPArrayList *src, int index)
{
    if (!src || (index < 0 || index > (src->actualSize) - 1))
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    if (src->actualSize == 0)
    {
        return SP_ARRAY_LIST_EMPTY;
    }
    for (int i = index; i < src->actualSize - 1; ++i)
    {
        src->elements[i]->destinationCol = src->elements[i + 1]->destinationCol;
        src->elements[i]->destinationRow = src->elements[i + 1]->destinationRow;
        src->elements[i]->startCol = src->elements[i + 1]->startCol;
        src->elements[i]->startRow = src->elements[i + 1]->startRow;
        src->elements[i]->fig = src->elements[i + 1]->fig;
        src->elements[i]->prevFig = src->elements[i + 1]->prevFig;
        src->elements[i]->gameStatus = src->elements[i + 1]->gameStatus;
        src->elements[i]->player = src->elements[i + 1]->player;
    }
    src->actualSize = src->actualSize - 1;
    return SP_ARRAY_LIST_SUCCESS;
}

ARRAY_LIST_MESSAGE ArrayListRemoveFirst(SPArrayList *src)
{
    if (!src)
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    if (ArrayListIsEmpty(src))
    {
        return SP_ARRAY_LIST_EMPTY;
    }
    return ArrayListRemoveAt(src, 0);
}
ARRAY_LIST_MESSAGE ArrayListRemoveLast(SPArrayList *src)
{
    if (!src)
    {
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    if (ArrayListIsEmpty(src))
    {
        return SP_ARRAY_LIST_EMPTY;
    }
    return ArrayListRemoveAt(src, (src->actualSize) - 1);
}
elem* ArrayListGetAt(SPArrayList* src, int index){

    if(index<0){
        printf("error- index<0\n");
        return NULL;
    }
    else if(index>src->actualSize-1){
        printf("error- index > size\n");
        return NULL;
    }
    else if(!src){
        printf("error - source is NULL\n");
        return NULL;
    }


	// if (index<0 || index>src->actualSize-1 || (!src)) //checks validity of the input
	// {
    //     printf("error\n");
    //     return NULL;
    // }	
	else
	{
        return src->elements[index];
    }	
}

elem *ArrayListGetFirst(SPArrayList *src)
{
    if (!src)
    {
        return NULL;
    }
    return ArrayListGetAt(src, 0);
}
elem *ArrayListGetLast(SPArrayList *src)
{
    if (!src)
    {
        printf("error\n");
        return NULL;
    }
    return ArrayListGetAt(src,(src->actualSize)-1);
}