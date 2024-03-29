#pragma once
/*#
    # vic20.h

    A Commodore VIC-20 emulator in a C header.

    Do this:
    ~~~C
    #define CHIPS_IMPL
    ~~~
    before you include this file in *one* C or C++ file to create the 
    implementation.

    Optionally provide the following macros with your own implementation
    
    ~~~C
    CHIPS_ASSERT(c)
    ~~~
        your own assert macro (default: assert(c))

    You need to include the following headers before including vic20.h:

    - chips/m6502.h
    - chips/m6522.h
    - chips/m6561.h
    - chips/kbd.h
    - chips/mem.h
    - chips/clk.h
    - systems/c1530.h

    ## The Commodore VIC-20


    TODO!

    ## Links

    http://blog.tynemouthsoftware.co.uk/2019/09/how-the-vic20-works.html

    ## zlib/libpng license

    Copyright (c) 2018 Andre Weissflog
    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.
    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:
        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software in a
        product, an acknowledgment in the product documentation would be
        appreciated but is not required.
        2. Altered source versions must be plainly marked as such, and must not
        be misrepresented as being the original software.
        3. This notice may not be removed or altered from any source
        distribution. 
#*/
#include <stdint.h>
#include <stdbool.h>

// #include "..\tms9928.h"
// extern tms9928_t vdp;

