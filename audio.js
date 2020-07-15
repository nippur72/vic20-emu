
const AUDIO_BUFSIZE = 4096;  // must match .c

let audio_buffers_queue = [];

function audio_buf_ready(ptr, size) {
   if(!audio_playing) return;

   let start = ptr / wasm_instance.HEAPF32.BYTES_PER_ELEMENT;
   let buffer = wasm_instance.HEAPF32.subarray(start,start+size);

   audio_buffers_queue.push([ ...buffer ]);  // push a cloned copy
}

// ********************************* AUDIO BUFFER TO BROWSER AUDIO ************************************

let audioContext = new (window.AudioContext || window.webkitAudioContext)();
const bufferSize = AUDIO_BUFSIZE;
const sampleRate = audioContext.sampleRate;
var speakerSound = audioContext.createScriptProcessor(bufferSize, 1, 1);

/*
let ch0 = [];
let ch1 = [];

function csave() {
   const wavData = {
      sampleRate: 48000,
      channelData: [ new Float32Array(ch0), new Float32Array(ch1) ]
   };
     
   const buffer = encodeSync(wavData, { bitDepth: 16, float: false });      
   
   let blob = new Blob([buffer], {type: "application/octet-stream"});   
   const fileName = "csaved.wav";
   saveAs(blob, fileName);
}
*/

speakerSound.onaudioprocess = function(e) {
   const output = e.outputBuffer.getChannelData(0);

   if(audio_buffers_queue.length === 0) {
      // console.log("warning: audio queue is empty");
      return;
   }
   else if(audio_buffers_queue.length > 10) {
      // console.log(`warning: audio queue is getting longer: ${audio_buffers_queue.length}`);
      audio_buffers_queue = [];
      return;
   }

   const buffer = audio_buffers_queue[0];
   audio_buffers_queue = audio_buffers_queue.slice(1);

   for(let i=0; i<bufferSize; i++) {
      output[i] = buffer[i];
   }
}

let audio_playing = false;

function goAudio() {
   speakerSound.connect(audioContext.destination);
   audio_playing = true;
   audio_buffers_queue = [];
}

function stopAudio() {
   speakerSound.disconnect(audioContext.destination);
   audio_playing = false;
}

function audioContextResume() {
   if(audioContext.state === 'suspended') {
      audioContext.resume().then(() => {
        audio_buffers_queue = [];
        // console.log('sound playback resumed successfully');
      });
   }
}
