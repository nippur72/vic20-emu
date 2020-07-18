#include <stdint.h>
#include <stdbool.h>

#include <emscripten/emscripten.h>

typedef uint8_t byte;
typedef uint16_t word;

#define CHIPS_IMPL

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

#define PIXBUFSIZE (708864)
unsigned char pixel_buffer[PIXBUFSIZE];

#define AUDIOBUFSIZE (4096)
float audio_buffer[AUDIOBUFSIZE];

void audio_cb(const float* samples, int num_samples, void* user_data) {
   byte unused = (byte) EM_ASM_INT({ audio_buf_ready($0, $1); }, samples, num_samples );
}

EMSCRIPTEN_KEEPALIVE
void sys_init() {

   desc.user_data = NULL;                            /* optional user-data for callback functions */

   desc.c1530_enabled = false;                       /* set to true to enable C1530 datassette emulation */
   desc.joystick_type = VIC20_JOYSTICKTYPE_NONE;     /* default is VIC20_JOYSTICK_NONE */
   desc.mem_config    = VIC20_MEMCONFIG_STANDARD;    /* default is VIC20_MEMCONFIG_STANDARD */

   // video
   desc.pixel_buffer = pixel_buffer;                 /* pointer to a linear RGBA8 pixel buffer, query required size via vic20_max_display_size() */
   desc.pixel_buffer_size = PIXBUFSIZE;                  /* size of the pixel buffer in bytes */

   // audio
   desc.audio_cb = audio_cb;                         /* called when audio_num_samples are ready */
   desc.audio_buffer = audio_buffer;
   desc.audio_num_samples = AUDIOBUFSIZE;
   desc.audio_sample_rate = 48000;                   /* playback sample rate in Hz, default is 44100 */
   desc.audio_volume = 1.0;                          /* audio volume of the VIC chip (0.0 .. 1.0), default is 1.0 */

   // ROM images
   desc.rom_char = dump_vic20_characters_901460_03_bin;      /* 4 KByte character ROM dump */
   desc.rom_basic = dump_vic20_basic_901486_01_bin;          /* 8 KByte BASIC dump */
   desc.rom_kernal = dump_vic20_kernal_901486_07_bin;        /* 8 KByte KERNAL dump */
   desc.rom_char_size = 4096;
   desc.rom_basic_size = 8192;
   desc.rom_kernal_size = 8192;

   vic20_init(&sys, &desc);

   vic20_reset(&sys);

   /*
   int w = vic20_std_display_width();
   int h = vic20_std_display_height();
   byte unused = (byte) EM_ASM_INT({ console.log($0, $1); }, w, h );
   */
}

EMSCRIPTEN_KEEPALIVE
void sys_reset() {
   vic20_reset(&sys);
}

EMSCRIPTEN_KEEPALIVE
void sys_exec() {
   vic20_exec(&sys, 16666);
   byte unused = (byte) EM_ASM_INT({ vdp_screen_update($0); }, pixel_buffer );
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
   vic20_quickload(&sys, bytes, num_bytes);
}

EMSCRIPTEN_KEEPALIVE
uint8_t sys_mem_cpu_rd(uint16_t address) {
   return mem_rd(&sys.mem_cpu, address);
}

EMSCRIPTEN_KEEPALIVE
void sys_mem_cpu_wr(uint16_t address, uint8_t data) {
   mem_wr(&sys.mem_cpu, address, data);
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
