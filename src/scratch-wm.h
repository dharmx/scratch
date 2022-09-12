/* scratch-wm.h
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

#ifdef GDK_WINDOWING_X11
#include <gdk/x11/gdkx.h>

#define XDIAG_ERR(METHOD, RETURN)                                                                                      \
    switch (RETURN) {                                                                                                  \
        case BadValue: g_warning("%s: returned BadValue!", METHOD); break;                                             \
        case BadAlloc: g_warning("%s: returned BadAlloc!", METHOD); break;                                             \
        case BadWindow: g_warning("%s: returned BadWindow!", METHOD); break;                                           \
        default: g_info("%s: call is successful!", METHOD); break;                                                     \
    }

typedef struct {
    uint32_t _NET_WM_STATE;
    uint32_t _NET_WM_STATE_HIDDEN;

    uint32_t _NET_WM_STATE_REMOVE;
    uint32_t _NET_WM_STATE_ADD;
    uint32_t _NET_WM_STATE_TOGGLE;

    uint32_t _NO_ATOM;
} AtomMap;

Atom get_xatom(Display*, const char* name);
void init_ewmh_atoms(Display*, AtomMap*);
bool set_xwindow_visiblity(Display*, Window, bool);
#endif

bool hide_window(GtkWindow*);

// vim:filetype=c