#ifdef __cplusplus
extern "C" {
#endif

#define VIC20_FREQUENCY (1108404)

/* VIC-20 joystick types (only one joystick supported) */
typedef enum {
    VIC20_JOYSTICKTYPE_NONE,
    VIC20_JOYSTICKTYPE_DIGITAL,
} vic20_joystick_type_t;

/* memory configuration (used in vic20_desc_t.mem_config) */
typedef enum {
    VIC20_MEMCONFIG_STANDARD,       /* unexpanded */
    VIC20_MEMCONFIG_3K,             /* Block 0 (3KB at 0400..0FFF) */
    VIC20_MEMCONFIG_8K,             /* Block 1 */
    VIC20_MEMCONFIG_16K,            /* Block 1+2 */
    VIC20_MEMCONFIG_24K,            /* Block 1+2+3 */
    VIC20_MEMCONFIG_32K,            /* Block 1+2+3+5 (note that BASIC can only use blocks 1+2+3) */
    VIC20_MEMCONFIG_MAX             /* 32K + 3KB at 0400..0FFF */
} vic20_memory_config_t;

/* joystick mask bits */
#define VIC20_JOYSTICK_UP    (1<<0)
#define VIC20_JOYSTICK_DOWN  (1<<1)
#define VIC20_JOYSTICK_LEFT  (1<<2)
#define VIC20_JOYSTICK_RIGHT (1<<3)
#define VIC20_JOYSTICK_BTN   (1<<4)

/* casette port bits, same as C1530_CASPORT_* */
#define VIC20_CASPORT_MOTOR   (1<<0)
#define VIC20_CASPORT_READ    (1<<1)
#define VIC20_CASPORT_WRITE   (1<<2)
#define VIC20_CASPORT_SENSE   (1<<3)

/* IEC port bits, same as C1541_IECPORT_* */
#define VIC20_IECPORT_RESET   (1<<0)
#define VIC20_IECPORT_SRQIN   (1<<1)
#define VIC20_IECPORT_DATA    (1<<2)
#define VIC20_IECPORT_CLK     (1<<3)
#define VIC20_IECPORT_ATN     (1<<4)

/* audio sample data callback */
typedef void (*vic20_audio_callback_t)(const float* samples, int num_samples, void* user_data);

/* config parameters for vic20_init() */
typedef struct {
    bool c1530_enabled;         /* set to true to enable C1530 datassette emulation */
    vic20_joystick_type_t joystick_type;    /* default is VIC20_JOYSTICK_NONE */
    vic20_memory_config_t mem_config;       /* default is VIC20_MEMCONFIG_STANDARD */

    /* video output config (if you don't want video decoding, set these to 0) */
    void* pixel_buffer;         /* pointer to a linear RGBA8 pixel buffer,
                                   query required size via vic20_max_display_size() */
    int pixel_buffer_size;      /* size of the pixel buffer in bytes */

    m6561_end_frame_t end_frame_cb;  /* end of frame callback */

    /* optional user-data for callback functions */
    void* user_data;

    /* audio output config (if you don't want audio, set audio_cb to zero) */
    vic20_audio_callback_t audio_cb;  /* called when audio_num_samples are ready */
    float *audio_buffer;            /* buffer containing audio samples */
    int audio_num_samples;          /* size of the audio buffer */
    int audio_sample_rate;          /* playback sample rate in Hz, default is 44100 */
    float audio_volume;             /* audio volume of the VIC chip (0.0 .. 1.0), default is 1.0 */

    /* ROM images */
    const void* rom_char;           /* 4 KByte character ROM dump */
    const void* rom_basic;          /* 8 KByte BASIC dump */
    const void* rom_kernal;         /* 8 KByte KERNAL dump */
    int rom_char_size;
    int rom_basic_size;
    int rom_kernal_size;
} vic20_desc_t;

/* VIC-20 emulator state */
typedef struct {
    uint64_t pins;
    m6502_t cpu;
    m6522_t via_1;
    m6522_t via_2;
    m6561_t vic;
    
    bool valid;
    vic20_joystick_type_t joystick_type;
    vic20_memory_config_t mem_config;
    uint8_t cas_port;           /* cassette port, shared with c1530_t if datasette is connected */
    uint8_t iec_port;           /* IEC serial port, shared with c1541_t if connected */
    uint8_t kbd_joy_mask;       /* current joystick state from keyboard-joystick emulation */
    uint8_t joy_joy_mask;       /* current joystick state from vic20_joystick() */
    uint64_t via1_joy_mask;     /* merged keyboard/joystick mask ready for or-ing with VIA1 input pins */
    uint64_t via2_joy_mask;     /* merged keyboard/joystick mask ready for or-ing with VIA2 input pins */

    kbd_t kbd;                  /* keyboard matrix state */
    mem_t mem_cpu;              /* CPU-visible memory mapping */
    mem_t mem_vic;              /* VIC-visible memory mapping */

    void* user_data;
    uint32_t* pixel_buffer;
    vic20_audio_callback_t audio_cb;
    int num_samples;
    int sample_pos;
    float* sample_buffer;

    uint8_t color_ram[0x0400];      /* special color RAM */
    uint8_t ram0[0x0400];           /* 1 KB zero page, stack, system work area */
    uint8_t ram_3k[0x0C00];         /* optional 3K exp RAM */
    uint8_t ram1[0x1000];           /* 4 KB main RAM */
    uint8_t rom_char[0x1000];       /* 4 KB character ROM image */
    uint8_t rom_basic[0x2000];      /* 8 KB BASIC ROM image */
    uint8_t rom_kernal[0x2000];     /* 8 KB KERNAL V3 ROM image */
    uint8_t ram_exp[4][0x2000];     /* optional expansion 8K RAM blocks */

    c1530_t c1530;                  /* c1530.valid = true if enabled */

    mem_t mem_cart;                 /* special ROM cartridge memory mapping helper */
} vic20_t;

/* initialize a new VIC-20 instance */
void vic20_init(vic20_t* sys, const vic20_desc_t* desc);
/* discard VIC-20 instance */
void vic20_discard(vic20_t* sys);
/* get the standard framebuffer width and height in pixels */
int vic20_std_display_width(void);
int vic20_std_display_height(void);
/* get the maximum framebuffer size in number of bytes */
int vic20_max_display_size(void);
/* get the current framebuffer width and height in pixels */
int vic20_display_width(vic20_t* sys);
int vic20_display_height(vic20_t* sys);
/* reset a VIC-20 instance */
void vic20_reset(vic20_t* sys);
/* tick VIC-20 instance for a given number of microseconds, also updates keyboard state */
void vic20_exec(vic20_t* sys, uint32_t micro_seconds);
/* ...or optionally: tick the VIC-20 instance once, does not update keyboard state! */
void vic20_tick(vic20_t* sys);
/* send a key-down event to the VIC-20 */
void vic20_key_down(vic20_t* sys, int key_code);
/* send a key-up event to the VIC-20 */
void vic20_key_up(vic20_t* sys, int key_code);
/* enable/disable joystick emulation */
void vic20_set_joystick_type(vic20_t* sys, vic20_joystick_type_t type);
/* get current joystick emulation type */
vic20_joystick_type_t vic20_joystick_type(vic20_t* sys);
/* set joystick mask (combination of VIC20_JOYSTICK_*) */
void vic20_joystick(vic20_t* sys, uint8_t joy_mask);
/* quickload a .prg/.bin file */
bool vic20_quickload(vic20_t* sys, const uint8_t* ptr, int num_bytes);
/* load a .prg/.bin file as ROM cartridge */
bool vic20_insert_rom_cartridge(vic20_t* sys, const uint8_t* ptr, int num_bytes);
/* remove current ROM cartridge */
void vic20_remove_rom_cartridge(vic20_t* sys);
/* insert tape as .TAP file (c1530 must be enabled) */
//bool vic20_insert_tape(vic20_t* sys, const uint8_t* ptr, int num_bytes);
/* remove tape file */
void vic20_remove_tape(vic20_t* sys);
/* return true if a tape is currently inserted */
bool vic20_tape_inserted(vic20_t* sys);
/* start the tape (press the Play button) */
void vic20_tape_play(vic20_t* sys);
/* stop the tape (unpress the Play button */
void vic20_tape_stop(vic20_t* sys);
/* return true if tape motor is on */
bool vic20_is_tape_motor_on(vic20_t* sys);

#ifdef __cplusplus
} /* extern "C" */
#endif

/*-- IMPLEMENTATION ----------------------------------------------------------*/
#ifdef CHIPS_IMPL
#include <string.h> /* memcpy, memset */
#ifndef CHIPS_ASSERT
    #include <assert.h>
    #define CHIPS_ASSERT(c) assert(c)
#endif

