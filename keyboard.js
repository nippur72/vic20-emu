
let last_scroll_lock = undefined;

function keyDown(e) {

   // from Chrome 71 audio is suspended by default and must resume within an user-generated event
   audioContextResume();

   if(last_scroll_lock === undefined) last_scroll_lock = e.getModifierState("ScrollLock");

   if(e.getModifierState("ScrollLock")!=last_scroll_lock) {
      let scroll_lock_key_pressed = e.getModifierState("ScrollLock");
      last_scroll_lock = scroll_lock_key_pressed;
      let emu_joystick = scroll_lock_key_pressed ? 1 : 0;
      vic20.emu_joy(emu_joystick);
      console.log(`Joystick emulation ${emu_joystick==1?"enabled":"disabled"}`);
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
