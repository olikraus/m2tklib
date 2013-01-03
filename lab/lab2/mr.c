/*
  
  mr.c
  
*/

#include <stdlib.h>
#include <string.h>
#include "mr.h"


/*================================================================*/
/* mr global */

mr_type mr_global = NULL;



/*================================================================*/
/* mr procedures */

mr_type mr_Open(void)
{
  mr_type mr;
  mr = malloc(sizeof(mr_struct));
  if ( mr != NULL )
  {
    mr->m2_element_list = b_pl_Open();
    if ( mr->m2_element_list != NULL )
    {
      mr->m2_list_list = b_pl_Open();
      if ( mr->m2_list_list != NULL )
      {
	mr->m2_str_pool = b_pl_Open();
	if ( mr->m2_str_pool != NULL )
	{
	  mr->u8_list = NULL;
	  return mr;
	}
	b_pl_Close(mr->m2_list_list);
      }
      b_pl_Close(mr->m2_element_list);
    }
    free(mr);
  }
  return NULL;
}

void mr_Clear(mr_type mr)
{
  int i;
  i = -1;
  while ( mr_WhileM2List(mr, &i) )
  {
    if ( mr_GetM2List(mr, i) != NULL )
    {
      free(mr_GetM2List(mr, i));
    }
  }
  b_pl_Clear(mr->m2_list_list);

  i = -1;
  while ( mr_WhileM2Element(mr, &i) )
  {
    if ( mr_GetM2Element(mr, i) != NULL )
    {
      free(mr_GetM2Element(mr, i));
    }
  }
  b_pl_Clear(mr->m2_element_list);
  
  i = -1;
  while ( mr_WhileM2Str(mr, &i) )
  {
    if ( mr_GetM2Str(mr, i) != NULL )
    {
      free((void *)mr_GetM2Str(mr, i));
    }
  }
  b_pl_Clear(mr->m2_str_pool);
  
  if ( mr->u8_list != NULL )
    free(mr->u8_list);
  mr->u8_list = NULL;
  mr->u8_cnt = 0;
}

void mr_Close(mr_type mr)
{
  mr_Clear(mr);
  b_pl_Close(mr->m2_list_list);
  b_pl_Close(mr->m2_element_list);
  b_pl_Close(mr->m2_str_pool);
  free(mr);
}

/*================================================================*/
/* element */

/*
  Add an element with the specified size.
  Returns list pos or -1 for error 
*/
int mr_AddElement(mr_type mr, size_t size)
{
  int pos;
  void *element;
  element = malloc(size);
  if ( element != NULL )
  {
    pos = b_pl_Add(mr->m2_element_list, element);
    if ( pos >= 0 )
    {
      return pos;
    }
    free(element);
  }
  return -1;  
}


/*================================================================*/
/* list */

/*
  Add a list with the specified length.
  Returns list pos or -1 for error 
*/
int mr_AddList(mr_type mr, int list_entries)
{
  int pos;
  void *list;
  list = malloc(sizeof(void *)*list_entries);
  if ( list != NULL )
  {
    pos = b_pl_Add(mr->m2_list_list, list);
    if ( pos >= 0 )
    {
      return pos;
    }
    free(list);
  }
  return -1;
}

void mr_SetListElement(mr_type mr, int pos, int el_idx, void *element)
{
  void **list;
  list = mr_GetM2List(mr, pos);
  list[el_idx] = element;
}

/*================================================================*/
/* string */

/*
  Add a string to the internal str pool.
  Returns list pos or -1 for error 
*/
int mr_AddStr(mr_type mr, const char *str)
{
  int pos;
  char *s;
  
  if ( str == NULL )
    str = "";
  s = strdup(str);
  if ( s != NULL )
  {
    pos = b_pl_Add(mr->m2_str_pool, s);
    if ( pos >= 0 )
    {
      return pos;
    }
    free(s);
  }
  return -1;  
}

/*================================================================*/
/* u8 */

uint8_t *mr_AddU8(mr_type mr)
{
  if ( mr->u8_list == NULL )
  {
    mr->u8_list = (uint8_t *)malloc(sizeof(uint8_t) * 1);
    mr->u8_cnt = 1;
  }
  else
  {
    void *ptr;
    ptr = realloc(mr->u8_list, mr->u8_cnt+1);
    if ( ptr == NULL )
      return NULL;
    mr->u8_list = (uint8_t *)ptr;
    mr->u8_cnt++;    
  }
  mr->u8_list[mr->u8_cnt-1] = 0;
  return &(mr->u8_list[mr->u8_cnt-1]);
}


/*================================================================*/
/* mr global API */

int mrg_Init(void)
{
  mrg_Destroy();
  mr_global = mr_Open();
  if ( mr_global == NULL )
    return 0;
  return 1;
}

void mrg_Destroy(void)
{
  if ( mr_global != NULL )
    mr_Close(mr_global);
  mr_global = NULL;
}

void mrg_Clear(void)
{
  if ( mr_global != NULL )
    mr_Clear(mr_global);
}

int mrg_AddElement(size_t size)
{
  return mr_AddElement(mr_global, size);
}

int mrg_AddList(int list_entries)
{
  return mr_AddList(mr_global, list_entries);
}

void mrg_SetListElement(int pos, int el_idx, void *element)
{
  mr_SetListElement(mr_global, pos, el_idx, element);
}

int mrg_AddStr(const char *s)
{
  return mr_AddStr(mr_global, s);
}

const char *mrg_StoreStr(const char *s)
{
  int pos;
  pos = mrg_AddStr(s);
  if ( pos < 0 )
    return NULL;
  return mrg_GetM2Str(pos);
}

uint8_t *mrg_AddU8(void)
{
  return mr_AddU8(mr_global);
}