/* checked on CRT PAL display */
#define _VIC20_DISPLAY_X (5*8)
#define _VIC20_DISPLAY_Y (6*8)
#define _VIC20_STD_DISPLAY_WIDTH  (208)
#define _VIC20_STD_DISPLAY_HEIGHT (264)

/*
// parameters for debug_vis = true
#define _VIC20_DISPLAY_X (0)
#define _VIC20_DISPLAY_Y (0)
#define _VIC20_STD_DISPLAY_WIDTH  (284)
#define _VIC20_STD_DISPLAY_HEIGHT (312)
*/

#define _VIC20_DBG_DISPLAY_WIDTH ((_M6561_HTOTAL+1)*4)
#define _VIC20_DBG_DISPLAY_HEIGHT (_M6561_VTOTAL+1)
#define _VIC20_DISPLAY_SIZE (_VIC20_DBG_DISPLAY_WIDTH*_VIC20_DBG_DISPLAY_HEIGHT*4)

static uint16_t _vic20_vic_fetch(uint16_t addr, void* user_data);
static void _vic20_init_key_map(vic20_t* sys);

#define _VIC20_DEFAULT(val,def) (((val) != 0) ? (val) : (def));
#define _VIC20_CLEAR(val) memset(&val, 0, sizeof(val))

