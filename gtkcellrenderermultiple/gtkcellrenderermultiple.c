#include "gtkcellrenderermultiple.h"

struct _GtkCellRendererMultiplePrivate
{
    GtkCellRenderer * cell_renderer_toggle;
    GtkCellRenderer * cell_renderer_text;
    GtkCellRenderer * cell_renderer_spin;
    GtkCellRenderer * cell_renderer_combo;
    int renderer_type;
};

static GtkCellRendererClass * gtk_cell_renderer_multiple_to_gtk_cell_renderer_internal(GtkCellRenderer * cell_renderer, GtkCellRenderer ** cell_dest)
{
    GtkCellRendererMultiple * cell_renderer_multiple = GTK_CELL_RENDERER_MULTIPLE(cell_renderer);
    *cell_dest = gtk_cell_renderer_multiple_get_gtk_cell_renderer_internal(cell_renderer, cell_renderer_multiple->priv->renderer_type);
    return GTK_CELL_RENDERER_GET_CLASS(*cell_dest);
}

static GObjectClass* gtk_cell_renderer_multiple_object_to_gtk_cell_renderer_internal_object(GObject * object, GObject ** dest)
{
    GtkCellRenderer * cell_dest = GTK_CELL_RENDERER(*dest);
    GtkCellRendererClass * render_class = gtk_cell_renderer_multiple_to_gtk_cell_renderer_internal(GTK_CELL_RENDERER(object), &cell_dest);
    *dest = G_OBJECT(cell_dest);
    return G_OBJECT_CLASS(render_class);
}

static void gtk_cell_renderer_multiple_get_property(GObject*object, guint param_id, GValue *value, GParamSpec *pspec)
{
    GObject *objectDest = NULL;
    gtk_cell_renderer_multiple_object_to_gtk_cell_renderer_internal_object(object, &objectDest);
    g_object_get_property(objectDest, pspec->name, value);
}

static void gtk_cell_renderer_multiple_set_property(GObject * object, guint param_id, const GValue *value, GParamSpec *pspec)
{
    GObject *objectDest = NULL;
    gtk_cell_renderer_multiple_object_to_gtk_cell_renderer_internal_object(object, &objectDest);
    g_object_set_property(objectDest, pspec->name, value);
}

static void gtk_cell_renderer_multiple_render(GtkCellRenderer *cell, cairo_t *cr, GtkWidget *widget, const GdkRectangle *background_area, const GdkRectangle *cell_area, GtkCellRendererState flags)
{    
    GtkCellRenderer *cell_renderer = NULL;
    gtk_cell_renderer_multiple_to_gtk_cell_renderer_internal(cell, &cell_renderer)->render(cell_renderer, cr, widget, background_area, cell_area, flags);
}

static GtkCellEditable * gtk_cell_renderer_multiple_start_editing(GtkCellRenderer *cell, GdkEvent *event, GtkWidget *widget, const gchar *path, const GdkRectangle *background_area, const GdkRectangle *cell_area, GtkCellRendererState flags)
{
    GtkCellRenderer *cell_renderer = NULL;
    return gtk_cell_renderer_multiple_to_gtk_cell_renderer_internal(cell, &cell_renderer)->start_editing(cell_renderer, event, widget, path, background_area, cell_area, flags);
}

static void gtk_cell_renderer_multiple_get_preferred_width(GtkCellRenderer *cell, GtkWidget *widget, gint *minimum_size, gint *natural_size)
{   
    GtkCellRenderer *cell_renderer = NULL;
    gtk_cell_renderer_multiple_to_gtk_cell_renderer_internal(cell, &cell_renderer)->get_preferred_width(cell_renderer, widget, minimum_size, natural_size);
}

static void gtk_cell_renderer_multiple_get_preferred_height(GtkCellRenderer *cell, GtkWidget *widget, gint *minimum_size, gint *natural_size)
{
    GtkCellRenderer *cell_renderer = NULL;
    gtk_cell_renderer_multiple_to_gtk_cell_renderer_internal(cell, &cell_renderer)->get_preferred_height(cell_renderer, widget, minimum_size, natural_size);
}

