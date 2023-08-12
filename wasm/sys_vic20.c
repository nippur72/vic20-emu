#include <stdint.h>
#include <stdbool.h>

#include <emscripten/emscripten.h>

typedef uint8_t byte;
typedef uint16_t word;

#define CHIPS_ASSERT(c) 1
#include <assert.h>

#define CHIPS_IMPL

#include "chips/chips_common.h"

#include "chips/m6502.h"
#include "chips/m6522.h"
#include "chips/m6561.h"
#include "chips/kbd.h"
#include "chips/mem.h"
#include "chips/clk.h"
#include "systems/c1530.h"
#include "systems/vic20.h"
#include "roms/vic20-roms.h"

vic20_desc_t desc;
vic20_t sys;

#define AUDIOBUFSIZE (1024)
float audio_buffer[AUDIOBUFSIZE];

void audio_cb(const float* samples, int num_samples, void* user_data) {
   byte unused = (byte) EM_ASM_INT({ audio_buf_ready($0, $1); }, samples, num_samples );
}

uint32_t vic20palette_RGBA[16] = {
    //AABBGGRR
    0xFF000000,   // black
    0xFFFFFFFF,   // white
    0xFF211FB6,   // red
    0xFFFFFF00,   // cyan
    0xFFFF3FB4,   // purple
    0xFF37E244,   // green
    0xFFFF341A,   // blue
    0xFF1BD7DC,   // yellow
    0xFF0054CA,   // orange
    0xFF72B0E9,   // light orange
    0xFF9392E7,   // light red
    0xFFFDF79A,   // light cyan
    0xFFFF9FE0,   // light purple
    0xFF93E48F,   // light green
    0xFFFF9082,   // ligth blue
    0xFF85DEE5    // light yellow
};

#define PIXEL_WIDTH_2 2

#ifdef PIXEL_WIDTH_2
uint32_t pixel_buffer[208*264*4];
void end_frame_cb(void* user_data) {
   uint8_t *src = sys.vic.crt.fb;
   uint32_t *dst = pixel_buffer;
   uint32_t pixel;

   src += 8; // center 1 char

   for(int r=0;r<264;r++) {
      for(int c=0;c<208;c++) { 
         pixel = vic20palette_RGBA[*src];
         dst[0]       = pixel;
         dst[1]       = pixel;
         dst[208*2]   = pixel; 
         dst[208*2+1] = pixel; 
         src++;    
         dst+=2;
      }   
      src += 512 - 208;
      dst += 208*2;
   }
   byte unused = (byte) EM_ASM_INT({ m6561_screen_update($0); }, pixel_buffer );
}
#else
uint32_t pixel_buffer[208*264];
void end_frame_cb(void* user_data) {
   uint8_t *src = sys.vic.crt.fb;
   uint32_t *dst = (uint32_t *)pixel_buffer;
   uint32_t vic20_pixel;   

   src += 8; // center 1 char

   for(int r=0;r<264;r++) {
      for(int c=0;c<208;c++) { 
         vic20_pixel = vic20palette_RGBA[*src];         
         *dst++ = vic20_pixel;
         src++;    
      }   
      src += 512-208;
   }
   byte unused = (byte) EM_ASM_INT({ m6561_screen_update($0); }, pixel_buffer );
}
#endif

EMSCRIPTEN_KEEPALIVE
void sys_init(vic20_memory_config_t config) {

   desc.c1530_enabled = false;                       /* enable the C1530 datassette emulation */
   desc.mem_config    = config;                      /* default is VIC20_MEMCONFIG_STANDARD */
   
   // audio
   desc.audio.callback = (chips_audio_callback_t) { audio_cb, &sys };
   desc.audio.num_samples = AUDIOBUFSIZE;
   desc.audio.sample_rate = 48000;
   desc.audio.volume = 1.0;

   // ROM images
   desc.roms.chars = (chips_range_t) { dump_vic20_characters_901460_03_bin, 4096 };
   desc.roms.basic = (chips_range_t) { dump_vic20_basic_901486_01_bin, 8192 };
   desc.roms.kernal = (chips_range_t) { dump_vic20_kernal_901486_07_bin, 8192 };

   vic20_init(&sys, &desc);

   sys.vic.end_frame_cb = end_frame_cb;
}