void vic20_init(vic20_t* sys, const vic20_desc_t* desc) {
    CHIPS_ASSERT(sys && desc);
    CHIPS_ASSERT(!desc->pixel_buffer || (desc->pixel_buffer_size >= _VIC20_DISPLAY_SIZE));

    memset(sys, 0, sizeof(vic20_t));
    sys->valid = true;
    sys->joystick_type = desc->joystick_type;
    sys->mem_config = desc->mem_config;
    sys->via1_joy_mask = M6522_PA2|M6522_PA3|M6522_PA4|M6522_PA5;
    sys->via2_joy_mask = M6522_PB7;

    CHIPS_ASSERT(desc->rom_char && (desc->rom_char_size == sizeof(sys->rom_char)));
    CHIPS_ASSERT(desc->rom_basic && (desc->rom_basic_size == sizeof(sys->rom_basic)));
    CHIPS_ASSERT(desc->rom_kernal && (desc->rom_kernal_size == sizeof(sys->rom_kernal)));
    memcpy(sys->rom_char, desc->rom_char, sizeof(sys->rom_char));
    memcpy(sys->rom_basic, desc->rom_basic, sizeof(sys->rom_basic));
    memcpy(sys->rom_kernal, desc->rom_kernal, sizeof(sys->rom_kernal));
    sys->user_data = desc->user_data;
    sys->audio_cb = desc->audio_cb;
    sys->sample_buffer = desc->audio_buffer;
    sys->num_samples = desc->audio_num_samples;

    /* datasette: motor off, no buttons pressed */
    sys->cas_port = VIC20_CASPORT_MOTOR|VIC20_CASPORT_SENSE;

    m6502_desc_t cpu_desc;
    _VIC20_CLEAR(cpu_desc);
    sys->pins = m6502_init(&sys->cpu, &cpu_desc);

    m6522_init(&sys->via_1);
    m6522_init(&sys->via_2);

    m6561_desc_t vic_desc;
    _VIC20_CLEAR(vic_desc);
    vic_desc.fetch_cb = _vic20_vic_fetch;
    vic_desc.rgba8_buffer = (uint32_t*) desc->pixel_buffer;
    vic_desc.rgba8_buffer_size = desc->pixel_buffer_size;
    vic_desc.vis_x = _VIC20_DISPLAY_X;
    vic_desc.vis_y = _VIC20_DISPLAY_Y;
    vic_desc.vis_w = _VIC20_STD_DISPLAY_WIDTH;
    vic_desc.vis_h = _VIC20_STD_DISPLAY_HEIGHT;
    vic_desc.end_frame_cb = desc->end_frame_cb;
    vic_desc.user_data = sys;
    vic_desc.tick_hz = VIC20_FREQUENCY;
    vic_desc.sound_hz = _VIC20_DEFAULT(desc->audio_sample_rate, 44100);
    vic_desc.sound_magnitude = _VIC20_DEFAULT(desc->audio_volume, 1.0f);
    m6561_init(&sys->vic, &vic_desc);

    _vic20_init_key_map(sys);
    
    /*
        VIC-20 CPU memory map:

        0000..03FF      zero-page, stack, system area
        [0400..0FFF]    3 KB Expansion RAM
        1000..1FFF      4 KB Main RAM (block 0)
        [2000..3FFF]    8 KB Expansion Block 1
        [4000..5FFF]    8 KB Expansion Block 2
        [6000..7FFF]    8 KB Expansion Block 3
        8000..8FFF      4 KB Character ROM
        9000..900F      VIC Registers
        9110..911F      VIA #1 Registers
        9120..912F      VIA #2 Registers
        9400..97FF      1Kx4 bit color ram (either at 9600 or 9400)
        [9800..9BFF]    1 KB I/O Expansion 2
        [9C00..9FFF]    1 KB I/O Expansion 3
        [A000..BFFF]    8 KB Expansion Block 5 (usually ROM cartridges)
        C000..DFFF      8 KB BASIC ROM
        E000..FFFF      8 KB KERNAL ROM

        NOTE: use mem layer 1 for the standard RAM/ROM, so that
        the higher-priority layer 0 can be used for ROM cartridges
    */
    mem_init(&sys->mem_cpu);
    mem_map_ram(&sys->mem_cpu, 1, 0x0000, 0x0400, sys->ram0);
    if (desc->mem_config == VIC20_MEMCONFIG_3K || desc->mem_config == VIC20_MEMCONFIG_MAX) {
        mem_map_ram(&sys->mem_cpu, 1, 0x0400, 0x0C00, sys->ram_3k);
    }
    mem_map_ram(&sys->mem_cpu, 1, 0x1000, 0x1000, sys->ram1);
    if (desc->mem_config >= VIC20_MEMCONFIG_8K) {
        mem_map_ram(&sys->mem_cpu, 1, 0x2000, 0x2000, sys->ram_exp[0]);
    }
    if (desc->mem_config >= VIC20_MEMCONFIG_16K) {
        mem_map_ram(&sys->mem_cpu, 1, 0x4000, 0x2000, sys->ram_exp[1]);
    }
    if (desc->mem_config >= VIC20_MEMCONFIG_24K) {
        mem_map_ram(&sys->mem_cpu, 1, 0x6000, 0x2000, sys->ram_exp[2]);
    }
    mem_map_rom(&sys->mem_cpu, 1, 0x8000, 0x1000, sys->rom_char);
    mem_map_ram(&sys->mem_cpu, 1, 0x9400, 0x0400, sys->color_ram);
    if (desc->mem_config >= VIC20_MEMCONFIG_32K) {
        mem_map_ram(&sys->mem_cpu, 1, 0xA000, 0x2000, sys->ram_exp[3]);
    }
    mem_map_rom(&sys->mem_cpu, 1, 0xC000, 0x2000, sys->rom_basic);
    mem_map_rom(&sys->mem_cpu, 1, 0xE000, 0x2000, sys->rom_kernal);

    /*
        VIC-I memory map:

        The VIC-I has 14 address bus bits VA0..VA13, for 16 KB of
        addressable memory. Bits VA0..VA12 are identical with the
        lower 13 CPU address bus pins, VA13 is the inverted BLK4
        address decoding bit.
    */
    mem_init(&sys->mem_vic);
    mem_map_rom(&sys->mem_vic, 0, 0x0000, 0x1000, sys->rom_char);       /* CPU: 8000..8FFF */
    // FIXME: can the VIC read the color RAM as data?
    //mem_map_rom(&sys->mem_vic, 0, 0x1400, 0x0400, sys->color_ram);      /* CPU: 9400..97FF */
    mem_map_rom(&sys->mem_vic, 0, 0x2000, 0x0400, sys->ram0);           /* CPU: 0000..03FF */
    if (desc->mem_config == VIC20_MEMCONFIG_MAX) {
        mem_map_rom(&sys->mem_vic, 0, 0x2400, 0x0C00, sys->ram_3k);     /* CPU: 0400..0FFF */
    }
    mem_map_rom(&sys->mem_vic, 0, 0x3000, 0x1000, sys->ram1);           /* CPU: 1000..1FFF */
    mem_map_rom(&sys->mem_vic, 0, 0x4000, 0x1000, sys->rom_char);       /* CPU: 8000..8FFF */

    /*
        A special memory mapping used to copy ROM cartridge PRG files
        into the VIC-20. Those PRG files may be merged from several files
        and have gaps in them. The data in those gaps must not
        scribble over the VIC-20's RAM, ROM or IO regions.
    */
    mem_init(&sys->mem_cart);
    mem_map_ram(&sys->mem_cart, 0, 0x2000, 0x2000, sys->ram_exp[0]);
    mem_map_ram(&sys->mem_cart, 0, 0x4000, 0x2000, sys->ram_exp[1]);
    mem_map_ram(&sys->mem_cart, 0, 0x6000, 0x2000, sys->ram_exp[2]);
    mem_map_ram(&sys->mem_cart, 0, 0xA000, 0x2000, sys->ram_exp[3]);

    /* optionally setup C1530 datasette drive */
    if (desc->c1530_enabled) {
        c1530_desc_t c1530_desc;
        _VIC20_CLEAR(c1530_desc);
        c1530_desc.cas_port = &sys->cas_port;
        c1530_init(&sys->c1530, &c1530_desc);
    }
}

void vic20_discard(vic20_t* sys) {
    CHIPS_ASSERT(sys && sys->valid);
    if (sys->c1530.valid) {
        c1530_discard(&sys->c1530);
    }
    sys->valid = false;
}

void vic20_reset(vic20_t* sys) {
    CHIPS_ASSERT(sys && sys->valid);
    sys->kbd_joy_mask = 0;
    sys->joy_joy_mask = 0;
    sys->via1_joy_mask = M6522_PA2|M6522_PA3|M6522_PA4|M6522_PA5;
    sys->via2_joy_mask = M6522_PB7;
    sys->pins |= M6502_RES;
    sys->cas_port = VIC20_CASPORT_MOTOR|VIC20_CASPORT_SENSE;
    m6522_reset(&sys->via_1);
    m6522_reset(&sys->via_2);
    m6561_reset(&sys->vic);
    if (sys->c1530.valid) {
        c1530_reset(&sys->c1530);
    }
}

