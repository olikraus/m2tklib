/*

  b_pl.h

  basic pointer list

  
*/

#ifndef _B_PL_H
#define _B_PL_H

#include <stddef.h>
#include <stdio.h>

struct _b_pl_struct
{
  int cnt;
  int max;
  void **list;
  
  /* this allows to close b_pl structure more than once */
  /* this is used by the pg_obj modul */
  int link_cnt;  
};
typedef struct _b_pl_struct b_pl_struct;
typedef struct _b_pl_struct *b_pl_type;

int b_pl_init(b_pl_type b_pl);
int b_pl_expand(b_pl_type b_pl);
void b_pl_destroy(b_pl_type b_pl);

#define b_pl_Cnt(b_pl)         ((b_pl)->cnt)
#define b_pl_Get(b_pl,pos)     ((b_pl)->list[pos])
#define b_pl_Set(b_pl,pos,val) ((b_pl)->list[pos] = (val))
#define b_pl_Clear(b_pl)          ((b_pl)->cnt = 0)


b_pl_type b_pl_Open();
b_pl_type b_pl_Link(b_pl_type b_pl);
void b_pl_Close(b_pl_type b_pl);
/* returns position or -1 */
int b_pl_Add(b_pl_type b_pl, void *ptr);
void b_pl_DelByPos(b_pl_type b_pl, int pos);
int b_pl_InsByPos(b_pl_type b_pl, void *ptr, int pos);
int b_pl_While(b_pl_type b_pl, int *pos);

int b_pl_Write(b_pl_type b_pl, FILE *fp, int (*write_el)(FILE *fp, void *el, void *ud), void *ud);
int b_pl_Read(b_pl_type b_pl, FILE *fp, void *(*read_el)(FILE *fp, void *ud), void *ud);

void b_pl_Sort(b_pl_type b_pl, int (*compar)(const void *, const void *));

size_t b_pl_GetMemUsage(b_pl_type b_pl);

#endif

