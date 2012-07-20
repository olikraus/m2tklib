/*

  mn.h
  
  menu node 
  
*/

typedef struct _mn_struct *mn_type;
struct _mn_struct
{
  mn_type n;
  mn_type d;
  void *data;
};

mn_type mn_Open(void);					/* mn_init.c */
void mn_Close(mn_type mn);					/* mn_init.c */
mn_type mn_OpenCopy(mn_type source);		/* mn_init.c */


mn_type mn_GetSubtree(mn_type n, int pos);		/* mn_get.c */


void mn_Del(mn_type n);					/* mn_del.c */
mn_type mn_CutSubtree(mn_type n, int pos);		/* mn_del.c */
mn_type mn_DelSubtree(mn_type n, int pos);		/* mn_del.c */

mn_type mn_Copy(mn_type n);				/* mn_copy.c */
mn_type mn_CopySubtree(mn_type n, int pos);	/* mn_copy.c */


void mn_AppendSubtree(mn_type n, mn_type s);			/* mn_add.c */
void mn_PasteSubtree(mn_type n, int pos, mn_type s);		/* mn_add.c */

