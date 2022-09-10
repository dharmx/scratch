/* scratch-textview.h
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

#pragma once

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SCRATCH_TYPE_TEXT_VIEW scratch_text_view_get_type()

G_DECLARE_FINAL_TYPE(ScratchTextView, scratch_text_view, SCRATCH, TEXT_VIEW, GtkTextView)

enum ScratchTextViewOpenResponseType {
    SCRATCH_OPEN_RESPONSE_SUCESS,
    SCRATCH_OPEN_RESPONSE_CANCEL,
    SCRATCH_OPEN_RESPONSE_ERROR,
};

ScratchTextView* scratch_text_view_new(void);

ScratchTextView* scratch_text_view_new_with_file(GFile*);

GFile* scratch_text_view_get_file(ScratchTextView*);

void scratch_text_view_set_file(ScratchTextView*, GFile*);

void scratch_text_view_save(ScratchTextView*);

void scratch_text_view_save_as(ScratchTextView*);

void scratch_text_view_open(ScratchTextView*, GtkWindow*);

G_END_DECLS

// vim:filetype=c
