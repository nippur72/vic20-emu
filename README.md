# VIC20 Javascript emulator

A JavaScript emulator for the VIC20 computer.

This emulator is based on the amazing work of Andre Weissflog

https://github.com/floooh/chips-test
https://github.com/floooh/chips-test

Copyright (c) 2017 Andre Weissflog

Open the emulator directly in your browser: [vic20-emu](https://nippur72.github.io/vic20-emu/)


QUERYSTRING OPTIONS
===================
- `config=n` sets memory config (n=0,1,...)
- `joy=1` emulates joystick
- `load=program.prg` loads and exectutes a program
- `b=<....>` loads the specified string as a base64 binary encoded file (created with the function `makeBase64())`

LOADING AND SAVING FILES
========================

- prg files (`.prg`) are common VIC-20 program files that are loaded in memory.
The first two bytes contain the loading address.

Dragging & dropping a ".prg" file on the emulator's window causes the file to be loaded.

Once a file is loaded, it's also stored on the browser cache so that you don't have
to drag&drop it again; you can use the `load()` function from the JavaScript console.

These are the commands you can type from the JavaScript console (F12 key):

- `load("file.prg" [,start])` loads a file at the specified address
- `save("file.prg" [,start, end])` saves a file 
- `download("file.prg")` gets the file as a browser download
- `remove("file.prg")` remove file from browser's cache
- `dir()` lists files on browser's cache
- `paste(text)` paste a string of text (e.g. containing a BASIC program)
- `makeBase64("file.prg")` encodes the file for use with the `b=` query string parameter (see)

MISC
========
- `vic20.config(n)` configures memory (n from 0 to 5)
- `vic20.reset()` resets the VIC20 (also via CTRL+ALT+BREAK keys)
- `vic20.peek(address)`
- `vic20.poke(address, data)`




