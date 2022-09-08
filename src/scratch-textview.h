#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SCRATCH_TYPE_TEXT_VIEW scratch_text_view_get_type()

G_DECLARE_FINAL_TYPE(ScratchTextView, scratch_text_view, SCRATCH, TEXT_VIEW, GtkTextView)

ScratchTextView* scratch_text_view_new(void);

G_END_DECLS

// vim:filetype=c
