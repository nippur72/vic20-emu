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
   //console.log(code, key, e);

   let hardware_keys = [];

   if(key === "1")             hardware_keys.push( "1".charCodeAt(0) );
   if(key === "2")             hardware_keys.push( "2".charCodeAt(0) );
   if(key === "3")             hardware_keys.push( "3".charCodeAt(0) );
   if(key === "4")             hardware_keys.push( "4".charCodeAt(0) );
   if(key === "5")             hardware_keys.push( "5".charCodeAt(0) );
   if(key === "6")             hardware_keys.push( "6".charCodeAt(0) );
   if(key === "7")             hardware_keys.push( "7".charCodeAt(0) );
   if(key === "8")             hardware_keys.push( "8".charCodeAt(0) );
   if(key === "9")             hardware_keys.push( "9".charCodeAt(0) );
   if(key === "0")             hardware_keys.push( "0".charCodeAt(0) );

   if(e.shiftKey) {
      if(code === "KeyQ")         hardware_keys.push( "q".charCodeAt(0) );
      if(code === "KeyW")         hardware_keys.push( "w".charCodeAt(0) );
      if(code === "KeyE")         hardware_keys.push( "e".charCodeAt(0) );
      if(code === "KeyR")         hardware_keys.push( "r".charCodeAt(0) );
      if(code === "KeyT")         hardware_keys.push( "t".charCodeAt(0) );
      if(code === "KeyY")         hardware_keys.push( "y".charCodeAt(0) );
      if(code === "KeyU")         hardware_keys.push( "u".charCodeAt(0) );
      if(code === "KeyI")         hardware_keys.push( "i".charCodeAt(0) );
      if(code === "KeyO")         hardware_keys.push( "o".charCodeAt(0) );
      if(code === "KeyP")         hardware_keys.push( "p".charCodeAt(0) );
      if(code === "KeyA")         hardware_keys.push( "a".charCodeAt(0) );
      if(code === "KeyS")         hardware_keys.push( "s".charCodeAt(0) );
      if(code === "KeyD")         hardware_keys.push( "d".charCodeAt(0) );
      if(code === "KeyF")         hardware_keys.push( "f".charCodeAt(0) );
      if(code === "KeyG")         hardware_keys.push( "g".charCodeAt(0) );
      if(code === "KeyH")         hardware_keys.push( "h".charCodeAt(0) );
      if(code === "KeyJ")         hardware_keys.push( "j".charCodeAt(0) );
      if(code === "KeyK")         hardware_keys.push( "k".charCodeAt(0) );
      if(code === "KeyL")         hardware_keys.push( "l".charCodeAt(0) );
      if(code === "KeyZ")         hardware_keys.push( "z".charCodeAt(0) );
      if(code === "KeyX")         hardware_keys.push( "x".charCodeAt(0) );
      if(code === "KeyC")         hardware_keys.push( "c".charCodeAt(0) );
      if(code === "KeyV")         hardware_keys.push( "v".charCodeAt(0) );
      if(code === "KeyB")         hardware_keys.push( "b".charCodeAt(0) );
      if(code === "KeyN")         hardware_keys.push( "n".charCodeAt(0) );
      if(code === "KeyM")         hardware_keys.push( "m".charCodeAt(0) );
   }
   else {
      if(code === "KeyQ")         hardware_keys.push( "Q".charCodeAt(0) );
      if(code === "KeyW")         hardware_keys.push( "W".charCodeAt(0) );
      if(code === "KeyE")         hardware_keys.push( "E".charCodeAt(0) );
      if(code === "KeyR")         hardware_keys.push( "R".charCodeAt(0) );
      if(code === "KeyT")         hardware_keys.push( "T".charCodeAt(0) );
      if(code === "KeyY")         hardware_keys.push( "Y".charCodeAt(0) );
      if(code === "KeyU")         hardware_keys.push( "U".charCodeAt(0) );
      if(code === "KeyI")         hardware_keys.push( "I".charCodeAt(0) );
      if(code === "KeyO")         hardware_keys.push( "O".charCodeAt(0) );
      if(code === "KeyP")         hardware_keys.push( "P".charCodeAt(0) );
      if(code === "KeyA")         hardware_keys.push( "A".charCodeAt(0) );
      if(code === "KeyS")         hardware_keys.push( "S".charCodeAt(0) );
      if(code === "KeyD")         hardware_keys.push( "D".charCodeAt(0) );
      if(code === "KeyF")         hardware_keys.push( "F".charCodeAt(0) );
      if(code === "KeyG")         hardware_keys.push( "G".charCodeAt(0) );
      if(code === "KeyH")         hardware_keys.push( "H".charCodeAt(0) );
      if(code === "KeyJ")         hardware_keys.push( "J".charCodeAt(0) );
      if(code === "KeyK")         hardware_keys.push( "K".charCodeAt(0) );
      if(code === "KeyL")         hardware_keys.push( "L".charCodeAt(0) );
      if(code === "KeyZ")         hardware_keys.push( "Z".charCodeAt(0) );
      if(code === "KeyX")         hardware_keys.push( "X".charCodeAt(0) );
      if(code === "KeyC")         hardware_keys.push( "C".charCodeAt(0) );
      if(code === "KeyV")         hardware_keys.push( "V".charCodeAt(0) );
      if(code === "KeyB")         hardware_keys.push( "B".charCodeAt(0) );
      if(code === "KeyN")         hardware_keys.push( "N".charCodeAt(0) );
      if(code === "KeyM")         hardware_keys.push( "M".charCodeAt(0) );
   }

   if(key === "!")             hardware_keys.push( "!".charCodeAt(0) );
   if(key === '"')             hardware_keys.push( '"'.charCodeAt(0) );
   if(key === "£")             hardware_keys.push( "~".charCodeAt(0) );
   if(key === "$")             hardware_keys.push( "$".charCodeAt(0) );
   if(key === "%")             hardware_keys.push( "%".charCodeAt(0) );
   if(key === "&")             hardware_keys.push( "&".charCodeAt(0) );
   if(key === "/")             hardware_keys.push( "/".charCodeAt(0) );
   if(key === "(")             hardware_keys.push( "(".charCodeAt(0) );
   if(key === ")")             hardware_keys.push( ")".charCodeAt(0) );
   if(key === "=")             hardware_keys.push( "=".charCodeAt(0) );
   if(key === "'")             hardware_keys.push( "'".charCodeAt(0) );
   if(key === "?")             hardware_keys.push( "?".charCodeAt(0) );
   if(key === "^")             hardware_keys.push( "^".charCodeAt(0) );
   if(key === "[")             hardware_keys.push( "[".charCodeAt(0) );
   if(key === "]")             hardware_keys.push( "]".charCodeAt(0) );
   if(key === "+")             hardware_keys.push( "+".charCodeAt(0) );
   if(key === "*")             hardware_keys.push( "*".charCodeAt(0) );
   if(key === "@")             hardware_keys.push( "@".charCodeAt(0) );
   if(key === "#")             hardware_keys.push( "#".charCodeAt(0) );
   if(key === "<")             hardware_keys.push( "<".charCodeAt(0) );
   if(key === ">")             hardware_keys.push( ">".charCodeAt(0) );
   if(key === ",")             hardware_keys.push( ",".charCodeAt(0) );
   if(key === ";")             hardware_keys.push( ";".charCodeAt(0) );
   if(key === ".")             hardware_keys.push( ".".charCodeAt(0) );
   if(key === ":")             hardware_keys.push( ":".charCodeAt(0) );
   if(key === "-")             hardware_keys.push( "-".charCodeAt(0) );

   // FKEYS
   if(code === "F1")           hardware_keys.push( 0xF1 );
   if(code === "F2")           hardware_keys.push( 0xF2 );
   if(code === "F3")           hardware_keys.push( 0xF3 );
   if(code === "F4")           hardware_keys.push( 0xF4 );
   if(code === "F5")           hardware_keys.push( 0xF5 );
   if(code === "F6")           hardware_keys.push( 0xF6 );
   if(code === "F7")           hardware_keys.push( 0xF7 );
   if(code === "F8")           hardware_keys.push( 0xF8 );

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

   if(code === "ControlLeft")  hardware_keys.push( 0x0E );
   if(code === "ControlRight") hardware_keys.push( 0x0E );
   if(code === "Tab")          hardware_keys.push( 0x0E );  // emulate CTRL
   if(code === "AltLeft")      hardware_keys.push( 0x0F );  // emulate CBM
   if(code === "PageUp")       hardware_keys.push( 0xFF );  // emulate RESTORE

   return hardware_keys;
}
