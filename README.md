# LM80C Javascript emulator

A JavaScript emulator for the [LM80C](https://github.com/leomil72/LM80C) computer by Leonardo Miliani.

Open the emulator directly in your browser: [lm80c-emu](https://nippur72.github.io/lm80c-emu/)

LOADING AND SAVING FILES
========================

- prg files (`.prg`) are plain files that are loaded in memory as-is

Dragging & dropping a file on the emulator's window causes the file to be loaded.

Once a file is loaded, it's also stored on the browser cache so that you don't have
to drag&drop it again; you can use the `load()` function from the JavaScript console.

These are the commands you can type from the JavaScript console (F12 key):

- `load("file.prg" [,start])` loads a file at the specified address
- `save("file.prg" [,start, end])` saves a file 
- `download("file.prg")` gets the file as a browser download
- `remove("file.prg")` remove file from browser's cache
- `dir()` lists files on browser's cache
- `paste(text)` paste a string of text (e.g. containing a BASIC program) via the LM80C serial line

DEBUGGER
========
You can plug your own Javascript debug functions by defining 
`debugBefore()` and `debugAfter(elapsed)` in the JavaScript console.

`debugBefore` is executed before any Z80 instruction; `debugAfter` is executed
after.

To activate the debug mode use `lm80c_set_debug(true)` and `lm80c_set_debug(false)`  
to deactivate it. Within the debug functions you can access all the emulator variables,
most likely you'll want to read the Z80 state with `cpu.getState()` or the memory content
with `mem_read()` and `mem_write()`.


AUTOLOADING
=================
The emulator can be used in cross-development allowing to automate the process of 
loading and executing the program being developed. This will save lot of annoying drag&drops. 

To enable "autoload":
- clone the emulator on your local machine (it won't work in the online-version because of browser restrictions)
- in your compile chain (`make` etc..), copy the binary you want to execute in the emulator directory naming it `autoload.bin`
- execute `node makeautoload`, this will turn `autoload.bin` into JavaScript code (`autoload.js`).
- refresh the browser, the program will be loaded in memory and make it RUN

When you no longer want the file to be autoloaded, delete `autoload.bin` and run again `node makeautoload`.



