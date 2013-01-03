/*

  mn.h
  
  menu node 
  
*/

#ifndef _MN_H
#define _MN_H

#include <stdint.h>

typedef struct _mn_struct *mn_type;
typedef struct _mn_arg_struct mn_arg_struct;
typedef int (*mn_fn)(mn_type mn, int msg, void *arg);

#define MN_MSG_NONE 0
#define MN_MSG_OPEN 1
#define MN_MSG_CLOSE 2
#define MN_MSG_COPY 3
#define MN_MSG_GET_DISPLAY_STRING 4

/* arg: char ** */
#define MN_MSG_GET_LABEL_STRING 5

/* arg: not used */
#define MN_MSG_C_CODE 6

/* arg: not used, n->mr_element_pos must be updated correctly */
#define MN_MSG_RTE 7

/* resolve RTE references (e.g. for M2_ROOT) */
#define MN_MSG_RTE_POST 8

/* must return 2 for a value M2 element (note: 1 is default) */
#define MN_MSG_IS_M2_ELEMENT 9

#define MN_ARG_NAME_LEN 64
#define MN_ARG_T_BOOL 0
#define MN_ARG_T_U8 1
#define MN_ARG_T_STR 2
#define MN_ARG_T_MN 3
struct _mn_arg_struct
{
  char name[MN_ARG_NAME_LEN];
  int t;						/* Variable type: MN_ARG_T_BOOL, MN_ARG_T_U8, MN_ARG_T_STR */
  uint32_t user_val;
  char *user_str;				/* MN_ARG_T_STR */
  mn_type user_mn;				/* MN_ARG_T_MN */
  unsigned long default_val;		/* probably obsolete */
  unsigned int is_fmt:1;
  unsigned int is_enable:1;		/* used only if is_fmt != 0 */
};

struct _mn_struct
{
  mn_type n;
  mn_type d;
  mn_fn fn;
  unsigned seq_nr;
  //void *data;
  
  unsigned arg_cnt;
  mn_arg_struct *arg_list;
  
  int mr_list_pos;				/* if this node is a M2 container, then this member value contains the list position for the runtime environment */
  int mr_list_len;
  int mr_element_pos;			/* for an M2 element, this is the position of the element in the runtime environment */
};

mn_type mn_OpenWithFn(mn_fn fn);			/* mn_init.c */
mn_type mn_Open(void);					/* mn_init.c */

/* Close a single node, dues not close the connected subtree. */
/* Usually it is better to use mn_Del() */
void mn_Close(mn_type mn);					/* mn_init.c */
mn_type mn_OpenCopy(mn_type source);		/* mn_init.c */


mn_type mn_GetSubtree(mn_type n, int pos);		/* mn_get.c */

/* Close/Delete 'n' and the whole subtree */
void mn_Del(mn_type n);					/* mn_del.c */

/* Remove the subtree at position "pos" and return this subtree */
mn_type mn_CutSubtree(mn_type n, int pos);		/* mn_del.c */
void mn_DelSubtree(mn_type n, int pos);		/* mn_del.c */


mn_type mn_Copy(mn_type n);				/* mn_copy.c */
mn_type mn_CopySubtree(mn_type n, int pos);	/* mn_copy.c */


void mn_AppendSubtree(mn_type n, mn_type s);			/* mn_add.c */
void mn_PasteSubtree(mn_type n, int pos, mn_type s);		/* mn_add.c */

int mn_GetCnt(mn_type n);							/* mn_cnt.c */

unsigned mn_BuildSeqNr(mn_type n, unsigned start_nr);	/* mn_seqnr.c */

void mn_Show(mn_type n);							/* mn_show.c */


int mn_FindArgByName(mn_type n, const char *name);										/* mn_arg.c */

void mn_SetArg(mn_type n, int pos, int t, const char *name, unsigned long default_val, int is_fmt); /* mn_arg.c */
int mn_AddArg(mn_type n, int t, const char *name, unsigned long default_val, int is_fmt); 		/* mn_arg.c */
const char *mn_GetFmtStr(mn_type n);													/* mn_arg.c */
const char *mn_GetRTEFmtStr(mn_type n);												/* mn_arg.c */

int mn_SetArgStr(mn_type n, int pos, const char *str);										/* mn_arg.c */
int mn_SetArgStrByName(mn_type n, const char *name, const char *str);						/* mn_arg.c */
const char *mn_GetArgStrByName(mn_type n, const char *name);							/* mn_arg.c */

int mn_SetArgNode(mn_type n, int pos, mn_type ref);										/* mn_arg.c */
int mn_SetArgNodeByName(mn_type n, const char *name, mn_type ref);						/* mn_arg.c */
mn_type mn_GetArgNodeByName(mn_type n, const char *name);								/* mn_arg.c */

int mn_SetArgU8(mn_type n, int pos, uint8_t u8);											/* mn_arg.c */
int mn_SetArgU8ByName(mn_type n, const char *name, uint8_t u8);							/* mn_arg.c */
uint8_t mn_GetArgU8ByName(mn_type n, const char *name);								/* mn_arg.c */


/*================================================*/

int mn_fn_empty(mn_type mn, int msg, void *arg);		/* mn_fn.c */
const char *mn_GetLabelString(mn_type mn);			/* mn_fn.c */

void mn_BuildCodeStr(const char *s);					/* mn_m2code.c */
void mn_BuildCodeNum(uint32_t v);					/* mn_m2code.c */
void mn_BuildCodeLabel(mn_type n);					/* mn_m2code.c */
void mn_BuildCodeListLabel(mn_type n);					/* mn_m2code.c */
void mn_BuildCodeListStatement(mn_type n);				/* mn_m2code.c */
void mn_BuildCode(mn_type n);						/* mn_m2code.c */

/*================================================*/

/* build dynamic elements for the m2 runtime environment */
int mn_BuildRTE(mn_type n);

/* resolve references */
void mn_BuildRTEPost(mn_type n);

int mn_fn_m2_vlist(mn_type mn, int msg, void *arg);
int mn_fn_m2_label(mn_type n, int msg, void *arg);
int mn_fn_m2_root(mn_type n, int msg, void *arg);
int mn_fn_m2_u8num(mn_type n, int msg, void *arg);

int16_t mn_GetCodeByFn(mn_fn fn);
mn_fn mn_GetFnByCode(int16_t code);

#endif
