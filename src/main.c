/* main.c
 *
 * Copyright 2022 dharmx
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "scratch.h"

static void open_button_clicked(GtkButton* self, gpointer user_data) {
    GtkScrolledWindow* scrolled_window;
    ScratchTextView* text_view;

    GtkTextBuffer* text_buffer;

    scrolled_window = GTK_SCROLLED_WINDOW(user_data);
    text_view = SCRATCH_TEXT_VIEW(gtk_scrolled_window_get_child(scrolled_window));
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
}

static void save_button_clicked(GtkButton* self, gpointer user_data) {}

static void close_button_clicked(GtkButton* self, gpointer user_data) {
    gtk_window_destroy(GTK_WINDOW(gtk_widget_get_ancestor(GTK_WIDGET(self), GTK_TYPE_WINDOW)));
}

static void hide_button_clicked(GtkButton* self, GtkWindow* window) { hide_window(window); }

static void startup_callback(GApplication* application, gpointer user_data) {
    GtkBuilder* builder;
    GtkApplicationWindow* window;
    GdkDisplay* display;
    GtkCssProvider* provider;

    GtkScrolledWindow* scrolled_window;
    GtkButton* open_button;
    GtkButton* save_button;
    GtkButton* hide_button;
    GtkButton* close_button;

    builder = gtk_builder_new_from_resource("/org/gtk/scratch/src/scratch-window.ui");
    window = GTK_APPLICATION_WINDOW(gtk_builder_get_object(builder, "win"));
    display = gtk_widget_get_display(GTK_WIDGET(window));
    provider = gtk_css_provider_new();

    scrolled_window = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scro-win"));
    open_button = GTK_BUTTON(gtk_builder_get_object(builder, "btn-open"));
    save_button = GTK_BUTTON(gtk_builder_get_object(builder, "btn-save"));
    close_button = GTK_BUTTON(gtk_builder_get_object(builder, "btn-close"));

    hide_button = GTK_BUTTON(gtk_builder_get_object(builder, "btn-hide"));

    g_signal_connect(open_button, "clicked", G_CALLBACK(open_button_clicked), scrolled_window);
    g_signal_connect(save_button, "clicked", G_CALLBACK(save_button_clicked), scrolled_window);
    g_signal_connect(close_button, "clicked", G_CALLBACK(close_button_clicked), scrolled_window);
    g_signal_connect(hide_button, "clicked", G_CALLBACK(hide_button_clicked), window);

    gtk_css_provider_load_from_resource(provider, "/org/gtk/scratch/src/scratch-style.css");
    gtk_style_context_add_provider_for_display(
        display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(application));
}

static void activate_callback(GApplication* self, gpointer user_data) {
    GtkWindow* window = NULL;

    window = gtk_application_get_active_window(GTK_APPLICATION(self));
    gtk_window_present(window);
}

static void open_callback(GApplication* self, gpointer files, gint total_files, gchar* hint, gpointer user_data) {}

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
