#!/bin/sh

# Use settings from KDE for Qt apps (alternatively, setup qt5ct)
export XDG_CURRENT_DESKTOP=GNOME

dwmblocks &
xrdb ~/.config/x11/Xresources
nitrogen &
dunst &
/usr/lib/polkit-kde-authentication-agent-1 &
exec dwm
