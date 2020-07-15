const fs = require('fs');

const fileName = "autoload.bin";
const outName = "autoload.js";

function makeAutoLoad() {   
   let s = "// file generated automatically by 'makeutoload.js'. Do not edit\r\n\r\n";

   if(!fs.existsSync(fileName)) {
      console.log(`${fileName} not found, disabling autoload`);
      s += "const autoload = undefined;\r\n";   
      fs.writeFileSync(outName,s);
      process.exit(-1);
   }

   const bytes = fs.readFileSync(fileName);   
   
   s += "const autoload = new Uint8Array([\n   ";

   bytes.forEach((value, i)=> {   
      const comma = (i != bytes.length-1) ? ',':'';
      const cr = (i % 32 == 31) ? '\n   ' : '';
      s += `${hex(value)}${comma}${cr}`;   
   });

   s+="]);\r\n";

   fs.writeFileSync(outName,s);
   console.log(`'${outName}' generated from '${fileName}'`);
}

function hex(value) {
   return "0x" + (value<=0xF ? "0":"") + value.toString(16);
}

makeAutoLoad();

