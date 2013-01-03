/*

  mr.h
  
  Menu Runtime Enviornment
  
*/

#ifndef _MR_H
#define _MR_H

#include "b_pl.h"
#include <stdint.h>

struct _mr_struct
{
  b_pl_type m2_element_list;
  b_pl_type m2_list_list;
  b_pl_type m2_str_pool;
  
  uint8_t *u8_list;
  int u8_cnt;
  
};
typedef struct _mr_struct *mr_struct;
typedef struct _mr_struct *mr_type;


#define mr_WhileM2Element(mr, pos) b_pl_While((mr)->m2_element_list, pos)
#define mr_WhileM2List(mr, pos) b_pl_While((mr)->m2_list_list, pos)
#define mr_WhileM2Str(mr, pos) b_pl_While((mr)->m2_str_pool, pos)

#define mr_GetM2Element(mr, pos) ((void *)(b_pl_Get((mr)->m2_element_list,(pos))))
#define mr_GetM2List(mr, pos) ((void **)(b_pl_Get((mr)->m2_list_list,(pos))))
#define mr_GetM2Str(mr, pos) ((const char *)(b_pl_Get((mr)->m2_str_pool,(pos))))

mr_type mr_Open(void);
void mr_Clear(mr_type mr);
void mr_Close(mr_type mr);

int mr_AddElement(mr_type mr, size_t size);

int mr_AddList(mr_type mr, int list_entries);
void mr_SetListElement(mr_type mr, int pos, int el_idx, void *element);

int mr_AddStr(mr_type mr, const char *s);

uint8_t *mr_AddU8(mr_type mr);

/* mr global API */

extern mr_type mr_global;

int mrg_Init(void);
void mrg_Destroy(void);
void mrg_Clear(void);

int mrg_AddElement(size_t size);
int mrg_AddList(int list_entries);
void mrg_SetListElement(int pos, int el_idx, void *element);
int mrg_AddStr(const char *s);
const char *mrg_StoreStr(const char *s);

#define mrg_GetM2Element(pos) mr_GetM2Element(mr_global, pos)
#define mrg_GetM2List(pos) mr_GetM2List(mr_global, pos)
#define mrg_GetM2Str(pos) mr_GetM2Str(mr_global, pos)

uint8_t *mrg_AddU8(void);

#endif /* _MR_H */


