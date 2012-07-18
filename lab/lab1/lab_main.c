

/*
http://developer.gnome.org/gtk3/stable/
http://scentric.net/tutorial/		tree view tutorial
http://www.proggen.org/doku.php?id=gui:gtk:start   german tutorial
http://www.gtk.org/api/2.6/gtk/index.html
http://developer.gnome.org/gtk/2.24/

static link
http://wiki.wxwidgets.org/Linking_With_Gtk_And_Running_On_Any_Linux


currently at max 3 additional arguments (however, fns and ptrs might count twice)

arg_label <-- appears in the edit window
arg_type        NUM, STR, PSTR, NUMPTR, STRPTR, (PSTRPTR)

for NUMPTR and STRPTR, a default value can be stored in an additional arg element


file syntax

[
element "abcd";
name "abcd";
format "w1h1";
string "abcd"            BUTTON, INFO, INFOP, LABEL, ROOT
str_var "ptrname"       LABELPTR, TEXT
cb_name "abcd"          BUTTON, COMBO, INFO, INFOP, STRLIST
cb_code "abcd"          BUTTON, COMBO, INFO, INFOP, STRLIST
u8_var1 "xyz"            COMBO, INFO, INFOP, RADIO, STRLIST, TOGGLE, U8NUM, 
u8_var2 "xyz"           INFO, INFOP, STRLIST, U8NUM
u32_var                 U32NUM
len "111"               COMBO
reference "element"     ROOT

]
{

}



*/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "lab.h"


GtkWidget *main_window;	
GtkTreeStore *m2_tree_store_model;  /* the model */
GtkWidget *m2_tree_view;
GtkTreeSelection *selection;



int el_def_find_by_iter(GtkTreeIter *iter)
{
        int idx;
        char *type;
        gtk_tree_model_get (GTK_TREE_MODEL(m2_tree_store_model), iter, TYPE_COLUMN, &type, -1);
        idx = el_def_find_by_type(type);
        g_free (type);
        return idx;
}

int el_def_find_by_child_iter(GtkTreeIter *iter)
{
        GtkTreeIter parent;
        if ( gtk_tree_model_iter_parent(GTK_TREE_MODEL(m2_tree_store_model), &parent, iter) == FALSE )
                return -1;
        return el_def_find_by_iter(&parent);
}

/*===================================================================*/

FILE *p_fp;
int p_current_char;
#define P_BUF_SIZE (1024*4)
char p_buf[P_BUF_SIZE];

char p_element[P_BUF_SIZE];
char p_name[P_BUF_SIZE];
char p_format[P_BUF_SIZE];


/* put next char into p_current_char */
int p_next_char(void)
{
  p_current_char = fgetc(p_fp);
  if ( p_current_char == EOF )
    return 0;
  return 1;
}

void p_err(const char *msg)
{
}

int p_skip_space(void)
{
  for(;;)
  {
    if ( p_current_char == 0 || p_current_char > 32 )
      break;
    if ( p_next_char() == 0 )
      return 0;
  }
  return 1;
}

/* put identifier into p_buf */
int p_get_identifier(void)
{
  int i = 0;
  if ( p_current_char == EOF )
    return p_err("unexpected EOF (expected identifier)"), 0;          
  for(;;)
  {
    if (  p_current_char <= 32 )
      break;
    p_buf[i++] =  p_current_char;
    if ( p_next_char() == 0 )
      break;
  }
  p_buf[i++] =  '\0';
  p_skip_space();
  return 1;    
}

int p_get_string(void)
{
  int i = 0;
  if ( p_current_char == EOF )
    return 0;
  if ( p_current_char != '\"' )
    return p_err("\" expected"), 0;
  if ( p_next_char() == 0 )
    return p_err("unexpected EOF (\")"), 0;    
  for(;;)
  {
    if ( p_current_char == '\\' )
    {
      if ( p_next_char() == 0 )
        return p_err("unexpected EOF (\\)"), 0;    
    }
    else if ( p_current_char == '\"' )
    {
      p_next_char();
      break;
    }
    p_buf[i++] =  p_current_char;
    if ( p_next_char() == 0 )
      return p_err("unexpected EOF (\")"), 0;          
  }
  p_buf[i] =  '\0';
  return 1;
}

