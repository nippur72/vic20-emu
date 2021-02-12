"use strict";

// TODO basic chain after load
// TODO uppercase/lowercase (shift lock)
// TODO fix for alt/cbm/shift what keyboard?
// TODO shift problem  SHIFT + 9
// TODO 1541 emulation

/******************/

let stopped = false; // allows to stop/resume the emulation

let frames = 0;
let averageFrameTime = 0;

let cycle = 0;
let total_cycles = 0;

let throttle = false;

let end_of_frame_hook = undefined;

let options = {
   load: undefined,
   restore: false
};

let last_keyboardpoll = 0;

function poll_keyboard() {
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
}

let last_timestamp = 0;

function oneFrame(timestamp) {
   let stamp = timestamp == undefined ? last_timestamp : timestamp;
   let usec = (stamp - last_timestamp)*1000;
   last_timestamp = stamp;

   if(usec > 100000) usec = 100000;

   if(stamp - last_keyboardpoll > 30) {
      poll_keyboard();
      last_keyboardpoll = stamp;
   }

   vic20.exec_us(usec);

   averageFrameTime = averageFrameTime * 0.992 + usec * 0.008;

   if(!stopped) requestAnimationFrame(oneFrame);
}

function main() {

   vic20.config(0);

   parseQueryStringCommands();

   // rom autoload
   if(autoload !== undefined) {
      autoload.forEach((e,i)=>rom_load(i,e));
   }

   // starts drawing frames
   goAudio();
   oneFrame();
}
