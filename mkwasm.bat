@echo off

rem abilitare il path con
rem ..\emsdk-master\emsdk_env.bat

call emcc wasm\prova.c -O3 ^
   -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" ^
   -s ENVIRONMENT=web ^
   -s MODULARIZE=1 ^
   -s EXPORT_NAME="emscripten_module" ^
   -o emscripten_module.js
    
echo emcc finished, putting wasm binary into UInt8Array

node wasm\make_array > emscripten_module.wasm.array.js

echo done


