#ifndef __GTK_CELL_RENDERER_MULTIPLE_H__
#define __GTK_CELL_RENDERER_MULTIPLE_H__

#include <gtk/gtk.h>

#define GTK_TYPE_CELL_RENDERER_MULTIPLE             (gtk_cell_renderer_multiple_get_type())
#define GTK_CELL_RENDERER_MULTIPLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj),  GTK_TYPE_CELL_RENDERER_MULTIPLE, GtkCellRendererMultiple))
#define GTK_CELL_RENDERER_MULTIPLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  GTK_TYPE_CELL_RENDERER_MULTIPLE, GtkCellRendererMultipleClass))
#define GTK_IS_CELL_RENDERER_MULTIPLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CELL_RENDERER_MULTIPLE))
#define GTK_IS_CELL_RENDERER_MULTIPLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  GTK_TYPE_CELL_RENDERER_MULTIPLE))
#define GTK_CELL_RENDERER_MULTIPLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),  GTK_TYPE_CELL_RENDERER_MULTIPLE, GtkCellRendererMultiple))

typedef struct _GtkCellRendererMultiple         GtkCellRendererMultiple;
typedef struct _GtkCellRendererMultiplePrivate  GtkCellRendererMultiplePrivate;
typedef struct _GtkCellRendererMultipleClass    GtkCellRendererMultipleClass;

struct _GtkCellRendererMultiple 
{
    GtkCellRenderer parent;
    GtkCellRendererMultiplePrivate * priv;
};

struct _GtkCellRendererMultipleClass
{
    GtkCellRendererClass parent_class;
    
    void (*edited)(GtkCellRendererText *cell_renderer_text, const gchar *path, const gchar *new_text, gpointer data);
    void (*editing_started)(GtkCellRendererText *cell, GtkCellEditable *editable, const gchar *path, gpointer data);
};

enum 
{
    RENDER_TOGGLE,
    RENDER_TEXT,
    RENDER_SPIN,
    RENDER_COMBO
};

GType gtk_cell_renderer_multiple_get_type(void) G_GNUC_CONST;

GtkCellRenderer * gtk_cell_renderer_multiple_new(void);

GtkCellRenderer * gtk_cell_renderer_multiple_get_gtk_cell_renderer_internal(GtkCellRenderer * renderer, int renderer_type);

void switch_internal_renderer(GtkCellRendererMultiple * multiple_cell_renderer, int renderer_type);

#endif