int p_read_element(void)
{
  if ( p_current_char != '[' )
    return p_err("[ expected"), 0;
  if ( p_next_char() == 0 )
    return p_err("unexpected EOF (after [)"), 0;          
  if ( p_skip_space() == 0 )
    return p_err("unexpected EOF (after [)"), 0;
    
  for(;;)
  {
    if ( p_current_char != ']' )
      break;
    if ( p_get_identifier() == 0 )
      return 0;
    if ( strcmp(p_buf, "element") == 0 )
    {
      if ( p_get_string() == 0 )
        return 0;
      strcpy(p_element, p_buf);
    }
    else if ( strcmp(p_buf, "name") == 0 )
    {
      if ( p_get_string() == 0 )
        return 0;
      strcpy(p_name, p_buf);
    }
    else if ( strcmp(p_buf, "format") == 0 )
    {
      if ( p_get_string() == 0 )
        return 0;
      strcpy(p_format, p_buf);
    }
  }
  return 1;

}


/*===================================================================*/

void dfs_loop(GtkTreeIter *iter, int depth)
{
        gchar *type;
        gchar *name;
        GtkTreeIter first_child;
        do
        {
                gtk_tree_model_get(GTK_TREE_MODEL(m2_tree_store_model), iter, TYPE_COLUMN, &type, NAME_COLUMN, &name, -1);
                g_print ("%d: %s %s\n", depth, type, name);
                g_free (type);
                g_free (name);
                
                if ( gtk_tree_model_iter_children(GTK_TREE_MODEL(m2_tree_store_model), &first_child, iter) != FALSE )
                        dfs_loop(&first_child, depth+1);
        } while ( gtk_tree_model_iter_next(GTK_TREE_MODEL(m2_tree_store_model), iter) != FALSE );
}

void dfs_model(void)
{
        GtkTreeIter iter;
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL(m2_tree_store_model), &iter);
        dfs_loop(&iter, 0);
}

/*===================================================================*/


void popup_add_below(GtkWidget *menuitem, gpointer userdata)
{
        /* user data contains pointer to el_def */
        GtkTreeIter iter;
        GtkTreeIter iter2;
        GtkTreeModel *model;
        GtkTreeSelection *selection;
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (m2_tree_view));
        if ( gtk_tree_selection_get_selected(selection, &model, &iter) )
        {
                el_def_struct *def = (el_def_struct *)userdata;
                
                gtk_tree_store_append(m2_tree_store_model, &iter2, &iter); 
                gtk_tree_store_set (m2_tree_store_model, &iter2,
                    TYPE_COLUMN, def->type_name,
                    NAME_COLUMN, "",
                    -1);
                // gtk_tree_model_iter_parent  --> move to parent		
        }
}

void popup_add_next(GtkWidget *menuitem, gpointer userdata)
{
        /* user data contains pointer to el_def */
        GtkTreeIter iter;
        GtkTreeIter iter2;
        GtkTreeModel *model;
        GtkTreeSelection *selection;
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (m2_tree_view));
        if ( gtk_tree_selection_get_selected(selection, &model, &iter) )
        {
                el_def_struct *def = (el_def_struct *)userdata;
                
                gtk_tree_store_insert_after(m2_tree_store_model, &iter2, NULL, &iter);		
                gtk_tree_store_set (m2_tree_store_model, &iter2,
                    TYPE_COLUMN, def->type_name,
                    NAME_COLUMN, "",
                    -1);
                // gtk_tree_model_iter_parent  --> move to parent		
        }
}

  void
  view_popup_menu_onDoSomething (GtkWidget *menuitem, gpointer userdata)
  {
        
        gchar *type;
        gchar *name;
        GtkTreeIter iter;
        GtkTreeModel *model;
          
        /* we passed the view as userdata when we connected the signal */
        GtkTreeView *treeview = GTK_TREE_VIEW(userdata);
        GtkTreeSelection *selection;
          
        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
          
        if ( gtk_tree_selection_get_selected(selection, &model, &iter) )
        {
                gtk_tree_model_get (model, &iter, TYPE_COLUMN, &type, NAME_COLUMN, &name, -1);

                g_print ("You selected: %s %s\n", type, name);

                build_edit_window(iter);
                g_free (type);
                g_free (name);
        }

        g_print ("Do something!\n");
  }


