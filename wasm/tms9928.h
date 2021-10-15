#include <stdint.h>
#include <stdbool.h>

#ifndef TMS9928_H
#define TMS9928_H 1

#define	TMS9928_PALETTE_SIZE                16
#define TMS9928_TOTAL_HORZ                  342
#define	TMS9928_TOTAL_VERT_NTSC             262
#define	TMS9928_TOTAL_VERT_PAL              313
#define	TMS9928_HORZ_DISPLAY_START          2 + 14 + 8 + 13
#define	TMS9928_VERT_DISPLAY_START_PAL      13 + 51
#define	TMS9928_VERT_DISPLAY_START_NTSC     13 + 27

/* callbacks */
typedef void (*tms9928_out_int_line_cb_t)(uint8_t int_state);
typedef void (*tms9928_screen_update_cb_t)(uint32_t *bmp);

typedef struct {
   // TODO implement timers
	//TIMER_LINE = 0;
	//int GROMCLK = 1;

	int m_vram_size;                                // 4K, 8K, or 16K

	tms9928_out_int_line_cb_t m_out_int_line_cb;    // Callback is called whenever the state of the INT output changes
	void *m_out_gromclk_cb;                         // GROMCLK line is optional; if present, pulse it by XTAL/24 rate

    tms9928_screen_update_cb_t screen_update_cb;    // screen update callback

	/* TMS9928A internal settings */
	uint8_t  m_ReadAhead;
	uint8_t  m_Regs[8];
	uint8_t  m_StatusReg;
	uint8_t  m_FifthSprite;
	uint8_t  m_latch;
	uint8_t  m_INT;
	uint16_t m_Addr;
	uint16_t m_colour;
	uint16_t m_pattern;
	uint16_t m_nametbl;
	uint16_t m_spriteattribute;
	uint16_t m_spritepattern;
	int      m_colourmask;
	int      m_patternmask;
	uint16_t m_TMS9928_TOTAL_HORZ;
	bool     m_50hz;                      // 50Hz (PAL) or 60Hz (NTSC)
	bool     m_reva;                      // Rev.A of the chip
	bool     m_99;

   uint8_t  *m_vram_space;               // the actual vram

   uint32_t *m_tmpbmp;                   // drawing buffer of RGBA

   uint8_t m_mode;

   int m_top_border;
   int m_vertical_size;

   int vpos;                                 // vertical posizion of the raster

   uint32_t palette[TMS9928_PALETTE_SIZE];   // palette of colours (RGBA)

    void* user_data;
} tms9928_t;

/* initialization attributes */
typedef struct {

   bool isPal;
   bool family99;
   bool reva;

   int vram_size;
   uint8_t *vram_space;

   uint32_t *display_buffer;

   tms9928_out_int_line_cb_t  out_int_line_cb;
   tms9928_screen_update_cb_t screen_update_cb;

   void* user_data;            /* optional user data for tick callback */
} tms9928_desc_t;

void tms9928_check_interrupt(tms9928_t *vdp);
void tms9928_update_table_masks(tms9928_t *vdp);
void tms9928_update_backdrop(tms9928_t *vdp);

uint8_t tms9928_vram_read(tms9928_t *vdp);
uint8_t tms9928_register_read(tms9928_t *vdp);
void tms9928_vram_write(tms9928_t *vdp, uint8_t data);
void tms9928_register_write(tms9928_t *vdp, uint8_t data);
void tms9928_init(tms9928_t *vdp, tms9928_desc_t *desc);
void tms9928_drawline(tms9928_t *vdp);
void tms9928_reset();

#endif
