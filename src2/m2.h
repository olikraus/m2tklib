

#include <stdint.h>
#include <stddef.h>


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


/* m2_elapi.c */
#define M2EL_MSG_GET_CHILD_CNT 0

#define M2EL_MSG_GET_CHILD_PTR 1
struct m2el_child_ptr_struct
{
  uint16_t pos;
  m2el_t *el_ptr;
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


