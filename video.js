
let aspect = 1.2;


// good values for CRT PAL
let SCREEN_W = 208;
let SCREEN_H = 264;
let POS_X = 0;
let POS_Y = 0;

/*
let tms9928_SCREEN_W = 342;
let tms9928_SCREEN_H = 262;

let tms9928_WW = 342;
let tms9928_HH = 262;
*/

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

   /*
   // canvas is the outer canvas where the aspect ratio is corrected
   let tms9928_canvas = document.getElementById("canvas_tms9928");
   tms9928_canvas.width  = tms9928_SCREEN_W * 2;
   tms9928_canvas.height = tms9928_SCREEN_H * 2;

   // screen is the inner canvas that contains the emulated PAL screen
   let tms9928_screenCanvas = document.createElement("canvas_tms9928");
   tms9928_screenCanvas.width  = tms9928_SCREEN_W * 2;
   tms9928_screenCanvas.height = tms9928_SCREEN_H * 2;
   */
}

calculateGeometry();

/**************************************************/

/*
let WW = 232;
let HH = 272;
*/

let WW = SCREEN_W;
let HH = SCREEN_H;

let m6561_canvas = document.getElementById("canvas");
let m6561_context = m6561_canvas.getContext('2d');
let m6561_imagedata = m6561_context.createImageData(WW*2, HH*2);
let imagedata_data = new Uint32Array(m6561_imagedata.data.buffer);

/*
let m6561_imagedata = m6561_context.getImageData(0, 0, WW*2, HH*2);
let imagedata_buffer = new ArrayBuffer(m6561_imagedata.data.length);
let imagedata_buf8 = new Uint8ClampedArray(imagedata_buffer);
let imagedata_data = new Uint32Array(imagedata_buffer);
*/

// called back by WASM at the end of each video frame
function m6561_screen_update(ptr) {
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

   // m6561_imagedata.data.set(imagedata_buf8);
   m6561_context.putImageData(m6561_imagedata, POS_X, POS_Y);

   frames++;
   if(end_of_frame_hook !== undefined) end_of_frame_hook();
}

// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************
// **************************************************************************
/*
let tms_counter = 0;
let tms9928_canvas = document.getElementById("canvas_tms9928");
let tms9928_context = tms9928_canvas.getContext('2d');
let tms9928_imagedata = tms9928_context.getImageData(0, 0, tms9928_WW*2, tms9928_HH*2);
let tms9928_imagedata_buffer = new ArrayBuffer(tms9928_imagedata.data.length);
let tms9928_imagedata_buf8 = new Uint8ClampedArray(tms9928_imagedata_buffer);
let tms9928_imagedata_data = new Uint32Array(tms9928_imagedata_buffer);

// called back by WASM at the end of each video frame
function tms9928_screen_update(ptr) {
   let start = ptr / wasm_instance.HEAPU32.BYTES_PER_ELEMENT;
   let size = tms9928_WW*tms9928_HH;
   let buffer = wasm_instance.HEAPU32.subarray(start,start+size);

   let ptr0 = 0;
   let ptr1 = 0;
   let ptr2 = tms9928_WW*2;

   for(let y=0;y<tms9928_HH;y++) {
      for(let x=0;x<tms9928_WW;x++) {
         let pixel = buffer[ptr0];
         if(x==20) pixel = 0x88C3F2A1;
         tms9928_imagedata_data[ptr1++] = pixel;
         tms9928_imagedata_data[ptr1++] = pixel;
         tms9928_imagedata_data[ptr2++] = pixel;
         tms9928_imagedata_data[ptr2++] = pixel;
         ptr0++;
      }
      ptr1 += tms9928_WW*2;
      ptr2 += tms9928_WW*2;
   }

   tms9928_imagedata.data.set(tms9928_imagedata_buf8);
   tms9928_context.putImageData(tms9928_imagedata, 0, 0);

   frames++;
   if(end_of_frame_hook !== undefined) end_of_frame_hook();

   tms_counter++;
   if(tms_counter % 16 == 0) console.log("tms");
}
*/