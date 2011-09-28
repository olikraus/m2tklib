


#include <gtk/gtk.h>


/*====================================================*/
/* GLOBAL */

enum
{
  TYPE_COLUMN,
  NAME_COLUMN,
  FMT_COLUMN,
  ARG0_COLUMN,
  ARG1_COLUMN,
  ARG2_COLUMN,
  ARG3_COLUMN,
  ARG4_COLUMN,
  ARG5_COLUMN,
  ARG6_COLUMN,
  ARG7_COLUMN,
  N_COLUMNS
};

extern GtkWidget *main_window;	
extern GtkTreeStore *m2_tree_store_model;  /* the model */
extern GtkWidget *m2_tree_view;



/*====================================================*/
/* lab_element.c */

#define EL_ARG_NONE 0

/* value directly provided */
#define EL_ARG_NUM 1
#define EL_ARG_STR 2

/* direct value is identifier, value in next arg */
#define EL_ARG_CSTR 100
#define EL_ARG_PSTR 101
#define EL_ARG_U8PTR 102
#define EL_ARG_U32PTR 103
#define EL_ARG_STRPTR 104
#define EL_ARG_FN 105

#define EL_ARG_CSTR_CONTENT 200
#define EL_ARG_PSTR_CONTENT  201
#define EL_ARG_U8PTR_DEFAULT 202
#define EL_ARG_U32PTR_DEFAULT 203
#define EL_ARG_STRPTR_DEFAULT 204
#define EL_ARG_FN_CODE 205

struct _el_arg_struct
{
        int arg_type;
        char *arg_label;
};
typedef struct _el_arg_struct el_arg_struct;

#define EL_DEF_PARENT_MODE_NONE 0
#define EL_DEF_PARENT_MODE_ONE_CHILD 1
#define EL_DEF_PARENT_MODE_UNLIMITED 2

#define EL_DEF_ARG_CNT 8

struct _el_def_struct
{
        int is_m2;
        char *type_name;
        char *opt_with_default_values;
        int parent_mode;		
        el_arg_struct args[EL_DEF_ARG_CNT];
};
typedef struct _el_def_struct el_def_struct;

#define EL_DEF_CNT 4
extern el_def_struct el_def_list[EL_DEF_CNT];

/* returns index into el_def_list */
int el_def_find_by_type(const char *type_name);

int el_def_get_arg_cnt(int el_idx);
/* returns number of edit fields for el_idx, el_idx is a index for el_def_list */
int el_def_get_field_cnt(int el_idx);

char el_def_get_nth_cmd(int el_idx, int opt_idx);
/* returns the label of the edit field */
const char *el_def_get_field_label_by_index(int el_idx, int fld_idx);
/* return one of EL_ARG_xxx values */
int el_def_get_field_arg_type_by_index(int el_idx, int fld_idx);

/*====================================================*/
/* lab_el_edit.c */

void build_edit_window(GtkTreeIter iter);