void build_sub_menu(GtkWidget *menu, int is_below)
{
        GtkWidget *menuitem;
        int i, cnt;
        cnt = sizeof(el_def_list)/sizeof(*el_def_list);
        for( i = 0; i < cnt; i++ )
        {
                menuitem = gtk_menu_item_new_with_label(el_def_list[i].type_name);
                if ( is_below )
                        g_signal_connect(menuitem, "activate", (GCallback) popup_add_below, el_def_list+i);
                else
                        g_signal_connect(menuitem, "activate", (GCallback) popup_add_next, el_def_list+i);
                        
                gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
        }
}
  
void view_popup_menu (GtkWidget *treeview, GdkEventButton *event, gpointer userdata)
{
    GtkWidget *menu, *menuitem;
    GtkWidget *submenu, *submenuitem;
    int button, event_time;

    menu = gtk_menu_new();
    //g_signal_connect (menu, "deactivate", G_CALLBACK (gtk_widget_destroy), NULL);
          
    menuitem = gtk_menu_item_new_with_label("Edit");
    g_signal_connect(menuitem, "activate", (GCallback) view_popup_menu_onDoSomething, treeview);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

    menuitem = gtk_menu_item_new_with_label("Add below");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

    submenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);
    build_sub_menu(submenu, 1);
                
          
    menuitem = gtk_menu_item_new_with_label("Add next");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

    submenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);
    build_sub_menu(submenu, 0);

    gtk_widget_show_all(menu);

  if (event)
    {
      button = event->button;
      event_time = event->time;
    }
  else
    {
      button = 0;
      event_time = gtk_get_current_event_time ();
    }	  
          
    /* Note: event can be NULL here when called from view_onPopupMenu;
     *  gdk_event_get_time() accepts a NULL argument */
  gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, NULL,
                  button, event_time);
}

gboolean tree_button_press (GtkWidget *treeview, GdkEventButton *event, gpointer userdata)
{
    /* single click with the right mouse button? */
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3)
    {
      g_print ("Single right click on the tree view.\n");

      /* optional: select row if no row is selected or only
       *  one other row is selected (will only do something
       *  if you set a tree selection mode as described later
       *  in the tutorial) */
      if (1)
      {
        GtkTreeSelection *selection;
        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

        /* Note: gtk_tree_selection_count_selected_rows() does not
         *   exist in gtk+-2.0, only in gtk+ >= v2.2 ! */
        if (gtk_tree_selection_count_selected_rows(selection)  <= 1)
        {
           GtkTreePath *path;

           /* Get tree path for row that was clicked */
           if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
                                             (gint) event->x, 
                                             (gint) event->y,
                                             &path, NULL, NULL, NULL))
           {
             gtk_tree_selection_unselect_all(selection);
             gtk_tree_selection_select_path(selection, path);
             gtk_tree_path_free(path);
           }
        }
      } /* end of optional bit */
      view_popup_menu(treeview, event, userdata);
      return TRUE; /* we handled this */
    }
    return FALSE; /* we did not handle this */
}

gboolean tree_popup_menu (GtkWidget *treeview, gpointer userdata)
{
        view_popup_menu(treeview, NULL, userdata);
        return TRUE; /* we handled this */
}

