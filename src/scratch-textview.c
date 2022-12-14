/* scratch-textview.c
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

struct _ScratchTextView {
    GtkTextView parent_instance;

    GFile* file;
};

G_DEFINE_FINAL_TYPE(ScratchTextView, scratch_text_view, GTK_TYPE_TEXT_VIEW)

enum { CHANGE_FILE, OPEN_RESPONSE, NUMBER_OF_SIGNALS };

static guint scratch_text_view_signals[NUMBER_OF_SIGNALS];

static void scratch_text_view_dispose(GObject* object) {
    ScratchTextView* text_view;

    text_view = SCRATCH_TEXT_VIEW(object);
    if (G_IS_FILE(text_view->file))
        g_clear_object(&text_view->file); // NOLINT
    G_OBJECT_CLASS(text_view)->dispose(object);
}

static void scratch_text_view_finalize(GObject* object) {
    ScratchTextView* text_view;

    text_view = SCRATCH_TEXT_VIEW(object);
    g_free(text_view->file); // @warn
    G_OBJECT_CLASS(text_view)->finalize(object);
}

static void scratch_text_view_class_init(ScratchTextViewClass* self_class) {
    GObjectClass* object_class;

    object_class = G_OBJECT_CLASS(self_class);
    object_class->dispose = scratch_text_view_dispose;
    object_class->finalize = scratch_text_view_finalize;

    scratch_text_view_signals[CHANGE_FILE] = g_signal_newv(
        "change-file", G_TYPE_FROM_CLASS(self_class), G_SIGNAL_RUN_LAST | G_SIGNAL_NO_HOOKS | G_SIGNAL_NO_RECURSE, 0,
        NULL, NULL, NULL, G_TYPE_NONE, 0, NULL);
    scratch_text_view_signals[OPEN_RESPONSE] = g_signal_new(
        "open-response", G_TYPE_FROM_CLASS(self_class), G_SIGNAL_RUN_LAST | G_SIGNAL_NO_HOOKS | G_SIGNAL_NO_RECURSE, 0,
        NULL, NULL, NULL, G_TYPE_NONE, 1, G_TYPE_INT, NULL);
}

static void scratch_text_view_init(ScratchTextView* self) {}

ScratchTextView* scratch_text_view_new(void) { return SCRATCH_TEXT_VIEW(g_object_new(SCRATCH_TYPE_TEXT_VIEW, NULL)); }

ScratchTextView* scratch_text_view_new_with_file(GFile* file) {
    g_return_val_if_fail(G_IS_FILE(file), NULL);

    ScratchTextView* text_view;
    GtkTextBuffer* text_buffer;

    char* contents;
    gsize length;

    if (!g_file_load_contents(file, NULL, &contents, &length, NULL, NULL))
        return NULL;

    if ((text_view = scratch_text_view_new()) != NULL) {
        text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        gtk_text_buffer_set_text(text_buffer, contents, length);
        text_view->file = file;
        gtk_text_buffer_set_modified(text_buffer, FALSE);
    }

    g_free(contents);
    return text_view;
}

GFile* scratch_text_view_get_file(ScratchTextView* self) { return self->file; }

void scratch_text_view_set_file(ScratchTextView* self, GFile* file) { self->file = file; }

static void show_error_dialog(GtkWindow* window, GError* error, char* message) {
    if (message != NULL)
        g_warning("%s\n", message);
    GtkWidget* message_dialog;

    message_dialog =
        gtk_message_dialog_new(window, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s.\n", error->message);
    g_signal_connect(message_dialog, "response", G_CALLBACK(gtk_window_destroy), NULL);
    gtk_widget_show(message_dialog);
}

static void open_dialog_response(GtkDialog* self, int response, ScratchTextView* text_view) {
    GtkTextBuffer* text_buffer;
    GtkWidget* message_dialog;
    GFile* file;
    GError* error;

    char* contents;
    gsize length;

    if (response != GTK_RESPONSE_ACCEPT) {
        g_signal_emit(text_view, scratch_text_view_signals[CHANGE_FILE], 0, SCRATCH_OPEN_RESPONSE_CANCEL, NULL);
    } else if (!G_IS_FILE(file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(self)))) {
        show_error_dialog(
            GTK_WINDOW(self), error, "ScratchTextView: gtk_file_chooser_get_file() does not return GFile!");
        g_error_free(error);
        g_signal_emit(text_view, scratch_text_view_signals[OPEN_RESPONSE], 0, SCRATCH_OPEN_RESPONSE_ERROR, NULL);
    } else if (!g_file_load_contents(file, NULL, &contents, &length, NULL, &error)) {
        show_error_dialog(GTK_WINDOW(self), error, NULL);
        g_error_free(error);
        g_signal_emit(text_view, scratch_text_view_signals[OPEN_RESPONSE], 0, SCRATCH_OPEN_RESPONSE_ERROR, NULL);
    } else {
        text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        gtk_text_buffer_set_text(text_buffer, contents, length);
        gtk_text_buffer_set_modified(text_buffer, FALSE);
        g_free(contents);

        if (G_IS_FILE(text_view->file))
            g_object_unref(text_view->file);
        scratch_text_view_set_file(text_view, file);

        g_signal_emit(text_view, scratch_text_view_signals[OPEN_RESPONSE], 0, SCRATCH_OPEN_RESPONSE_SUCESS, NULL);
        g_signal_emit(text_view, scratch_text_view_signals[CHANGE_FILE], 0, SCRATCH_OPEN_RESPONSE_SUCESS, NULL);
    }

    gtk_window_destroy(GTK_WINDOW(self));
}

void scratch_text_view_open(ScratchTextView* self, GtkWindow* window) {
    g_return_if_fail(SCRATCH_IS_TEXT_VIEW(self));
    g_return_if_fail(GTK_IS_WINDOW(window));

    GtkWidget* dialog;

    dialog = gtk_file_chooser_dialog_new(
        "Select File", window, GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT,
        NULL);
    g_signal_connect(dialog, "response", G_CALLBACK(open_dialog_response), self);
    gtk_widget_show(dialog);
}

void scratch_text_view_save(ScratchTextView* self) {
    g_return_if_fail(SCRATCH_IS_TEXT_VIEW(self));

    GFile* file;

    file = scratch_text_view_get_file(self);

    if (file != NULL) {
    }
}

void scratch_text_view_save_as(ScratchTextView* self) {}

// vim:filetype=c