EMSCRIPTEN_KEEPALIVE
void sys_reset() {
   vic20_reset(&sys);
}

EMSCRIPTEN_KEEPALIVE
void sys_exec_us(uint32_t msec) {
   vic20_exec(&sys, msec);
}

EMSCRIPTEN_KEEPALIVE
void sys_key_down(int key_code) {
   vic20_key_down(&sys, key_code);
}

EMSCRIPTEN_KEEPALIVE
void sys_key_up(int key_code) {
   vic20_key_up(&sys, key_code);
}

EMSCRIPTEN_KEEPALIVE
void sys_quick_load(uint8_t *bytes, int num_bytes) {
   vic20_quickload(&sys, (chips_range_t) { bytes, num_bytes });
}

EMSCRIPTEN_KEEPALIVE
void sys_insert_rom_cartdrige(uint8_t *bytes, int num_bytes) {
   vic20_insert_rom_cartridge(&sys, (chips_range_t) { bytes, num_bytes });
}

EMSCRIPTEN_KEEPALIVE
uint8_t sys_mem_cpu_rd(uint16_t address) {
   return mem_rd(&sys.mem_cpu, address);
}

EMSCRIPTEN_KEEPALIVE
void sys_mem_cpu_wr(uint16_t address, uint8_t data) {
   mem_wr(&sys.mem_cpu, address, data);
}

EMSCRIPTEN_KEEPALIVE
void sys_config(vic20_memory_config_t config) {
   sys_init(config);
   sys_reset();
}

EMSCRIPTEN_KEEPALIVE
void sys_joystick(uint8_t joy_mask) {
   vic20_joystick(&sys, joy_mask);
}

EMSCRIPTEN_KEEPALIVE
void sys_set_joystick_type(vic20_joystick_type_t type) {
   desc.joystick_type = type;
   vic20_set_joystick_type(&sys, desc.joystick_type);
}

EMSCRIPTEN_KEEPALIVE
bool sys_insert_tape(uint8_t* ptr, int num_bytes) {
   return vic20_insert_tape(&sys, (chips_range_t) { ptr, num_bytes } );
}

EMSCRIPTEN_KEEPALIVE
void sys_remove_tape() {
   vic20_remove_tape(&sys);
}

EMSCRIPTEN_KEEPALIVE
void sys_tape_play() {
   vic20_tape_play(&sys);
}

EMSCRIPTEN_KEEPALIVE
void sys_tape_stop() {
   vic20_tape_stop(&sys);
}

EMSCRIPTEN_KEEPALIVE
uint8_t sys_cas_port() {
   return sys.cas_port;
}

EMSCRIPTEN_KEEPALIVE
void sys_dump_vic() {
   for(int t=0;t<16;t++) {
      byte unused = (byte) EM_ASM_INT({ console.log($0, $1); }, t, sys.vic.regs[t] );
   }
}


///* enable/disable joystick emulation */
//void vic20_set_joystick_type(vic20_t* sys, vic20_joystick_type_t type);
///* get current joystick emulation type */
//vic20_joystick_type_t vic20_joystick_type(vic20_t* sys);
///* set joystick mask (combination of VIC20_JOYSTICK_*) */
//void vic20_joystick(vic20_t* sys, uint8_t joy_mask);
///* quickload a .prg/.bin file */
//bool vic20_quickload(vic20_t* sys, const uint8_t* ptr, int num_bytes);
///* load a .prg/.bin file as ROM cartridge */
//bool vic20_insert_rom_cartridge(vic20_t* sys, const uint8_t* ptr, int num_bytes);
///* remove current ROM cartridge */
//void vic20_remove_rom_cartridge(vic20_t* sys);
///* insert tape as .TAP file (c1530 must be enabled) */
//bool vic20_insert_tape(vic20_t* sys, const uint8_t* ptr, int num_bytes);
///* remove tape file */
//void vic20_remove_tape(vic20_t* sys);
///* return true if a tape is currently inserted */
//bool vic20_tape_inserted(vic20_t* sys);
///* start the tape (press the Play button) */
//void vic20_tape_play(vic20_t* sys);
///* stop the tape (unpress the Play button */
//void vic20_tape_stop(vic20_t* sys);
///* return true if tape motor is on */
//bool vic20_is_tape_motor_on(vic20_t* sys);
//*/