static uint64_t _vic20_tick(vic20_t* sys, uint64_t pins) {

    /* tick the CPU */
    pins = m6502_tick(&sys->cpu, pins);

    /* the IRQ and NMI pins will be set by the VIAs each tick */
    pins &= ~(M6502_IRQ|M6502_NMI);

    /* VIC+VIAs address decoding and memory access */
    uint64_t vic_pins  = pins & M6502_PIN_MASK;
    uint64_t via1_pins = pins & M6502_PIN_MASK;
    uint64_t via2_pins = pins & M6502_PIN_MASK;
    if ((pins & 0xFC00) == 0x9000) {
        /* 9000..93FF: VIA and VIC IO area

            A4 high:    VIA-1
            A5 high:    VIA-2

            The VIC has no chip-select pin instead it's directly snooping
            the address bus for a specific pin state:

            A8,9,10,11,13 low, A12 high

            (FIXME: why is the VIC only access on address 9000 (A15=1, A14=0),
            and not on 5000 (A15=0, A14=1) and D000 (A15=1, A14=1)???)

            NOTE: Unlike a real VIC, the VIC emulation has a traditional
            chip-select pin.
        */
        if (M6561_SELECTED_ADDR(pins)) {
            vic_pins |= M6561_CS;
        }
        if (pins & M6502_A4) {
            via1_pins |= M6522_CS1;
        }
        if (pins & M6502_A5) {
            via2_pins |= M6522_CS1;
        }
    }
    else {
        /* regular memory access */
        const uint16_t addr = M6502_GET_ADDR(pins);
        if (pins & M6502_RW) {
            M6502_SET_DATA(pins, mem_rd(&sys->mem_cpu, addr));
        }
        else {
            mem_wr(&sys->mem_cpu, addr, M6502_GET_DATA(pins));
        }
    }

    /* tick VIA1

        VIA1 IRQ pin is connected to CPU NMI pin

        VIA1 Port A input:
            PA0:    Serial CLK (FIXME)
            PA1:    Serial DATA (FIXME)
            PA2:    in: JOY0 (up)
            PA3:    in: JOY1 (down)
            PA4:    in: JOY2 (left)
            PA5:    in: PEN/BTN (fire)
            PA6:    in: CASS SENSE
            PA7:    SERIAL ATN OUT (???)

            CA1:    in: RESTORE KEY
            CA2:    out: CASS MOTOR

        VIA1 Port B input:
            all connected to USR port

        NOTE: the IRQ/NMI mapping is reversed from the C64
    */
    {
        // FIXME: SERIAL PORT
        via1_pins |= sys->via1_joy_mask | (M6522_PA0|M6522_PA1|M6522_PA7);
        if (sys->cas_port & VIC20_CASPORT_SENSE) {
            via1_pins |= M6522_PA6;
        }
        if(sys->kbd.scanout_column_masks[8] & 1) {
            via1_pins |= M6522_CA1;  /* RESTORE key is pressed */
        }
        via1_pins = m6522_tick(&sys->via_1, via1_pins);
        if (via1_pins & M6522_CA2) {
            sys->cas_port |= VIC20_CASPORT_MOTOR;
        }
        else {
            sys->cas_port &= ~VIC20_CASPORT_MOTOR;
        }
        if (via1_pins & M6522_IRQ) {
            pins |= M6502_NMI;
        }
        if ((via1_pins & (M6522_CS1|M6522_RW)) == (M6522_CS1|M6522_RW)) {
            pins = M6502_COPY_DATA(pins, via1_pins);
        }
    }

    /* tick VIA2

        VIA2 IRQ pin is connected to CPU IRQ pin

        VIA2 Port A input:
            read keyboard matrix rows

            CA1: in: CASS READ

        VIA2 Port B input:
            PB7:    JOY3 (Right)

        VIA2 Port A output:
            ---(???)

        VIA2 Port B output:
            write keyboard matrix columns
            PB3 -> CASS WRITE (not implemented)
    */
    {
        uint8_t kbd_lines = ~kbd_scan_lines(&sys->kbd);
        M6522_SET_PA(via2_pins, kbd_lines);
        via2_pins |= sys->via2_joy_mask;
        if (sys->cas_port & VIC20_CASPORT_READ) {
            via2_pins |= M6522_CA1;
        }
        via2_pins = m6522_tick(&sys->via_2, via2_pins);
        uint8_t kbd_cols = ~M6522_GET_PB(via2_pins);
        kbd_set_active_columns(&sys->kbd, kbd_cols);
        if (via2_pins & M6522_IRQ) {
            pins |= M6502_IRQ;
        }
        if ((via2_pins & (M6522_CS1|M6522_RW)) == (M6522_CS1|M6522_RW)) {
            pins = M6502_COPY_DATA(pins, via2_pins);
        }
    }

    /* tick the VIC */
    {
        vic_pins = m6561_tick(&sys->vic, vic_pins);
        if ((vic_pins & (M6561_CS|M6561_RW)) == (M6561_CS|M6561_RW)) {
            pins = M6502_COPY_DATA(pins, vic_pins);
        }
        if (vic_pins & M6561_SAMPLE) {
            sys->sample_buffer[sys->sample_pos++] = sys->vic.sound.sample;
            if (sys->sample_pos == sys->num_samples) {
                if (sys->audio_cb) {
                    sys->audio_cb(sys->sample_buffer, sys->num_samples, sys->user_data);
                }
                sys->sample_pos = 0;
            }
        }
    }

    /* optionally tick the C1530 datassette */
    if (sys->c1530.valid) {
        c1530_tick(&sys->c1530);
    }

    // tick the TMS9928 on $A000 - $A001
    /*
    if((pins & 0xFFFE) == 0xA000) {
      // byte unused = (byte) EM_ASM_INT({ console.log('tms access'); }, 0 );
      int MODE = pins & 1;      
      int read = pins & M6502_RW;  // CSR
      
      if(read) {
         // read
         byte data;
         if(MODE == 0) data = tms9928_vram_read(&vdp);
         else          data = tms9928_register_read(&vdp);
         M6502_SET_DATA(pins, data);
         //if(MODE == 0) { byte unused = (byte) EM_ASM_INT({ console.log('tms vram read:', $0); }, data ); }
         //else          { byte unused = (byte) EM_ASM_INT({ console.log('tms reg read:', $0); }, data ); }         
      }
      else  {
         byte data = M6502_GET_DATA(pins);
         if(MODE == 0) tms9928_vram_write(&vdp, data);
         else          tms9928_register_write(&vdp, data);
         
         //if(MODE == 0) { byte unused = (byte) EM_ASM_INT({ console.log('tms vram write:', $0); }, data ); }
         //else          { byte unused = (byte) EM_ASM_INT({ console.log('tms reg write:', $0); }, data ); }         
      }   
    }
    */

    return pins;
}

