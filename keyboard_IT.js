/*
     7   6   5   4   3   2   1   0
    --------------------------------
  7| F7  F5  F3  F1  CDN CRT RET DEL    CRT=Cursor-Right, CDN=Cursor-Down
   |
  6| HOM UA  =   RSH /   ;   *   BP     BP=British Pound, RSH=Should be Right-SHIFT,
   |                                    UA=Up Arrow
  5| -   @   :   .   ,   L   P   +
   |
  4| 0   O   K   M   N   J   I   9
   |
  3| 8   U   H   B   V   G   Y   7
   |
  2| 6   T   F   C   X   D   R   5
   |
  1| 4   E   S   Z   LSH A   W   3      LSH=Should be Left-SHIFT
   |
  0| 2   Q   CBM SPC STP CTL LA  1      LA=Left Arrow, CTL=Should be CTRL, STP=RUN/STOP
   |                                    CBM=Commodore key

   */

function pckey_to_hardware_keys_ITA(code, key, e) {
   console.log(code, key, e);

   let hardware_keys = [];

   //if(e.ctrlKey) hardware_keys.push( KEY_CTRL );
   //if(e.altKey) hardware_keys.push( KEY_CBM );

   //if(code === "Home" && e.shiftKey) hardware_keys.push( KEY_SHIFT, KEY_CLR_HOME  );

   // ASCII 32-126
   if(key.length==1 && (key.charCodeAt(0) > 32 && key.charCodeAt(0) < 127) ) hardware_keys.push( key.charCodeAt(0) );

   // FKEYS
   if(code === "F1")           hardware_keys.push( 0xF1 );
   if(code === "F2")           hardware_keys.push( 0xF2 );
   if(code === "F3")           hardware_keys.push( 0xF3 );
   if(code === "F4")           hardware_keys.push( 0xF4 );
   if(code === "F5")           hardware_keys.push( 0xF5 );
   if(code === "F6")           hardware_keys.push( 0xF6 );
   if(code === "F7")           hardware_keys.push( 0xF7 );
   if(code === "F8")           hardware_keys.push( 0xF8 );
   if(key === "£")             hardware_keys.push( "~".charCodeAt(0)  );

   if(code === "Space")        hardware_keys.push( 0x20  );
   if(code === "ArrowLeft")    hardware_keys.push( 0x08  );
   if(code === "ArrowRight")   hardware_keys.push( 0x09  );
   if(code === "ArrowDown")    hardware_keys.push( 0x0a  );
   if(code === "ArrowUp")      hardware_keys.push( 0x0b  );

   if(code === "Delete")       hardware_keys.push( 0x01  );
   if(code === "Insert")       hardware_keys.push( 0x07  );

   if(code === "Backspace" &&  e.shiftKey) hardware_keys.push( 0x07  );
   if(code === "Backspace" && !e.shiftKey) hardware_keys.push( 0x01  );

   if(code === "Enter")        hardware_keys.push( 0x0D  );
   if(code === "Escape")       hardware_keys.push( 0x03  );  // run stop
   if(code === "Backquote")    hardware_keys.push( 0x04  );  // left arrow

   if(code === "Home")               hardware_keys.push( 0x05  );
   if(code === "Home" && e.shiftKey) hardware_keys.push( 0x06  );

   // TODO Restore key

   //if(code === "End")          hardware_keys.push( KEY_SHIFT, KEY_CLR_HOME );

   //if(code === "NumpadEnter")  hardware_keys.push( KEY_RETURN  );

   if(code === "ControlLeft")  hardware_keys.push( 0x0E );
   if(code === "ControlRight") hardware_keys.push( 0x0E );
   if(code === "AltLeft")      hardware_keys.push( 0x0F );

   //if(code === "Tab")          hardware_keys.push( KEY_RUN_STOP );
   //if(key === "\\")            hardware_keys.push( KEY_ESC  );

   // disabled because of brower bug
   //if(code === "Pause")        hardware_keys.push( KEY_RUN_STOP );

   //if(key === "{")             hardware_keys.push(  );
   //if(key === "}")             hardware_keys.push(  );

   //if(key === "|")             hardware_keys.push(   );
   //if(key === "_")             hardware_keys.push(  );

   //if(code === "PageUp")       hardware_keys.push(  );
   //if(code === "PageDown")     hardware_keys.push(  );
   //if(code === "CapsLock")     hardware_keys.push(  );

   //console.log(hardware_keys);
   return hardware_keys;
}
