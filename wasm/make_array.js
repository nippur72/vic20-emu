const fs = require('fs');

function hex(value) {
   return "0x" + (value<=0xF ? "0":"") + value.toString(16);
}

function makeWasm() {

   const eprom = fs.readFileSync("emscripten_module.wasm");

   let s = "// wasm\r\n\r\n";
   
   s += "const emscripten_wasm_binary = new Uint8Array([\n   ";

   for(let i=0; i<eprom.length;i++)
   {
      let value = eprom[i];
      const comma = (i != eprom.length-1) ? ',':'';
      const cr = (i % 16 == 15) ? '\n   ' : '';
      s += `${hex(value)}${comma}${cr}`;   
   }

   s+="]);";

   console.log(s);
}

makeWasm();


