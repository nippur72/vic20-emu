
let aspect = 1.2;


// good values for CRT PAL
let SCREEN_W = 208;
let SCREEN_H = 264;
let POS_X = 0;
let POS_Y = 0;

/*
// values for debug
let SCREEN_W = 284;
let SCREEN_H = 312;
let POS_X = 0;
let POS_Y = 0;
*/

let saturation = 1.0;

function calculateGeometry() {
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

/*
let WW = 232;
let HH = 272;
*/

let WW = SCREEN_W;
let HH = SCREEN_H;

let tms9928a_canvas = document.getElementById("canvas");
let tms9928a_context = tms9928a_canvas.getContext('2d');
let tms9928a_imagedata = tms9928a_context.getImageData(0, 0, WW*2, HH*2);
let imagedata_buffer = new ArrayBuffer(tms9928a_imagedata.data.length);
let imagedata_buf8 = new Uint8ClampedArray(imagedata_buffer);
let imagedata_data = new Uint32Array(imagedata_buffer);

// called back by WASM at the end of each video frame
function vdp_screen_update(ptr) {
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
   tms9928a_context.putImageData(tms9928a_imagedata, POS_X, POS_Y);

   frames++;
   if(end_of_frame_hook !== undefined) end_of_frame_hook();
}
