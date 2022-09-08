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

#include "scratch-textview.h"

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

    scratch_text_view_signals[CHANGE_FILE] = g_signal_newv("change-file", G_TYPE_FROM_CLASS(self_class),
                                                           G_SIGNAL_RUN_LAST | G_SIGNAL_NO_HOOKS | G_SIGNAL_NO_RECURSE,
                                                           0, NULL, NULL, NULL, G_TYPE_NONE, 0, NULL);
    scratch_text_view_signals[OPEN_RESPONSE] = g_signal_new("open-response", G_TYPE_FROM_CLASS(self_class),
                                                            G_SIGNAL_RUN_LAST | G_SIGNAL_NO_HOOKS | G_SIGNAL_NO_RECURSE,
                                                            0, NULL, NULL, NULL, G_TYPE_NONE, 1, G_TYPE_INT, NULL);
}

static void scratch_text_view_init(ScratchTextView* self) {}

ScratchTextView* scratch_text_view_new(void) { return SCRATCH_TEXT_VIEW(g_object_new(SCRATCH_TYPE_TEXT_VIEW, NULL)); }

// vim:filetype=c
