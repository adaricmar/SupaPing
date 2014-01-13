SupaPing
========

Ping-spamming application for Dota 2 on Win32/Linux. Spam the hell out of your team when they are being idiots, and get communications-banned!


Usage
--------

Get the binaries from the bin folder.

On Windows, open the .exe and leave it open in the background. Switch to Dota 2, and hold ALT to ping.

On Linux, cd to the directory where the executable is, do a sudo chmod 777 to give yourself permissions to run it, and do ./SupaPing. Switch to Dota 2, and hold ALT to ping. Running SupaPing with the -r argument will give you a randomly-determined delay between pings, as an alternative to as-fast-as-possible.


Compiling
--------

Compiling on Linux should consist of:

      gcc/clang SupaPingLinux.c -o SupaPing -lpthread -lx11 -lxi
      
      
Compiling on Windows should consist of:

      mingw32-gcc SupaPingWin.c -o SupaPing.exe -lpthread
      
      
Troubleshooting
--------

On Linux, you must choose the proper device you want to listen for the ALT key on. By default it listens to device 12; if this does not work, run xinput, find your keyboard device id, and use the id number as an argument. For example, if my keyboard device is 11, usage would be ./SupaPing 11.

On Windows, SmartScreen or other anti-virus software may flag SupaPing. This may occur due to the low-level hooks it uses to grab keyboard input, much like a keylogger. Rest assured, this software is safe, and the source code is available.

(Note: Whilst this program does hook into Dota 2 to grab keyboard input(on Windows), using it will not get you VAC banned.)

