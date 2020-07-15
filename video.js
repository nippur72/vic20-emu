
let aspect = 1.2;

const TEXT_W = 232;
const TEXT_H = 272;

let BORDER_V_TOP;
let BORDER_V_BOTTOM;
let BORDER_H_LEFT;
let BORDER_H_RIGHT;

let SCREEN_W;
let SCREEN_H;

let saturation = 1.0;

function calculateGeometry() {
   BORDER_V_TOP    = 0;
   BORDER_V_BOTTOM = 0;
   BORDER_H_LEFT   = 0;
   BORDER_H_RIGHT  = 0;
   SCREEN_W = BORDER_H_LEFT + TEXT_W + BORDER_H_RIGHT;
   SCREEN_H = BORDER_V_TOP  + TEXT_H + BORDER_V_BOTTOM;

   // canvas is the outer canvas where the aspect ratio is corrected
   let canvas = document.getElementById("canvas");
   canvas.width  = SCREEN_W * 2;
   canvas.height = SCREEN_H * 2;

   // screen is the inner canvas that contains the emulated PAL screen
   let screenCanvas = document.createElement("canvas");
   screenCanvas.width  = SCREEN_W * 2;
   screenCanvas.height = SCREEN_H * 2;

   //console.log(`${screenCanvas.width} ${screenCanvas.height}`);
}

calculateGeometry();

/**************************************************/

let WW = 232;
let HH = 272;

let tms9928a_canvas = document.getElementById("canvas");
let tms9928a_context = tms9928a_canvas.getContext('2d');
let tms9928a_imagedata = tms9928a_context.getImageData(0, 0, WW*2, HH*2);
let imagedata_buffer = new ArrayBuffer(tms9928a_imagedata.data.length);
let imagedata_buf8 = new Uint8ClampedArray(imagedata_buffer);
let imagedata_data = new Uint32Array(imagedata_buffer);


function vdp_screen_update(ptr) {
   //console.log(ptr);
   let start = ptr / wasm_instance.HEAPU32.BYTES_PER_ELEMENT;
   let size = WW*HH;
   let buffer = wasm_instance.HEAPU32.subarray(start,start+size);

   let ptr0 = 0;
   let ptr1 = 0;
   let ptr2 = WW*2;

   for(let y=0;y<HH;y++) {
      for(let x=0;x<WW;x++) {
         let pixel = buffer[ptr0];
         imagedata_data[ptr1++] = pixel;
         imagedata_data[ptr1++] = pixel;
         imagedata_data[ptr2++] = pixel;
         imagedata_data[ptr2++] = pixel;
         ptr0++;
      }
      ptr1 += WW*2;
      ptr2 += WW*2;
   }

   tms9928a_imagedata.data.set(imagedata_buf8);
   tms9928a_context.putImageData(tms9928a_imagedata, 0, 0);

   // update LED
   document.getElementById("LED").style.visibility = LED>0 ? "visible" : "hidden";
}

