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

#include "scratch-wm.h"

#ifdef GDK_WINDOWING_X11

Atom get_xatom(Display* display, const char* name) {
    Atom atom = XInternAtom(display, name, False);
    XDIAG_ERR("XInternAtom", atom)
    return atom;
}

void init_ewmh_atoms(Display* display, AtomMap* ewmh_atoms) {
    ewmh_atoms->_NET_WM_STATE = get_xatom(display, "_NET_WM_STATE");
    ewmh_atoms->_NET_WM_STATE_HIDDEN = get_xatom(display, "_NET_WM_STATE_HIDDEN");
    ewmh_atoms->_NET_WM_STATE_REMOVE = 0;
    ewmh_atoms->_NET_WM_STATE_ADD = 1;
    ewmh_atoms->_NET_WM_STATE_TOGGLE = 2;
    ewmh_atoms->_NO_ATOM = None;
}

// clang-format off
bool set_xwindow_visiblity(Display* display, Window window, bool visible) {
    AtomMap EWMH;
    init_ewmh_atoms(display, &EWMH);

    XClientMessageEvent payload = {
        .type = ClientMessage,
        .message_type = EWMH._NET_WM_STATE,
        .format = 32,
        .window = window,
        .send_event = True,
        .display = display,
        .data.l = {
            visible ? EWMH._NET_WM_STATE_REMOVE : EWMH._NET_WM_STATE_ADD,
            EWMH._NET_WM_STATE_HIDDEN,
            EWMH._NO_ATOM
        }
    };

    int status;
    status = XSendEvent(display, window, True, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent*) &payload);
    XDIAG_ERR("XSendEvent", status)
    status = XFlush(display);
    XDIAG_ERR("XFlush", status)
    return status;
}

#endif

bool hide_window(GtkWindow* window) {
    GdkDisplay* display = gtk_widget_get_display(GTK_WIDGET(window));
    GdkSurface* surface = gtk_native_get_surface(gtk_widget_get_native(GTK_WIDGET(window)));
    bool status;

#ifdef GDK_WINDOWING_X11
    Display* xdisplay = gdk_x11_display_get_xdisplay(display);
    Window xwindow = gdk_x11_surface_get_xid(surface);
    if (GDK_IS_X11_DISPLAY(display)) status = set_xwindow_visiblity(xdisplay, xwindow, false);
    else g_critical("Unsupported GDK Backend!");
#endif

    return status;
}

// vim:filetype=c
