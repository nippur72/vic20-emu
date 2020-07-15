"use strict";

// TODO restore key
// TODO uppercase/lowercase (shift lock)
// TODO fix audio engine
// TODO submit keyboard changes
// TODO modify 6561 to trigger a callback at the end of the video
// TODO filesystem load, save, download, dir
// TODO joystick interface
// TODO fix for alt/cbm/shift what keyboard?

/******************/

const cpuSpeed = 3686400;    // 7372800/2 number given by @leomil72
const vdcSpeed = 10738635;   // number given by @leomil72
const frameRate = vdcSpeed/(342*262*2);   // ~60 Hz
const frameDuration = 1000/frameRate;     // duration of 1 frame in msec
const cyclesPerLine = cpuSpeed / vdcSpeed * 342;

let stopped = false; // allows to stop/resume the emulation

let frames = 0;
let nextFrameTime = 0;
let averageFrameTime = 0;
let minFrameTime = Number.MAX_VALUE;

let cycle = 0;
let total_cycles = 0;

let throttle = false;

let options = {
   load: undefined,
   restore: false
};

// scanline version
function renderLines(nlines) {
   vic20.exec();
}

function renderAllLines() {

   // poll keyboard
   if(keyboard_buffer.length > 0) {
      let key_event = keyboard_buffer[0];
      keyboard_buffer = keyboard_buffer.slice(1);

      if(key_event.type === "press") {
         let keys = key_event.hardware_keys;
         //keys.forEach((k) => console.log(k));
         keys.forEach((k) => vic20.key_down(k));
      }
      else if(key_event.type === "release") {
         let keys = key_event.hardware_keys;
         //keys.forEach((k) => console.log(k));
         keys.forEach((k) => vic20.key_up(k));
      }
   }

   renderLines();
}


let nextFrame;
let end_of_frame_hook = undefined;

function oneFrame() {
   const startTime = new Date().getTime();      

   if(nextFrame === undefined) nextFrame = startTime;

   nextFrame = nextFrame + (1000/frameRate); 

   renderAllLines();
   frames++;   

   if(end_of_frame_hook !== undefined) end_of_frame_hook();

   const now = new Date().getTime();
   const elapsed = now - startTime;
   averageFrameTime = averageFrameTime * 0.992 + elapsed * 0.008;
   if(elapsed < minFrameTime) minFrameTime = elapsed;

   let time_out = nextFrame - now;
   if(time_out < 0 || throttle) {
      time_out = 0;
      nextFrame = undefined;      
   }
   if(!stopped) setTimeout(()=>oneFrame(), time_out);   
}


function main() {

   parseQueryStringCommands();

   vic20.init();

   goAudio();

   // rom autoload
   if(autoload !== undefined) {
      autoload.forEach((e,i)=>rom_load(i,e));
   }

   // starts drawing frames
   oneFrame();
}

function cpu_actual_speed() {
   return (total_cycles / (new Date().valueOf() - cpu_started_msec)) * 1000;
}