void vic20_tick(vic20_t* sys) {
    sys->pins = _vic20_tick(sys, sys->pins);
}

void vic20_exec(vic20_t* sys, uint32_t micro_seconds) {
    CHIPS_ASSERT(sys && sys->valid);
    uint32_t num_ticks = clk_us_to_ticks(VIC20_FREQUENCY, micro_seconds);
    uint64_t pins = sys->pins;
    for (uint32_t ticks = 0; ticks < num_ticks; ticks++) {
        pins = _vic20_tick(sys, pins);
    }
    sys->pins = pins;
    kbd_update(&sys->kbd, micro_seconds);

    // TMS9928
    /*
    if(1) {
      if(micro_seconds > 10000) {
         for(int t=0;t<262;t++) {
            tms9928_drawline(&vdp);
         }
      }  
    }
    */
}

static uint16_t _vic20_vic_fetch(uint16_t addr, void* user_data) {
    vic20_t* sys = (vic20_t*) user_data;
    uint16_t data = (sys->color_ram[addr & 0x03FF]<<8) | mem_rd(&sys->mem_vic, addr);
    return data;
}

static void _vic20_init_key_map(vic20_t* sys) {
    kbd_init(&sys->kbd, 1);
    const char* keymap =
    /* no shift */
    //   01234567 (col)
        "1     Q2"  // row 0
        "3WA ZSE4"  // row 1
        "5RDXCFT6"  // row 2
        "7YGVBHU8"  // row 3
        "9IJNMKO0"  // row 4
        "+PL,.:@-"  // row 5
        "~*;/ =^ "  // row 6, ~ is british pound
        "        "  // row 7

        /* shift */
        "!     q\""
        "#wa zse$"
        "%rdxcft&"
        "'ygvbhu("
        ")ijnmko "
        " pl<>[  "
        "$ ]?    "
        "        ";
    CHIPS_ASSERT(strlen(keymap) == 128);
    /* shift is column 3, line 1 */
    kbd_register_modifier(&sys->kbd, 0, 3, 1);
    /* ctrl is column 2, line 0 */
    kbd_register_modifier(&sys->kbd, 1, 2, 0);
    for (int shift = 0; shift < 2; shift++) {
        for (int column = 0; column < 8; column++) {
            for (int line = 0; line < 8; line++) {
                int c = keymap[shift*64 + line*8 + column];
                if (c != 0x20) {
                    kbd_register_key(&sys->kbd, c, column, line, shift?(1<<0):0);
                }
            }
        }
    }

    /* special keys */
    kbd_register_key(&sys->kbd, 0x20, 4, 0, 0);    /* space */
    kbd_register_key(&sys->kbd, 0x08, 2, 7, 1);    /* cursor left */
    kbd_register_key(&sys->kbd, 0x09, 2, 7, 0);    /* cursor right */
    kbd_register_key(&sys->kbd, 0x0A, 3, 7, 0);    /* cursor down */
    kbd_register_key(&sys->kbd, 0x0B, 3, 7, 1);    /* cursor up */
    kbd_register_key(&sys->kbd, 0x07, 0, 7, 1);    /* inst */
    kbd_register_key(&sys->kbd, 0x01, 0, 7, 0);    /* del */
    kbd_register_key(&sys->kbd, 0x0D, 1, 7, 0);    /* return */
    kbd_register_key(&sys->kbd, 0x03, 3, 0, 0);    /* stop */
    kbd_register_key(&sys->kbd, 0x04, 1, 0, 0);    /* left arrow */
    kbd_register_key(&sys->kbd, 0x05, 7, 6, 0);    /* home */
    kbd_register_key(&sys->kbd, 0x06, 7, 6, 1);    /* clr */
    kbd_register_key(&sys->kbd, 0x0E, 2, 0, 0);    /* ctrl */
    kbd_register_key(&sys->kbd, 0x0F, 5, 0, 0);    /* C= key */
    kbd_register_key(&sys->kbd, 0xF1, 4, 7, 0);
    kbd_register_key(&sys->kbd, 0xF2, 4, 7, 1);
    kbd_register_key(&sys->kbd, 0xF3, 5, 7, 0);
    kbd_register_key(&sys->kbd, 0xF4, 5, 7, 1);
    kbd_register_key(&sys->kbd, 0xF5, 6, 7, 0);
    kbd_register_key(&sys->kbd, 0xF6, 6, 7, 1);
    kbd_register_key(&sys->kbd, 0xF7, 7, 7, 0);
    kbd_register_key(&sys->kbd, 0xF8, 7, 7, 1);

    kbd_register_key(&sys->kbd, 0xFF, 0, 8, 0);   /* restore key is mapped on a separate line outside  */
                                                  /* the keyboard matrix and routed to VIA1 pin CA1 */
}

