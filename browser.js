// handles interaction between browser and emulation 

function onResize(e) {
   const canvas = document.getElementById("canvas");   

   if(window.innerWidth > (window.innerHeight*aspect))
   {
      canvas.style.width  = `${aspect*100}vmin`;
      canvas.style.height = "100vmin";
   }
   else if(window.innerWidth > window.innerHeight)
   {
      canvas.style.width  = "100vmax";
      canvas.style.height = `${(1/aspect)*100}vmax`;
   }
   else
   {
      canvas.style.width  = "100vmin";
      canvas.style.height = `${(1/aspect)*100}vmin`;
   }

   const trueHeight = canvas.offsetHeight
}

function goFullScreen() 
{
        if(canvas.webkitRequestFullscreen !== undefined) canvas.webkitRequestFullscreen();
   else if(canvas.mozRequestFullScreen !== undefined) canvas.mozRequestFullScreen();      
   onResize();
}

window.addEventListener("resize", onResize);
window.addEventListener("dblclick", goFullScreen);

onResize();

// **** save state on close ****

window.onbeforeunload = function(e) {
   saveState();   
 };

// **** visibility change ****

window.addEventListener("visibilitychange", function() {
   if(document.visibilityState === "hidden")
   {
      stopped = true;
      stopAudio();
   }
   else if(document.visibilityState === "visible")
   {
      stopped = false;
      oneFrame();
      goAudio();
   }
});

// **** drag & drop ****

const dropZone = document.getElementById('screen');

// Optional.   Show the copy icon when dragging over.  Seems to only work for chrome.
dropZone.addEventListener('dragover', function(e) {
   e.stopPropagation();
   e.preventDefault();
   e.dataTransfer.dropEffect = 'copy';
});

// Get file data on drop
dropZone.addEventListener('drop', e => {
   audioContextResume();

   e.stopPropagation();
   e.preventDefault();
   const files = e.dataTransfer.files; // Array of all files

   for(let i=0, file; file=files[i]; i++) {                   
      const reader = new FileReader();      
      reader.onload = e2 => droppedFile(file.name, new Uint8Array(e2.target.result));
      reader.readAsArrayBuffer(file); 
   }
});

async function droppedFile(outName, bytes) {
   const prg = /\.prg$/i;
   if(prg.test(outName)) {     
      await writeFile(outName, bytes);
      await crun(outName);
   }

   const tap = /\.tap$/i;
   if(tap.test(outName)) {
      await writeFile(outName, bytes);
      await crun(outName);
   }
}

function getQueryStringObject(options) {
   let a = window.location.search.split("&");
   let o = a.reduce((o, v) =>{
      var kv = v.split("=");
      const key = kv[0].replace("?", "");
      let value = kv[1];
           if(value === "true") value = true;
      else if(value === "false") value = false;
      o[key] = value;
      return o;
   }, options);
   return o;
}

async function parseQueryStringCommands() {
   options = getQueryStringObject(options);  

   if(options.config !== undefined) {
      vic20.config(options.config);
   }

   if(options.joy !== undefined) {
      vic20.emu_joy(options.joy);
      console.log(`Joystick emulation ${options.joy==1?"enabled":"disabled"}`);
   }

   if(options.b !== undefined) {
      const encoded_file = options.b;
      let prg = window.atob(encoded_file).split(",").map(i=>Number(i));
      const bytes = new Uint8Array(prg);
      let filename = "binary.prg";
      await writeFile(filename, bytes);
      await crun(filename);
   }

   if(options.load !== undefined) {
      const name = options.load;
      if(name.startsWith("http")) {
         // external load
         externalLoad("loadPrg", name);
      }
      else {
         // internal load
         await fetchProgram(name);
      }   
   }
}

async function fetchProgram(name)
{
   //console.log(`wanting to load ${name}`);
   try
   {
      const response = await fetch(`software/${name}`);
      if(response.status === 404) return false;
      const bytes = new Uint8Array(await response.arrayBuffer());
      loadBytes({
         buffer: bytes,
         length: bytes.length
      });
      return true;
   }
   catch(err)
   {
      return false;      
   }
}

