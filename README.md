SupaPing
========

Ping-spamming application for Dota 2 on Win32/Linux

On Linux, SupaPing depends on X11 (no Wayland/Mir support) and xdotool, because it uses ugly insecure system() calls to it.

On Windows, the libraries are statically-linked and the .exe should work out-of-the-box.