void name_edited_callback(GtkCellRendererText *cell,
                                  gchar               *path_string,
                                  gchar               *new_text,
                                  gpointer             user_data)
{
        GtkTreeIter iter;
        if ( gtk_tree_model_get_iter_from_string( GTK_TREE_MODEL(m2_tree_store_model), &iter, path_string) )
        {		
                gtk_tree_store_set ( m2_tree_store_model, &iter,
                    NAME_COLUMN, new_text,
                    -1);
                g_print ("NAME = %s\n", new_text);
        }
}

void build_tree(void)
{
        GtkTreeIter iter;
        GtkTreeIter iter2;

        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        
        m2_tree_store_model = gtk_tree_store_new (N_COLUMNS,       /* Total number of columns */
                                          G_TYPE_STRING,   /* Type */
                                          G_TYPE_STRING,   /* Name */
                                          G_TYPE_STRING,  /* Format */
                                          G_TYPE_STRING,  /* arg0 */
                                          G_TYPE_STRING,  /* arg1 */
                                          G_TYPE_STRING,  /* arg2 */
                                          G_TYPE_STRING,  /* arg3 */
                                          G_TYPE_STRING,  /* arg4 */
                                          G_TYPE_STRING,  /* arg5 */
                                          G_TYPE_STRING,  /* arg6 */
                                          G_TYPE_STRING  /* arg7 */
        );


        gtk_tree_store_append (m2_tree_store_model, &iter, NULL); 

        gtk_tree_store_set (m2_tree_store_model, &iter,
                    TYPE_COLUMN, "M2_VLIST",
                    NAME_COLUMN, "vlist",
                    -1);
        
        gtk_tree_store_append (m2_tree_store_model, &iter2, &iter); 
        gtk_tree_store_set (m2_tree_store_model, &iter2,
                    TYPE_COLUMN, "M2_BUTTON",
                    NAME_COLUMN, "b1",
                    -1);
        gtk_tree_store_append (m2_tree_store_model, &iter2, &iter); 
        gtk_tree_store_set (m2_tree_store_model, &iter2,
                    TYPE_COLUMN, "M2_BUTTON",
                    NAME_COLUMN, "b2",
                    -1);
        
        m2_tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (m2_tree_store_model));
        
        
        renderer = gtk_cell_renderer_text_new ();
        column = gtk_tree_view_column_new_with_attributes ("Type",
                                                           renderer,
                                                           "text", TYPE_COLUMN,
                                                           NULL);
        gtk_tree_view_column_set_resizable(column, TRUE);
        gtk_tree_view_append_column (GTK_TREE_VIEW (m2_tree_view), column);	

        renderer = gtk_cell_renderer_text_new ();
        g_object_set(renderer, "editable", TRUE, NULL);
        g_signal_connect(renderer, "edited", (GCallback) name_edited_callback, NULL);
        column = gtk_tree_view_column_new_with_attributes ("Name",
                                                           renderer,
                                                           "text", NAME_COLUMN,
                                                           NULL);
        gtk_tree_view_column_set_resizable(column, TRUE);
        gtk_tree_view_append_column (GTK_TREE_VIEW (m2_tree_view), column);	

        renderer = gtk_cell_renderer_text_new ();
        column = gtk_tree_view_column_new_with_attributes ("Format",
                                                           renderer,
                                                           "text", FMT_COLUMN,
                                                           NULL);
        gtk_tree_view_column_set_resizable(column, TRUE);
        gtk_tree_view_append_column (GTK_TREE_VIEW (m2_tree_view), column);	


        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (m2_tree_view));
        gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);


        g_signal_connect(m2_tree_view, "button-press-event", (GCallback) tree_button_press, NULL);
        g_signal_connect(m2_tree_view, "popup-menu", (GCallback) tree_popup_menu, NULL); 	/* Shift-F10 */
}


int main(int argc, char *argv[])
{
 
    gtk_init(&argc, &argv);
        
    build_tree();
        
 
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        
    gtk_container_add (GTK_CONTAINER (main_window), m2_tree_view);
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
 
    gtk_widget_show_all(main_window);
        dfs_model();
    gtk_main();
    return 0;
}

