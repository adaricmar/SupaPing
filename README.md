SupaPing
========

Ping-spamming application for Dota 2 on Win32/Linux. Spam the hell out of your teammates when they are being idiots, and get communications-banned!


Features:

 * Multithreaded Pinging! Utilize all of your cores for maximum pinging potential!
 * FPS-friendly! Uses 0% of system resources when not pinging.
 * VAC-safe!
 * Linux support!

Usage
--------

Get the binaries from the bin folder. Please right click -> save as. 

On Windows, open the .exe and leave it open in the background. Switch to Dota 2, and hold ALT to ping.

On Linux, cd to the directory where the executable is. 

Then,

      sudo chmod 777 SupaPing 

to give yourself permissions to execute it. Then,

      ./SupaPing [OPTIONS] [DEVICE ID]

Options: 

-r to set a random delay between pings, as an alternative to as-fast-as-possible.

Please remember to give SupaPing your keyboard's device id from xinput, else it may listen for input on a wrong device. See Troubleshooting for more information.




Compiling
--------

Compiling on Linux should consist of:

      gcc SupaPingLinux.c -o SupaPing -lpthread -lx11 -lxi

or

      clang SupaPingLinux.c -o SupaPing -lpthread -lx11 -lxi
      
      
Compiling on Windows should consist of:

      mingw32-gcc SupaPingWin.c -o SupaPing.exe -lpthread
      
with the win32-pthread headers installed.
      
      
Troubleshooting
--------

On Linux, you must choose the proper device you want to listen for the ALT key on. By default it listens to device 12; if this does not work, run xinput, find your keyboard device id, and use the id number as an argument. For example, if my keyboard device is 11, usage would be ./SupaPing 11.

On Windows, SmartScreen or other anti-virus software may flag SupaPing. This may occur due to the low-level hooks it uses to grab keyboard input, much like a keylogger. Rest assured, this software is safe, and the source code is available.

(Note: Whilst this program does hook into Dota 2 to grab keyboard input(on Windows), using it will not get you VAC banned.)

