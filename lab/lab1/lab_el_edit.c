/*

  lab_el_edit.c

*/

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "m2.h"
#include "lab.h"


#define EL_EDIT_FIELDS_MAX 40

/* edit element dialog window */
GtkTreeIter edit_iter;				/* iterator for the row which is edited by the user */
GtkWidget *edit_entry_name;		/* name of the element */
GtkWidget *edit_fields[EL_EDIT_FIELDS_MAX];
int edit_arg_type[EL_EDIT_FIELDS_MAX];
int el_edit_def_idx;


/* translate the fld index into column index of the gtk store model */
int el_edit_get_column_index(int el_idx, int fld_idx)
{
  int options; 
  if ( fld_idx == 0 )
    return 1; /* element name */
  fld_idx--;
  options = m2_get_cmd_cnt(el_def_list[el_idx].opt_with_default_values);
  if ( fld_idx < options )
    return 2;   /* format column */
  fld_idx--;
  return fld_idx+3; /* remaining fields map 1:1 */
}

/* result must be freed with g_free */
gchar *el_edit_get_value(GtkTreeIter iter, int el_idx, int fld_idx)
{
  int col;
  gchar *str;
  uint8_t cmd, opt;
  char buf[32];
  col = el_edit_get_column_index(el_idx, fld_idx);
  
  gtk_tree_model_get(GTK_TREE_MODEL(m2_tree_store_model), &iter, col, &str, -1);
  if ( col == 1 || col >= 3 )
  {
    if ( str == NULL )
      str = g_strdup("");
    return str;
  }

  cmd = el_def_get_nth_cmd(el_idx, fld_idx-1);
  opt = m2_opt_get_val_any_default(str, cmd, 0);
  
  sprintf(buf, "%d", opt);
  return g_strdup(buf);
}

void el_edit_store_format(GtkTreeIter iter, int el_idx)
{
  char buf[4*EL_EDIT_FIELDS_MAX];
  uint8_t options, i, c;
  const gchar *s;
  int value;
  options = m2_get_cmd_cnt(el_def_list[el_idx].opt_with_default_values);
  buf[0] = '\0';
  for( i = 0; i < options; i++ )
  {
    c = m2_get_nth_cmd(el_def_list[el_idx].opt_with_default_values, i );
    s = gtk_entry_get_text(GTK_ENTRY(edit_fields[i+1]));        /* return value must not be freed */
    value = atoi(s);
    sprintf(buf+strlen(buf), "%c%d", c, value);
  }
  gtk_tree_store_set(m2_tree_store_model, &iter, 2, buf, -1);
}

void el_edit_store_name(GtkTreeIter iter)
{
  const gchar *s;
  s = gtk_entry_get_text(GTK_ENTRY(edit_fields[0]));        /* return value must not be freed */
  gtk_tree_store_set(m2_tree_store_model, &iter, 1, s, -1);  
}

void el_edit_store_args(GtkTreeIter iter, int el_idx)
{
  int arg_cnt = el_def_get_arg_cnt(el_edit_def_idx);
  int options = m2_get_cmd_cnt(el_def_list[el_idx].opt_with_default_values);
  int i;
  for( i= 0; i < arg_cnt; i++ )
  {
    switch( edit_arg_type[i+options+1] )
    {
        case EL_ARG_FN_CODE:
          break;
        default:
        {
          const gchar *s;
          s = gtk_entry_get_text(GTK_ENTRY(edit_fields[i+options+1]));        /* return value must not be freed */
          gtk_tree_store_set(m2_tree_store_model, &iter, i+3, s, -1);  
          break;
        }
    }
  }
  
  
}


void edit_ok(GtkButton *button, gpointer data)
{
  
  el_edit_store_name(edit_iter);
  el_edit_store_format(edit_iter, el_edit_def_idx);
  /* data is a pointer to the toplevel window of the dialog */
  gtk_widget_destroy(data);
}

void edit_cancel(GtkButton *button, gpointer data)
{
        /* data is a pointer to the toplevel window of the dialog */
        gtk_widget_destroy(data);
}


