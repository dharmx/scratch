#include "scratch-textview.h"

static void open_button_clicked(GtkButton* self, gpointer user_data) {}

static void save_button_clicked(GtkButton* self, gpointer user_data) {}

static void close_button_clicked(GtkButton* self, gpointer user_data) {}

static void hide_button_clicked(GtkButton* self, gpointer user_data) {}

static void stick_button_clicked(GtkButton* self, gpointer user_data) {}

static void startup_callback(GApplication* self, gpointer user_data) {
    GtkBuilder* builder;
    GtkApplicationWindow* window;
    GdkDisplay* display;
    GtkCssProvider* provider;

    GtkScrolledWindow* scrolled_window;
    GtkButton* open_button;
    GtkButton* save_button;
    GtkButton* stick_button;
    GtkButton* hide_button;
    GtkButton* close_button;

    builder = gtk_builder_new_from_resource("/org/gtk/scratch/src/scratch-window.ui");
    window = GTK_APPLICATION_WINDOW(gtk_builder_get_object(builder, "win"));
    display = gtk_widget_get_display(GTK_WIDGET(window));
    provider = gtk_css_provider_new();

    scrolled_window = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scro-win"));
    open_button = GTK_BUTTON(gtk_builder_get_object(builder, "btn-open"));
    save_button = GTK_BUTTON(gtk_builder_get_object(builder, "btn-save"));
    stick_button = GTK_BUTTON(gtk_builder_get_object(builder, "btn-stick"));
    hide_button = GTK_BUTTON(gtk_builder_get_object(builder, "btn-hide"));
    close_button = GTK_BUTTON(gtk_builder_get_object(builder, "btn-close"));

    g_signal_connect(open_button, "clicked", G_CALLBACK(open_button_clicked), scrolled_window);
    g_signal_connect(save_button, "clicked", G_CALLBACK(save_button_clicked), scrolled_window);
    g_signal_connect(close_button, "clicked", G_CALLBACK(close_button_clicked), scrolled_window);
    g_signal_connect(stick_button, "clicked", G_CALLBACK(stick_button_clicked), NULL);
    g_signal_connect(hide_button, "clicked", G_CALLBACK(hide_button_clicked), NULL);

    gtk_css_provider_load_from_resource(provider, "/org/gtk/scratch/src/scratch-style.css");
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void open_callback(GApplication* self, gpointer user_data) {}

static void activate_callback(GApplication* self, gpointer files, gint total_files, gchar* hint, gpointer user_data) {
    g_print("This is working!\n");
}

#define APPLICATION_ID "org.gtk.scratch"

int main(int argc, char** argv) {
    int status;
    GtkApplication* application;

    application = gtk_application_new(APPLICATION_ID, G_APPLICATION_HANDLES_OPEN);
    g_signal_connect(application, "startup", G_CALLBACK(startup_callback), NULL);
    g_signal_connect(application, "activate", G_CALLBACK(activate_callback), NULL);
    g_signal_connect(application, "open", G_CALLBACK(open_callback), NULL);
    status = g_application_run(G_APPLICATION(application), argc, argv);

    return status;
}

// vim:filetype=c