static void gtk_cell_renderer_multiple_get_preferred_height_for_width(GtkCellRenderer *cell, GtkWidget *widget, gint width, gint *minimum_height, gint *natural_height)
{   
    GtkCellRenderer *cell_renderer = NULL;
    gtk_cell_renderer_multiple_to_gtk_cell_renderer_internal(cell, &cell_renderer)->get_preferred_height_for_width(cell_renderer, widget, width, minimum_height, natural_height);
}

static void gtk_cell_renderer_multiple_get_aligned_area(GtkCellRenderer *cell, GtkWidget *widget, GtkCellRendererState flags, const GdkRectangle *cell_area, GdkRectangle *aligned_area)
{   
    GtkCellRenderer *cell_renderer = NULL;
    gtk_cell_renderer_multiple_to_gtk_cell_renderer_internal(cell, &cell_renderer)->get_aligned_area(cell_renderer, widget, flags, cell_area, aligned_area);
}

static gint gtk_cell_renderer_multiple_activate(GtkCellRenderer *cell, GdkEvent *event, GtkWidget *widget, const gchar *path, const GdkRectangle *background_area, const GdkRectangle *cell_area, GtkCellRendererState flags)
{
    GtkCellRenderer *cell_renderer = NULL;
    return gtk_cell_renderer_multiple_to_gtk_cell_renderer_internal(cell, &cell_renderer)->activate(cell_renderer, event, widget, path, background_area, cell_area, flags);
}

static void gtk_cell_renderer_multiple_finalize(GObject *object)
{
    
}

static void gtk_cell_editing_started(GtkCellRendererText *cell, GtkCellEditable *editable, const gchar *path, gpointer data)
{
    guint signal_id_p;
    GQuark detail_p;
    GtkCellRendererMultiple * cell_renderer_multiple = data;
    
    if(g_signal_parse_name("editing-started", GTK_TYPE_CELL_RENDERER, &signal_id_p, &detail_p, TRUE) == TRUE)
        g_signal_emit(cell_renderer_multiple->priv->cell_renderer_combo, signal_id_p, 0, editable, path);
    else
        g_printerr("g_signal_parse_name editing-started\n");
}

static void gtk_cell_renderer_multiple_class_init(GtkCellRendererMultipleClass * klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    GtkCellRendererClass *cell_class = GTK_CELL_RENDERER_CLASS(klass);
    
    object_class->finalize = &gtk_cell_renderer_multiple_finalize;
    object_class->get_property = &gtk_cell_renderer_multiple_get_property;
    object_class->set_property = &gtk_cell_renderer_multiple_set_property;
    
    /*
     * Renderer text
     */
    cell_class->render = &gtk_cell_renderer_multiple_render;
    cell_class->start_editing = &gtk_cell_renderer_multiple_start_editing;
    cell_class->get_preferred_width = &gtk_cell_renderer_multiple_get_preferred_width;
    cell_class->get_preferred_height = &gtk_cell_renderer_multiple_get_preferred_height;
    cell_class->get_preferred_height_for_width = &gtk_cell_renderer_multiple_get_preferred_height_for_width;
    cell_class->get_aligned_area = &gtk_cell_renderer_multiple_get_aligned_area;

    /*
     * Renderer Toggle
     */
     cell_class->activate = &gtk_cell_renderer_multiple_activate;
     
     g_type_class_add_private(object_class, sizeof(GtkCellRendererMultiplePrivate));     
}

