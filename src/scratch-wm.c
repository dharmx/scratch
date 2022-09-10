/* scratch-wm.c
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

#ifdef GDK_WINDOWING_X11

#define WM_STATE_VAL(S) ((S) == 1 ? "normal" : (S) == 3 ? "iconic" : "withdrawn")

void xset_state(Display* display, Window window, int state) {
    Atom wm_state = XInternAtom(display, "WM_STATE", FALSE);
    long data[2] = {state, None};

    switch (XChangeProperty(display, window, wm_state, wm_state, 32, PropModeReplace, (u_char*) &data, 2)) {
        case BadAlloc: g_warning("BadAlloc ERROR on %lx!\n", window); break;
        case BadAtom: g_warning("BadAtom ERROR on %lx!\n", window); break;
        case BadMatch: g_warning("BadMatch ERROR on %lx!\n", window); break;
        case BadValue: g_warning("BadValue ERROR on %lx!\n", window); break;
        case BadWindow: g_warning("BadWindow ERROR on %lx!\n", window); break;
        default: g_info("Succesfully set 0x%lx to %s state.\n", window, WM_STATE_VAL(state)); break;
    }
}

void xset_hide(Display* display, Window window, bool hidden) {
    if (hidden) {
        XUnmapWindow(display, window); // Actually sets it to WithdrawnState
        xset_state(display, window, IconicState); // redundant - help
        // This works (if NormalState) but not if the window is in WithdrawnState
        // The above statement is redundant - I'll take a look at this later.
    } else {
        xset_state(display, window, NormalState);
        XMapWindow(display, window);
    }
    XFlush(display);
}

void xhide_window(GtkWidget* window) {
    GdkDisplay* display;
    GdkSurface* surface;

    display = gtk_widget_get_display(window);
    surface = gtk_native_get_surface(gtk_widget_get_native(window));

    Display* xdisplay = gdk_x11_display_get_xdisplay(display);
    Window xwindow = gdk_x11_surface_get_xid(surface);
    xset_hide(xdisplay, xwindow, true);
}

#endif

// vim:filetype=c