void build_edit_window(GtkTreeIter iter)
{
        GtkWidget *window;	
        GtkWidget *vbox;	
        GtkWidget *hbox;	
        GtkWidget *button;
        GtkWidget *label;
        GtkWidget *table;
        GtkWidget *widget;
        gchar *type;
        gchar *name;
        gchar *value;
        int fld_idx, fld_cnt;
        
        gtk_tree_model_get (GTK_TREE_MODEL(m2_tree_store_model), &iter, TYPE_COLUMN, &type, NAME_COLUMN, &name, -1);
        el_edit_def_idx = el_def_find_by_type(type);
        assert( el_edit_def_idx >= 0 );
  
        fld_cnt = el_def_get_field_cnt(el_edit_def_idx);
  
        g_free (type);
        g_free (name);

        edit_iter = iter;
        
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Edit");
        gtk_window_set_modal(GTK_WINDOW(window), TRUE);
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));
        
                vbox = gtk_vbox_new(FALSE, 0);
                table = gtk_table_new(fld_cnt+2, 5, 0);


/*
                        hbox = gtk_hbox_new(FALSE, 0);
                        gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new("Name: "), FALSE, FALSE, 0);	
                        edit_entry_name = gtk_entry_new();
                        gtk_box_pack_start(GTK_BOX(hbox), edit_entry_name, FALSE, FALSE, 0);
        
                gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);	
*/
          for( fld_idx = 0; fld_idx < fld_cnt; fld_idx++ )
          {
            edit_arg_type[fld_idx] = el_def_get_field_arg_type_by_index(el_edit_def_idx, fld_idx);
            switch( edit_arg_type[fld_idx] )
            {
                case EL_ARG_FN_CODE:
                  {
                    GtkWidget *scroll = gtk_scrolled_window_new(NULL,NULL);
                    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),GTK_POLICY_ALWAYS,GTK_POLICY_ALWAYS);
                    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll),GTK_SHADOW_IN);
                    edit_fields[fld_idx] = gtk_text_view_new();
                    gtk_container_add(GTK_CONTAINER(scroll), edit_fields[fld_idx]); // add textview into scroll
                    widget = scroll;
                    break;
                  }
                default:
                    edit_fields[fld_idx] = gtk_entry_new();
                    widget = edit_fields[fld_idx];
                    value = el_edit_get_value(iter, el_edit_def_idx, fld_idx);
                    gtk_entry_set_text(GTK_ENTRY(widget), value);

                    break;
            }

            /*
                hbox = gtk_hbox_new(FALSE, 0);
                gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new(el_def_get_field_label_by_index(el_edit_def_idx, fld_idx)), FALSE, FALSE, 0);	
                gtk_box_pack_start(GTK_BOX(hbox), edit_fields[fld_idx], FALSE, FALSE, 0);
                gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);	
            */
            label = gtk_label_new(el_def_get_field_label_by_index(el_edit_def_idx, fld_idx));
            gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
            gtk_misc_set_padding(GTK_MISC(label), 1, 0);
            gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 2, fld_idx, fld_idx+1);
            gtk_table_attach_defaults(GTK_TABLE(table), widget, 2, 4, fld_idx, fld_idx+1);
          }


                        hbox = gtk_hbutton_box_new(); 
                        button = gtk_button_new_with_label("Ok");
                        g_signal_connect(button, "clicked", G_CALLBACK(edit_ok), window);			
                        //gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 1, fld_idx+1, fld_idx+2);
                        gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
                        button = gtk_button_new_with_label("Cancel");
                        g_signal_connect(button, "clicked", G_CALLBACK(edit_cancel), window);
                        //gtk_table_attach_defaults(GTK_TABLE(table), button, 1, 2, fld_idx+1, fld_idx+2);
                        gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0); 
          
                        gtk_table_attach_defaults(GTK_TABLE(table), hbox, 1, 3, fld_idx+1, fld_idx+2);

                /* gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0); */	

        /* Ensure that the dialog box is destroyed when the user responds */
        g_signal_connect_swapped (window, "destroy", G_CALLBACK (gtk_widget_destroy), window);
                             
    gtk_container_add(GTK_CONTAINER(window), table);
    gtk_widget_show_all(window);	
}
