#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#include "gtkcellrenderermultiple.h"

enum
{
   TEXT_COLUMN,
   COMBO_COLUMN,
   COMBO_TEXT_COLUMN,
   SPIN_COLUMN,
   TOGGLE_COLUMN,
   MULTIPLE_COLUMN,
   N_COLUMNS
};

typedef struct {
    int type;
    union {
        char * string;
        int integer;
    } value;
} MultipleModel;

MultipleModel * multiple_model_new(int type, ...)
{
    va_list ap;
    MultipleModel * model = malloc(sizeof(MultipleModel));  
    if(model == NULL)
        return NULL;
    
    va_start(ap, type);
    
    model->type = type;
    
    switch(model->type)
    {
        case RENDER_SPIN:
        {
            char buffer[20];
            snprintf(buffer, 20, "%d", va_arg(ap, int));
            model->value.string = strdup(buffer);
            if(model->value.string == NULL)
            {
                free(model);
                return NULL;
            }
        }
        break;
        case RENDER_TEXT:
        case RENDER_COMBO:
            model->value.string = strdup(va_arg(ap, char*));
            if(model->value.string == NULL)
            {
                free(model);
                return NULL;
            }
        break;
        case RENDER_TOGGLE:
            model->value.integer = va_arg(ap, int);
        break;
    }
    
    return model;
}

void multiple_model_free(MultipleModel ** multiple_model)
{
    MultipleModel * model = *multiple_model;
    
    switch(model->type)
    {
        case RENDER_TEXT:
        case RENDER_COMBO:
        case RENDER_SPIN:
            free(model->value.string);
    }
    
    free(model);
    *multiple_model = NULL;
}

int multiple_model_set_string(MultipleModel * model, char const * string)
{
    switch(model->type)
    {
        case RENDER_TEXT:
        case RENDER_COMBO:
            free(model->value.string);
            model->value.string = strdup(string);
            if(model->value.string == NULL)
                return 0;
        break;
        case RENDER_SPIN:
        case RENDER_TOGGLE:
            return 0;
        break;
    }
    
    return 1;
}

int multiple_model_set_int(MultipleModel * model, int integer)
{
    switch(model->type)
    {
        case RENDER_TEXT:
        case RENDER_COMBO:
            return 0;
        break;
        case RENDER_SPIN:
        {
            char buffer[20];
            snprintf(buffer, 20, "%d", integer);
            
            free(model->value.string);
            model->value.string = strdup(buffer);
            if(model->value.string == NULL)
                return 0;
        }
        break;
        case RENDER_TOGGLE:
            model->value.integer = integer;
        break;
    }
    
    return 1;
}

static void text_edited(GtkCellRendererText *cell, const gchar *path, const gchar *value, gpointer data)
{
    GtkTreeIter iter;
    GtkListStore * list = GTK_LIST_STORE(data);
    
    gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(list), &iter, path);
    gtk_list_store_set(list, &iter, TEXT_COLUMN, value, -1);
}

static void combo_edited(GtkCellRendererText *cell, const gchar *path, const gchar *value, gpointer data)
{
    GtkTreeIter iter;
    GtkListStore *list = GTK_LIST_STORE(data);
    
    gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(list), &iter, path);
    gtk_list_store_set(list, &iter, COMBO_COLUMN, value, -1);
}

static void combo_editing_started(GtkCellRendererText *cell, GtkCellEditable *editable, const gchar *path, gpointer data)
{
    GtkTreeIter iter;
    GtkListStore *list = GTK_LIST_STORE(data);
    
    gtk_list_store_clear(list);
    
    /* 1er row */
    if(strcmp(path, "0") == 0)
    {        
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 0, "10 55 First Item", -1);
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 0, "Second Item", -1);
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 0, "Third Item", -1);
    }
    else
    {
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 0, "20 Apple", -1);
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 0, "Orange", -1);
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 0, "Banana", -1);
    }
}

static void spin_edited(GtkCellRendererText *cell, const gchar *path, const gchar *value, gpointer data)
{
    GtkTreeIter iter;
    GtkListStore * list = GTK_LIST_STORE(data);
    
    gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(list), &iter, path);
    gtk_list_store_set(list, &iter, SPIN_COLUMN, atoi(value), -1);
}