static void gtk_cell_renderer_multiple_init(GtkCellRendererMultiple * cell_renderer_multiple)
{
    cell_renderer_multiple->priv = G_TYPE_INSTANCE_GET_PRIVATE(cell_renderer_multiple, GTK_TYPE_CELL_RENDERER_MULTIPLE, GtkCellRendererMultiplePrivate);
    
    cell_renderer_multiple->priv->cell_renderer_toggle = gtk_cell_renderer_toggle_new();
    cell_renderer_multiple->priv->cell_renderer_text = gtk_cell_renderer_text_new();
    cell_renderer_multiple->priv->cell_renderer_spin = gtk_cell_renderer_spin_new();
    cell_renderer_multiple->priv->cell_renderer_combo = gtk_cell_renderer_combo_new();
    
    switch_internal_renderer(cell_renderer_multiple, RENDER_TEXT);
    
    g_signal_connect(G_OBJECT(cell_renderer_multiple), "editing-started", G_CALLBACK(gtk_cell_editing_started), cell_renderer_multiple);
}

GType gtk_cell_renderer_multiple_get_type(void)
{
    static GType renderer_multiple_get_type = 0;

    if(renderer_multiple_get_type == 0)
    {
        static const GTypeInfo multiple_info =
        {
            sizeof(GtkCellRendererMultipleClass),                       /* class_size */
            NULL,                                                       /* base_init */
            NULL,                                                       /* base_finalize */
            (GClassInitFunc) gtk_cell_renderer_multiple_class_init,     /* class_init */
            NULL,                                                       /* class_finalize */
            NULL,                                                       /* class_data */
            sizeof (GtkCellRendererMultiple),                           /* instance_size */
            0,                                                          /* n_preallocs */
            (GInstanceInitFunc) gtk_cell_renderer_multiple_init,        /* instance_init */
            NULL                                                        /* value_table */
        };

        /* Derive from GtkCellRenderer */
        renderer_multiple_get_type = g_type_register_static(GTK_TYPE_CELL_RENDERER, "GtkCellRendererMultiple", &multiple_info, 0);
    }
    
    return renderer_multiple_get_type;
}


GtkCellRenderer * gtk_cell_renderer_multiple_new(void)
{
    return g_object_new(GTK_TYPE_CELL_RENDERER_MULTIPLE, NULL);
}

GtkCellRenderer * gtk_cell_renderer_multiple_get_gtk_cell_renderer_internal(GtkCellRenderer * renderer, int renderer_type)
{
    GtkCellRendererMultiple * cell_renderer_multiple = NULL;
    
    g_return_val_if_fail(GTK_IS_CELL_RENDERER_MULTIPLE(renderer), NULL);
    
    cell_renderer_multiple = GTK_CELL_RENDERER_MULTIPLE(renderer);
    
    switch(renderer_type)
    {
        case RENDER_TOGGLE:
            return cell_renderer_multiple->priv->cell_renderer_toggle;
        case RENDER_TEXT:
            return cell_renderer_multiple->priv->cell_renderer_text;
        case RENDER_SPIN:
            return cell_renderer_multiple->priv->cell_renderer_spin;
        case RENDER_COMBO:
            return cell_renderer_multiple->priv->cell_renderer_combo;
    }
    return NULL;
}

void switch_internal_renderer(GtkCellRendererMultiple * cell_renderer_multiple, int renderer_type)
{
    cell_renderer_multiple->priv->renderer_type = renderer_type;
    switch(renderer_type)
    {
        case RENDER_TOGGLE:
            GTK_CELL_RENDERER(cell_renderer_multiple)->priv = cell_renderer_multiple->priv->cell_renderer_toggle->priv;
        break;
        case RENDER_TEXT:
            GTK_CELL_RENDERER(cell_renderer_multiple)->priv = cell_renderer_multiple->priv->cell_renderer_text->priv;
        break;
        case RENDER_SPIN:
            GTK_CELL_RENDERER(cell_renderer_multiple)->priv = cell_renderer_multiple->priv->cell_renderer_spin->priv;
        break;
        case RENDER_COMBO:
            GTK_CELL_RENDERER(cell_renderer_multiple)->priv = cell_renderer_multiple->priv->cell_renderer_combo->priv;
        break;
    }
}
