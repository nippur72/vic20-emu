// paste fake basic program
(function() {
   let c=0;
   let s="";

   for(let t=1;t<300;t++) {
      s+=`${t} A=A+${t}\r`;
      c+=t;
   }
   s+="9999 PRINT A\rRUN\r";
   pasteLine(s);
   console.log(c);
})();



// prova di lettura dal psg port B
(function() {
   let PORT_A_WR = 1<<6;
   let PORT_B_WR = 1<<7;
   let PORT_A_RD = 0;
   let PORT_B_RD = 0;
   let r;

   psg_init();
   psg_reset();

   psg_write(0x40, 7);
   console.log(psg_query_addr());

   psg_write(0x41, PORT_A_WR|PORT_B_RD);
   console.log(psg_query_reg(7));

   /*
   psg_write(0x40, 15);
   console.log(psg_query_addr());

   let x = psg_read(0x40);
   console.log(x);
   */

   psg_write(0x40, 14);
   console.log(psg_query_addr());

   psg_write(0x41, 77);
   console.log(psg_query_addr());
})();


// topaz font
for(let t=0;t<topaz.length/2;t++) rom[0x4383+t] = topaz[t*2];

// laser 500 fonts
(function() {
   function reverse(b) {
      b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
      b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
      b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
      return b;
   }
   for(let t=33*8;t<127*8;t++) rom[0x4383+t] = reverse(charset_laser500[(256*8)*4+t]);
})();



paste(`
0 PRINT CHR$(159.5+RND(1));:GOTO
`)

paste(`
90 print "wait..."
100 dim f(32)
110 for i=0 to 31
120 x=440*2^((i-12)/12)
140 f(i)=4096-(115206.1875/x)
150 next
160 cls
161 print "*** LM80C Piano ***"
162 print
163 print "by Antonino Porcino, 2020"
164 print:print
165 print "  2 3   5 6 7   9 0
166 print " Q W E R T Y U I O P
167 print
168 print "  S D   G H J
169 print " Z X C V B N M
174 volume 1,15
175 a$="zsxdcvgbhnjmq2w3er5t6y7ui9o0p"
180 x=inkey(10):if x=0 then 180
190 x=instr(a$,chr$(x))
200 if x=0 then 180
210 sound 1,f(x),50
220 goto 180
`)

(function (){
   debugBefore = ()=>
   {
      let state = cpu.getState();
      if(state.pc == 0x2982 && (state.a != 191 && state.a !=183)) console.log(`a=${state.a}`);
   }
})();

paste(`
10 volume 1,15
20 for t=4001 to 4095 step 2
25 print t
30 sound 1,t,20:pause 20
40 next
`)

paste(`
100 input x
110 f = 4096-(115206.1875/x)
120 print f
130 volume 1,15
140 sound 1,f,200
`)


paste(`
10 volume 1,15
20 sound 1,rnd(1)*4095,10
30 pause 10
40 goto 20
`)

paste(`
10 volume 1,15
20 sound 4,10:sound 4,0
30 if sstat(7)<>191 then print sstat(7)
40 goto 20
`)