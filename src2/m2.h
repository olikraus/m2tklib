

#include <stdint.h>
#include <stddef.h>

#ifndef M2_PROGMEM
#define M2_PROGMEM
#endif


typedef struct m2_struct m2_t;
typedef struct m2el_struct m2el_t;
typedef uint8_t (*m2_dfs_fn)(m2_t *m2, uint8_t dfs_msg, uint16_t el_seq_nr, const m2el_t *el_ptr);

struct m2_struct
{
  uint16_t dfs_el_nr;
  const  m2el_t *dfs_el_ptr;
  uint16_t dfs_depth;
  m2_dfs_fn dfs_fn;
  
  m2el_t *root_el_ptr;
  
};
typedef struct m2_struct m2_t;

typedef uint16_t (*m2el_fn_t)(m2_t *m2, const m2el_t *el, uint8_t el_msg, void *arg);
struct m2el_struct
{
  m2el_fn_t fn;
};
typedef struct m2el_struct m2el_t;

struct m2el_opt_struct
{
  m2el_t el_base;
  char *opt;
};
typedef struct m2el_opt_struct m2el_opt_t;

struct m2el_list_struct
{
  m2el_opt_t el_opt;
  uint16_t cnt;
  const m2el_t **list;
};
typedef struct m2el_list_struct m2el_list_t;

/* m2el_base.c */
/* base class function, does not call anything else */
uint16_t  m2el_base(m2_t *m2, const m2el_t *el, uint8_t el_msg, void *arg);

/* m2el_opt.c */
/* option processing function, calls base class */
uint16_t  m2el_opt(m2_t *m2, const m2el_t *el, uint8_t el_msg, void *arg);

/* m2el_list.c */
/* base container class function, calls m2el_opt() */
uint16_t m2el_list(m2_t *m2, const m2el_t *el, uint8_t el_msg, void *arg);

/* m2_elapi.c */
#define M2EL_MSG_GET_CHILD_CNT 0

#define M2EL_MSG_GET_CHILD_PTR 1
struct m2el_child_ptr_struct
{
  uint16_t pos;
  const m2el_t *el_ptr;
};
typedef struct m2el_child_ptr_struct m2el_child_ptr_t;

#define M2EL_MSG_GET_OPT 2

uint16_t m2el_GetChildCnt(m2_t *m2, const m2el_t *el);
const m2el_t *m2el_GetChildPtr(m2_t *m2, const m2el_t *el, uint16_t pos);
uint8_t m2el_GetOpt(m2_t *m2, const m2el_t *el, char opt);


/* m2_dfs.c */
#define M2_DFS_MSG_PRE 0
#define M2_DFS_MSG_POST 1
uint8_t m2_DFS(m2_t *m2, m2_dfs_fn fn);

/* m2_opt.c */
/* 
  The following functions are calledby m2el_opt(). 
  There should be no need to call these functions from other parts of the software.
  Use 
      m2el_GetOpt(m2_t *m2, const m2el_t *el, char opt) 
  instead.
*/
uint8_t m2_opt_get_val_any_default(const char *str, char cmd, uint8_t default_value);
uint8_t m2_opt_get_val_zero_default(const char *str, char cmd);