void mem_write_word(vic20_t* sys, uint16_t address, uint16_t value) {
	mem_wr(&sys->mem_cpu, address,   (value>>0) & 0xFF);
	mem_wr(&sys->mem_cpu, address+1, (value>>8) & 0xFF);
}

bool vic20_quickload(vic20_t* sys, const uint8_t* ptr, int num_bytes) {
    CHIPS_ASSERT(sys && sys->valid && ptr && (num_bytes > 0));
    if (num_bytes < 2) {
        return false;
    }
    const uint16_t start_addr = ptr[1]<<8 | ptr[0];
    ptr += 2;
    const uint16_t end_addr = start_addr + (num_bytes - 2);
    uint16_t addr = start_addr;
    while (addr < end_addr) {
        mem_wr(&sys->mem_cpu, addr++, *ptr++);
    }
    
    // update the BASIC pointers
	mem_write_word(sys, 0x2d, end_addr);	
	mem_write_word(sys, 0x2f, end_addr);	
	mem_write_word(sys, 0x31, end_addr);	
	mem_write_word(sys, 0x33, end_addr);	
	mem_write_word(sys, 0xae, end_addr);
    
    return true;
}

bool vic20_insert_rom_cartridge(vic20_t* sys, const uint8_t* ptr, int num_bytes) {
    CHIPS_ASSERT(sys && sys->valid && ptr && (num_bytes > 0));
    if (num_bytes < 2) {
        return false;
    }

    /* the cartridge ROM may be a special merged PRG with a gap between the
       two memory regions with valid data, we cannot scribble over memory
       in that gap, so use a temporary memory mapping
    */
    const uint16_t start_addr = ptr[1]<<8 | ptr[0];
    ptr += 2;
    const uint16_t end_addr = start_addr + (num_bytes - 2);
    uint16_t addr = start_addr;
    while (addr < end_addr) {
        mem_wr(&sys->mem_cart, addr++, *ptr++);
    }

    /* map the ROM cartridge into the CPU's memory layer 0 */
    mem_unmap_layer(&sys->mem_cpu, 0);
    if (start_addr == 0x2000) {
        mem_map_rom(&sys->mem_cpu, 0, 0x2000, 0x2000, sys->ram_exp[0]);
    }
    else if (start_addr == 0x4000) {
        mem_map_rom(&sys->mem_cpu, 0, 0x4000, 0x2000, sys->ram_exp[1]);
    }
    else if (start_addr == 0x6000) {
        mem_map_rom(&sys->mem_cpu, 0, 0x6000, 0x2000, sys->ram_exp[2]);
    }
    mem_map_rom(&sys->mem_cpu, 0, 0xA000, 0x2000, sys->ram_exp[3]);
    sys->pins |= M6502_RES;
    return true;
}

void vic20_remove_rom_cartridge(vic20_t* sys) {
    CHIPS_ASSERT(sys && sys->valid);
    mem_unmap_layer(&sys->mem_cpu, 0);
    sys->pins |= M6502_RES;
}

int vic20_std_display_width(void) {
    return _VIC20_STD_DISPLAY_WIDTH;
}

int vic20_std_display_height(void) {
    return _VIC20_STD_DISPLAY_HEIGHT;
}

int vic20_max_display_size(void) {
    return _VIC20_DISPLAY_SIZE;
}

int vic20_display_width(vic20_t* sys) {
    CHIPS_ASSERT(sys && sys->valid);
    return m6561_display_width(&sys->vic);
}