static void toggle_toggled(GtkCellRendererToggle *cell, gchar *path, gpointer data)
{
    GtkTreeIter iter;
    GtkListStore * list = GTK_LIST_STORE(data);
    gboolean value;
    
    gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(list), &iter, path);
    gtk_tree_model_get(GTK_TREE_MODEL(list), &iter, TOGGLE_COLUMN, &value, -1);
    gtk_list_store_set(list, &iter, TOGGLE_COLUMN, !value, -1);
}

static void multiple_text_edited(GtkCellRendererText *cell, const gchar *path, const gchar *value, gpointer data)
{
    GtkTreeIter iter;
    GtkListStore *list = GTK_LIST_STORE(data);
    MultipleModel * model = NULL;
    
    gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(list), &iter, path);
    gtk_tree_model_get(GTK_TREE_MODEL(list), &iter, MULTIPLE_COLUMN, &model, -1);
    multiple_model_set_string(model, value);
}

static void multiple_combo_edited(GtkCellRendererText *cell, const gchar *path, const gchar *value, gpointer data)
{
    GtkTreeIter iter;
    GtkListStore *list = GTK_LIST_STORE(data);
    MultipleModel * model = NULL;
    
    gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(list), &iter, path);
    gtk_tree_model_get(GTK_TREE_MODEL(list), &iter, MULTIPLE_COLUMN, &model, -1);
    multiple_model_set_string(model, value);
}

static void multiple_combo_editing_started(GtkCellRendererText *cell, GtkCellEditable *editable, const gchar *path, gpointer data)
{
    GtkTreeIter iter;
    GtkListStore *list = GTK_LIST_STORE(data);
    
    gtk_list_store_clear(list);
    
    /* 1er row */
    if(strcmp(path, "0") == 0)
    {        
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 0, "10 55 First Item", -1);
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 0, "Second Item", -1);
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 0, "Third Item", -1);
    }
    else
    {
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 0, "20 Apple", -1);
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 0, "Orange", -1);
        gtk_list_store_append(list, &iter);
        gtk_list_store_set(list, &iter, 0, "Banana", -1);
    }
}

static void multiple_spin_edited(GtkCellRendererText *cell, const gchar *path, const gchar *value, gpointer data)
{
    GtkTreeIter iter;
    GtkListStore *list = GTK_LIST_STORE(data);
    MultipleModel * model = NULL;
    
    gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(list), &iter, path);
    gtk_tree_model_get(GTK_TREE_MODEL(list), &iter, MULTIPLE_COLUMN, &model, -1);
    multiple_model_set_int(model, atoi(value));
}

static void multiple_toggle_toggled(GtkCellRendererToggle *cell, gchar *path, gpointer data)
{
    GtkTreeIter iter;
    GtkListStore *list = GTK_LIST_STORE(data);
    MultipleModel * model = NULL;
    
    gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(list), &iter, path);
    gtk_tree_model_get(GTK_TREE_MODEL(list), &iter, MULTIPLE_COLUMN, &model, -1);
    multiple_model_set_int(model, !model->value.integer);
}

void mutliple_cell_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
    GtkCellRendererMultiple * renderer_multiple = GTK_CELL_RENDERER_MULTIPLE(renderer);
    MultipleModel * multipleModel = NULL;
    GtkCellRenderer *internalRenderer = NULL;
    
    gtk_tree_model_get(model, iter, MULTIPLE_COLUMN, &multipleModel, -1);
    
    switch(multipleModel->type)
    {
        case RENDER_TEXT:
            switch_internal_renderer(renderer_multiple, RENDER_TEXT);
            internalRenderer = gtk_cell_renderer_multiple_get_gtk_cell_renderer_internal(renderer, RENDER_TEXT);
            g_object_set(internalRenderer, "text", multipleModel->value.string, NULL);
        break;
        case RENDER_COMBO:
            switch_internal_renderer(renderer_multiple, RENDER_COMBO);
            internalRenderer = gtk_cell_renderer_multiple_get_gtk_cell_renderer_internal(renderer, RENDER_COMBO);
            g_object_set(internalRenderer, "text", multipleModel->value.string, NULL);
            g_object_set(internalRenderer, "text-column", 0, NULL); /* 0 == first column of listStoreCombo */
        break;
        case RENDER_SPIN:
            switch_internal_renderer(renderer_multiple, RENDER_SPIN);
            internalRenderer = gtk_cell_renderer_multiple_get_gtk_cell_renderer_internal(renderer, RENDER_SPIN);
            g_object_set(internalRenderer, "text", multipleModel->value.string, NULL);
        break;
        case RENDER_TOGGLE:
            switch_internal_renderer(renderer_multiple, RENDER_TOGGLE);
            internalRenderer = gtk_cell_renderer_multiple_get_gtk_cell_renderer_internal(renderer, RENDER_TOGGLE);
            g_object_set(internalRenderer, "active", multipleModel->value.integer, NULL);
        break;
    }
}

