#include "tms9928.c"

tms9928_t vdp;
tms9928_desc_t vdp_desc;

byte vram[16384];
uint32_t display_buffer[342*262];

byte int_NMI;

void tms9928_interrupt_called(uint8_t int_state) {
   int_NMI = int_state;
}

int cnt = 0;
void tms9928_screen_update_called(uint32_t *buffer) {
   cnt++;
   if((cnt & 1) == 1) {
      byte unused = (byte) EM_ASM_INT({ tms9928_screen_update($0); }, buffer );
   }
}

void vdp_init() {
   vdp_desc.isPal = false;
   vdp_desc.family99 = true;
   vdp_desc.reva = true;

   vdp_desc.vram_size = 16384;
   vdp_desc.vram_space = vram;
   //for(int t=0;t<16384;t++) vram[t] = t & 0xFF; // fill with pattern

   vdp_desc.display_buffer = display_buffer;

   vdp_desc.out_int_line_cb = &tms9928_interrupt_called;
   vdp_desc.screen_update_cb = &tms9928_screen_update_called;

   tms9928_init(&vdp, &vdp_desc);
}