int vic20_display_height(vic20_t* sys) {
    CHIPS_ASSERT(sys && sys->valid);
    return m6561_display_height(&sys->vic);
}

/* generate precomputed VIA-1 and VIA-2 joystick port masks */
static void _vic20_update_joymasks(vic20_t* sys) {
    uint8_t jm = sys->kbd_joy_mask | sys->joy_joy_mask;
    sys->via1_joy_mask = M6522_PA2|M6522_PA3|M6522_PA4|M6522_PA5;
    sys->via2_joy_mask = M6522_PB7;
    if (jm & VIC20_JOYSTICK_UP) {
        sys->via1_joy_mask &= ~M6522_PA2;
    }
    if (jm & VIC20_JOYSTICK_DOWN) {
        sys->via1_joy_mask &= ~M6522_PA3;
    }
    if (jm & VIC20_JOYSTICK_LEFT) {
        sys->via1_joy_mask &= ~M6522_PA4;
    }
    if (jm & VIC20_JOYSTICK_BTN) {
        sys->via1_joy_mask &= ~M6522_PA5;
    }
    if (jm & VIC20_JOYSTICK_RIGHT) {
        sys->via2_joy_mask &= ~M6522_PB7;
    }
}

void vic20_key_down(vic20_t* sys, int key_code) {
    CHIPS_ASSERT(sys && sys->valid);
    if (sys->joystick_type == VIC20_JOYSTICKTYPE_NONE) {
        kbd_key_down(&sys->kbd, key_code);
    }
    else {
        uint8_t m = 0;
        switch (key_code) {
            case 0x20: m = VIC20_JOYSTICK_BTN; break;
            case 0x08: m = VIC20_JOYSTICK_LEFT; break;
            case 0x09: m = VIC20_JOYSTICK_RIGHT; break;
            case 0x0A: m = VIC20_JOYSTICK_DOWN; break;
            case 0x0B: m = VIC20_JOYSTICK_UP; break;
            default: kbd_key_down(&sys->kbd, key_code); break;
        }
        if (m != 0) {
            if (sys->joystick_type == VIC20_JOYSTICKTYPE_DIGITAL) {
                sys->kbd_joy_mask |= m;
                _vic20_update_joymasks(sys);
            }
        }
    }
}

void vic20_key_up(vic20_t* sys, int key_code) {
    CHIPS_ASSERT(sys && sys->valid);
    if (sys->joystick_type == VIC20_JOYSTICKTYPE_NONE) {
        kbd_key_up(&sys->kbd, key_code);
    }
    else {
        uint8_t m = 0;
        switch (key_code) {
            case 0x20: m = VIC20_JOYSTICK_BTN; break;
            case 0x08: m = VIC20_JOYSTICK_LEFT; break;
            case 0x09: m = VIC20_JOYSTICK_RIGHT; break;
            case 0x0A: m = VIC20_JOYSTICK_DOWN; break;
            case 0x0B: m = VIC20_JOYSTICK_UP; break;
            default: kbd_key_up(&sys->kbd, key_code); break;
        }
        if (m != 0) {
            if (sys->joystick_type == VIC20_JOYSTICKTYPE_DIGITAL) {
                sys->kbd_joy_mask &= ~m;
                _vic20_update_joymasks(sys);
            }
        }
    }
}

void vic20_set_joystick_type(vic20_t* sys, vic20_joystick_type_t type) {
    CHIPS_ASSERT(sys && sys->valid);
    sys->joystick_type = type;
}

vic20_joystick_type_t vic20_joystick_type(vic20_t* sys) {
    CHIPS_ASSERT(sys && sys->valid);
    return sys->joystick_type;
}

void vic20_joystick(vic20_t* sys, uint8_t joy_mask) {
    CHIPS_ASSERT(sys && sys->valid);
    sys->joy_joy_mask = joy_mask;
    _vic20_update_joymasks(sys);
}

bool vic20_insert_tape(vic20_t* sys, const uint8_t* ptr, int num_bytes) {
    CHIPS_ASSERT(sys && sys->valid && sys->c1530.valid);
    //return c1530_insert_tape(&sys->c1530, ptr, num_bytes);
    return false;
}

void vic20_remove_tape(vic20_t* sys) {
    CHIPS_ASSERT(sys && sys->valid && sys->c1530.valid);
    c1530_remove_tape(&sys->c1530);
}

bool vic20_tape_inserted(vic20_t* sys) {
    CHIPS_ASSERT(sys && sys->valid && sys->c1530.valid);
    return c1530_tape_inserted(&sys->c1530);
}

void vic20_tape_play(vic20_t* sys) {
    CHIPS_ASSERT(sys && sys->valid && sys->c1530.valid);
    c1530_play(&sys->c1530);
}

void vic20_tape_stop(vic20_t* sys) {
    CHIPS_ASSERT(sys && sys->valid && sys->c1530.valid);
    c1530_stop(&sys->c1530);
}

bool vic20_is_tape_motor_on(vic20_t* sys) {
    CHIPS_ASSERT(sys && sys->valid && sys->c1530.valid);
    return c1530_is_motor_on(&sys->c1530);
}

#endif /* CHIPS_IMPL */