GtkWidget * setup_list(MultipleModel ** multiple_models, unsigned int length)
{
    GtkListStore *listStore = NULL;
    GtkListStore *listStoreCombo = NULL;
    GtkWidget *list = NULL;
    GtkTreeViewColumn *column = NULL;
    GtkCellRenderer *renderer = NULL;
    GtkCellRenderer *internalRenderer = NULL;
    GtkAdjustment * adjustment = NULL;
    GtkTreeIter iter;

    listStore = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_INT, G_TYPE_BOOLEAN, G_TYPE_POINTER);
    
    multiple_models[--length] = multiple_model_new(RENDER_TEXT, "Text");
    gtk_list_store_append(listStore, &iter);
    gtk_list_store_set(listStore, &iter, TEXT_COLUMN, "10", COMBO_COLUMN, "20", COMBO_TEXT_COLUMN, 0, SPIN_COLUMN, 30, TOGGLE_COLUMN, FALSE, MULTIPLE_COLUMN, multiple_models[length], -1);
    
    multiple_models[--length] = multiple_model_new(RENDER_COMBO, "Combo");
    gtk_list_store_append(listStore, &iter);
    gtk_list_store_set(listStore, &iter, TEXT_COLUMN, "50", COMBO_COLUMN, "60", COMBO_TEXT_COLUMN, 0, SPIN_COLUMN, 70, TOGGLE_COLUMN, FALSE, MULTIPLE_COLUMN, multiple_models[length], -1);
    
    multiple_models[--length] = multiple_model_new(RENDER_SPIN, 10);
    gtk_list_store_append(listStore, &iter);
    gtk_list_store_set(listStore, &iter, TEXT_COLUMN, "90", COMBO_COLUMN, "100", COMBO_TEXT_COLUMN, 0, SPIN_COLUMN, 110, TOGGLE_COLUMN, FALSE, MULTIPLE_COLUMN, multiple_models[length], -1);
    
    multiple_models[--length] = multiple_model_new(RENDER_TOGGLE, TRUE);
    gtk_list_store_append(listStore, &iter);
    gtk_list_store_set(listStore, &iter, TEXT_COLUMN, "90", COMBO_COLUMN, "100", COMBO_TEXT_COLUMN, 0, SPIN_COLUMN, 110, TOGGLE_COLUMN, TRUE, MULTIPLE_COLUMN, multiple_models[length], -1);
    
    list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(listStore));

    g_object_unref(G_OBJECT(listStore));
    
    /**
     * === Column text ===
     */
    renderer = gtk_cell_renderer_text_new();
    g_signal_connect(G_OBJECT(renderer), "edited", G_CALLBACK(text_edited), listStore);
    g_object_set(G_OBJECT(renderer), "editable", TRUE, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(list), -1, "Text", renderer, "text", TEXT_COLUMN, NULL);
    
    /**
     * === Column combo ===
     */
    renderer = gtk_cell_renderer_combo_new();
	listStoreCombo = gtk_list_store_new(1, G_TYPE_STRING);
	g_object_set(G_OBJECT(renderer), "model", listStoreCombo, "editable", TRUE, NULL);
    g_signal_connect(G_OBJECT(renderer), "edited", G_CALLBACK(combo_edited), listStore);
    g_signal_connect(G_OBJECT(renderer), "editing-started", G_CALLBACK(combo_editing_started), listStoreCombo);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(list), -1, "Combo", renderer, "text", COMBO_COLUMN, "text-column", COMBO_TEXT_COLUMN, NULL);
    
    /**
     * === Column spin ===
     */
    renderer = gtk_cell_renderer_spin_new();
    g_object_set(G_OBJECT(renderer), "editable", TRUE, NULL);
    adjustment = gtk_adjustment_new(0, 0, 10, 1, 2, 0);
    g_object_set(G_OBJECT(renderer), "adjustment", adjustment, NULL);
    g_signal_connect(G_OBJECT(renderer), "edited", G_CALLBACK(spin_edited), listStore);
    column = gtk_tree_view_column_new_with_attributes("      Spin      ", renderer, "text", SPIN_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
    
    /**
     * === Column toggle ===
     */
    renderer = gtk_cell_renderer_toggle_new();
    g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(toggle_toggled), listStore);
    column = gtk_tree_view_column_new_with_attributes("      Toggle      ", renderer, "active", TOGGLE_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
    
    /**
     * === Column multiple ===
     */
    renderer = gtk_cell_renderer_multiple_new();
    
    /* Text */
    internalRenderer = gtk_cell_renderer_multiple_get_gtk_cell_renderer_internal(renderer, RENDER_TEXT);
    g_object_set(G_OBJECT(internalRenderer), "editable", TRUE, NULL);
    g_signal_connect(G_OBJECT(internalRenderer), "edited", G_CALLBACK(multiple_text_edited), listStore);
    
    /* Combo */
    internalRenderer = gtk_cell_renderer_multiple_get_gtk_cell_renderer_internal(renderer, RENDER_COMBO);
    listStoreCombo = gtk_list_store_new(1, G_TYPE_STRING);
    g_object_set(G_OBJECT(internalRenderer), "model", listStoreCombo, "editable", TRUE, NULL);
    g_signal_connect(G_OBJECT(internalRenderer), "edited", G_CALLBACK(multiple_combo_edited), listStore);
    g_signal_connect(G_OBJECT(internalRenderer), "editing-started", G_CALLBACK(multiple_combo_editing_started), listStoreCombo);
    
    /* Spin */
    internalRenderer = gtk_cell_renderer_multiple_get_gtk_cell_renderer_internal(renderer, RENDER_SPIN);
    g_object_set(G_OBJECT(internalRenderer), "editable", TRUE, NULL);
    adjustment = gtk_adjustment_new(0, 0, 10, 1, 2, 0);
    g_object_set(G_OBJECT(internalRenderer), "adjustment", adjustment, NULL);
    g_signal_connect(G_OBJECT(internalRenderer), "edited", G_CALLBACK(multiple_spin_edited), listStore);
    
    /* Toggle */
    internalRenderer = gtk_cell_renderer_multiple_get_gtk_cell_renderer_internal(renderer, RENDER_TOGGLE);
    g_object_set(G_OBJECT(internalRenderer), "activatable", TRUE, NULL);
    /*gtk_cell_renderer_toggle_set_activatable(internalRenderer, TRUE);*/
    g_signal_connect(G_OBJECT(internalRenderer), "toggled", G_CALLBACK(multiple_toggle_toggled), listStore);
    
    column = gtk_tree_view_column_new_with_attributes("Multiple", renderer, NULL);
    gtk_tree_view_column_set_cell_data_func(column, renderer, mutliple_cell_data_func, NULL, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW(list), column);
    
    return list;
}

int main(int argc, char *argv[])
{
    GtkWidget *window = NULL;
    GdkScreen * screen = NULL;
    GtkWidget * list = NULL;
    unsigned int length = 4;
    MultipleModel ** multiple_models = malloc(sizeof(MultipleModel*) * length);
    
    if(multiple_models == NULL)
        return EXIT_FAILURE;
    
    gtk_init(&argc, &argv);
    
    /* List */
    list = setup_list(multiple_models, length);
    gtk_widget_show(list);
    
    /* Window */
    screen =  gdk_screen_get_default();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), gdk_screen_get_width(screen)/2, gdk_screen_get_height(screen)/2);
    
    gtk_container_add(GTK_CONTAINER(window), list);
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show(window);

    gtk_main();
    
    while(length != 0)
        multiple_model_free(&multiple_models[--length]);
    free(multiple_models);
    
    return EXIT_SUCCESS;
}
