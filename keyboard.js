
let emulate_joystick = false;

function keyDown(e) {

   // from Chrome 71 audio is suspended by default and must resume within an user-generated event
   audioContextResume();

   if(e.getModifierState("ScrollLock")!=emulate_joystick) {
      emulate_joystick = e.getModifierState("ScrollLock");
      vic20.emu_joy(emulate_joystick);
   }

   // disable auto repeat, as it is handled on the firmware
   if(e.repeat) {
      e.preventDefault(); 
      return;
   }   

   // *** special (non characters) keys ***   

   // RESET key is CTRL+ALT+BREAK
   if(e.code === "Pause" && e.altKey && e.ctrlKey) {
      vic20.reset();
      e.preventDefault(); 
      return;
   }

   // if keyboard ITA
   {
      const hardware_keys = pckey_to_hardware_keys_ITA(e.code, e.key, e);
      if(hardware_keys.length === 0) return;
      keyboard_buffer.push({ type: "press", hardware_keys });
      e.preventDefault();
   }
}

function keyUp(e) {
   const hardware_keys = pckey_to_hardware_keys_ITA(e.code, e.key, e);
   if(hardware_keys.length === 0) return;
   keyboard_buffer.push({ type: "release", hardware_keys });
   e.preventDefault();
}

// connect DOM events
const element = document; 
element.onkeydown = keyDown;
element.onkeyup = keyUp;

let keyboard_buffer = [];
