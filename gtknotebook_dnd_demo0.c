#include <gtk/gtk.h>

static gpointer g_groupid = "GROUPID";
static guint g_width = 400;
static guint g_height = 300;

GtkNotebook *window_creation_function(GtkNotebook *source_notebook,
                                      GtkWidget *child,
                                      gint x,
                                      gint y,
                                      gpointer data
                                      )
{
  GtkWidget *window, *notebook;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  notebook = gtk_notebook_new();

  gtk_notebook_set_group(GTK_NOTEBOOK(notebook),
                         gtk_notebook_get_group(source_notebook));

  gtk_container_add(GTK_CONTAINER(window), notebook);
  gtk_window_set_default_size(GTK_WINDOW(window), g_width, g_height);
  gtk_window_move(GTK_WINDOW(window), x, y);
  gtk_widget_show_all(window);

  return GTK_NOTEBOOK(notebook);
}

const GtkTargetEntry button_targets[] =
  {
   {"GTK_NOTEBOOK_TAB", GTK_TARGET_SAME_APP, 0},
  };

void on_drag_data_received(GtkWidget *widget,
                           GdkDragContext *context,
                           gint x,
                           gint y,
                           GtkSelectionData *data,
                           guint info,
                           guint time,
                           gpointer user_data
                           )
{
  GtkWidget *source;
  GtkWidget **child;

  source = gtk_drag_get_source_widget(context);
  child = (void *)data->data;

  gtk_container_remove(GTK_CONTAINER(source), *child);
}

GtkWidget *create_trash_button()
{
  GtkWidget *button;

  button = gtk_button_new_from_stock(GTK_STOCK_DELETE);

  gtk_drag_dest_set(button,
                    GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_DROP,
                    button_targets,
                    G_N_ELEMENTS(button_targets),
                    GDK_ACTION_MOVE
                    );
  g_signal_connect_after(G_OBJECT(button),
                         "drag-data-received",
                         G_CALLBACK(on_drag_data_received),
                         NULL
                         );

  return button;
}

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkNotebook *notebook;
  GtkWidget *page1, *page2, *title1, *title2;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  // In order to allow for a tab to be dragged out and form a new window,
  // the application must call gtk_notebook_set_window_creation_hook and
  // pass a callback function. When a tab marked as detachable is dragged
  // to the root window, this function will be called. It is expected that
  // the function create a new window, position it as per the x and y
  // coordinates of the drop (if it so desires), show it, and return the
  // resulting notebook. GTK+ will then add the tab to that notebook.
  gtk_notebook_set_window_creation_hook(window_creation_function, NULL, NULL);

  notebook = gtk_notebook_new();
  page1 = gtk_entry_new();
  page2 = gtk_entry_new();
  title1 = gtk_label_new("page1");
  title2 = gtk_label_new("page2");

  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page1, title1);
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page2, title2);
  gtk_notebook_set_tab_detachable(GTK_NOTEBOOK(notebook), page1, TRUE);
  gtk_notebook_set_tab_detachable(GTK_NOTEBOOK(notebook), page2, TRUE);
  gtk_notebook_set_group(GTK_NOTEBOOK(notebook), g_groupid);
  gtk_container_add(GTK_CONTAINER(window), notebook);

  gtk_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_default_size(GTK_WINDOW(window), g_width, g_height);
  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
