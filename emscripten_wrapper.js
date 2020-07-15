
let wasm_instance;

/*
let mem_read;
let mem_write;
let rom_load;

let io_read;
let io_write;

let lm80c_tick;
let lm80c_tick_line;
let lm80c_set_debug;
let lm80c_ctc_enable;
let lm80c_init;
let lm80c_reset;

let keyboard_reset;
let keyboard_press;
let keyboard_release;
let keyboard_poll;
*/

let vic20 = { };

function load_wasm(ready_cb) {

   // emscripten_module.js exports "emscripten_module" globally

   let instance = emscripten_module({ wasmBinary: emscripten_wasm_binary, onRuntimeInitialized: ()=>{
      // makes C exported functions available globally

      vic20.init = instance.cwrap("sys_init", null);
      vic20.exec = instance.cwrap("sys_exec", null);

      vic20.key_down = instance.cwrap("sys_key_down" , null, ['number'] );
      vic20.key_up   = instance.cwrap("sys_key_up"   , null, ['number'] );

      /*
      cpu_init            = instance.cwrap("cpu_init", null);
      cpu_reset           = instance.cwrap("cpu_reset", null);
      cpu_run_instruction = instance.cwrap("cpu_run_instruction", 'number');

      mem_read           = instance.cwrap("mem_read", 'number', ['number']);
      mem_write          = instance.cwrap("mem_write", null, ['number', 'number']);
      rom_load           = instance.cwrap("rom_load", null, ['number', 'number']);

      io_read            = instance.cwrap("io_read", 'number', ['number']);
      io_write           = instance.cwrap("io_write", null, ['number', 'number']);

      lm80c_tick         = instance.cwrap("lm80c_tick", 'number');
      lm80c_set_debug    = instance.cwrap("lm80c_set_debug", null, ['bool']);
      lm80c_ctc_enable   = instance.cwrap("lm80c_ctc_enable", null, ['bool']);
      lm80c_init         = instance.cwrap("lm80c_init", null);
      lm80c_reset        = instance.cwrap("lm80c_reset", null);

      lm80c_tick_line    = instance.cwrap("lm80c_tick_line", 'number', ['number']);

      keyboard_reset     = instance.cwrap("keyboard_reset"  , null );
      keyboard_press     = instance.cwrap("keyboard_press"  , null, ['number', 'number'] );
      keyboard_release   = instance.cwrap("keyboard_release", null, ['number', 'number'] );
      keyboard_poll      = instance.cwrap("keyboard_poll"   , 'number', ['number'] );
      */

      // export instance globally (not strictly required)
      wasm_instance = instance;

      // finished
      ready_cb();
   }});
}
