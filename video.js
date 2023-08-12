
let aspect = 1.2;


// good values for CRT PAL
let SCREEN_W = 208;
let SCREEN_H = 264;
let POS_X = 0;
let POS_Y = 0;
const PIXEL_WIDTH = 2;

let saturation = 1.0;

function calculateGeometry() {
   // canvas is the outer canvas where the aspect ratio is corrected
   let canvas = document.getElementById("canvas");
   canvas.width  = SCREEN_W * PIXEL_WIDTH;
   canvas.height = SCREEN_H * PIXEL_WIDTH;

   // screen is the inner canvas that contains the emulated PAL screen
   let screenCanvas = document.createElement("canvas");
   screenCanvas.width  = SCREEN_W * PIXEL_WIDTH;
   screenCanvas.height = SCREEN_H * PIXEL_WIDTH;   
}

calculateGeometry();

/**************************************************/

let WW = SCREEN_W;
let HH = SCREEN_H;

let m6561_canvas = document.getElementById("canvas");
let m6561_context = m6561_canvas.getContext('2d');
let m6561_imagedata = m6561_context.createImageData(WW*PIXEL_WIDTH, HH*PIXEL_WIDTH);
let imagedata_data = new Uint32Array(m6561_imagedata.data.buffer);

function get_wasm_instance() {
   return wasm_instance
}

let cnt = 0;

// called back by WASM at the end of each video frame
function m6561_screen_update(ptr) {
   
   if(PIXEL_WIDTH === 1) {   
      // 1 pixel wide version
      let size1 = WW*HH*4*PIXEL_WIDTH*PIXEL_WIDTH;
      const heap = get_wasm_instance().HEAPU8.subarray(ptr,ptr+size1);
      const data = new Uint8ClampedArray(heap);       
      const img = new ImageData(data, WW, HH);
      m6561_context.putImageData(img, 0, 0);   
      return;
   } else {
      // 2 pixel wide version
      let size1 = WW*HH*4*PIXEL_WIDTH*PIXEL_WIDTH;
      const heap = get_wasm_instance().HEAPU8.subarray(ptr,ptr+size1);
      const data = new Uint8ClampedArray(heap);       
      const img = new ImageData(data, WW*PIXEL_WIDTH, HH*PIXEL_WIDTH);
      m6561_context.putImageData(img, -POS_X, -POS_Y);   
   }

   /*
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
   */
}

