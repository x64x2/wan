#define PAD_A			0x01
#define PAD_B			0x02
#define PAD_SELECT		0x04
#define PAD_START		0x08
#define PAD_UP			0x10
#define PAD_DOWN		0x20
#define PAD_LEFT		0x40
#define PAD_RIGHT		0x80

#define OAM_FLIP_V		0x80
#define OAM_FLIP_H		0x40
#define OAM_BEHIND		0x20

#define MAX(x1,x2)		((x1)<(x2)?(x2):(x1))
#define MIN(x1,x2)		((x1)<(x2)?(x1):(x2))

#define MASK_SPR		0x10
#define MASK_BG			0x08
#define MASK_EDGE_SPR	0x04
#define MASK_EDGE_BG	0x02

#define NULL			0
#define TRUE			1
#define FALSE			0

void __fastcall__ pal_all(const char *data);


void __fastcall__ pal_bg(const char *data);

void __fastcall__ pal_spr(const char *data);

void __fastcall__ pal_col(unsigned char index,unsigned char color);

void __fastcall__ pal_clear(void);

void __fastcall__ pal_bright(unsigned char bright);

void __fastcall__ ppu_off(void);

void __fastcall__ ppu_on_all(void);

void __fastcall__ ppu_on_bg(void);

void __fastcall__ ppu_on_spr(void);

void __fastcall__ ppu_mask(unsigned char mask);

unsigned char __fastcall__ ppu_system(void);

//clear OAM buffer, all the sprites are hidden

void __fastcall__ oam_clear(void);

//set sprites size, 0 for 8x8, 1 for 8x16

void __fastcall__ oam_size(unsigned char size);

unsigned char __fastcall__ oam_spr(unsigned char x,unsigned char y,unsigned char chrnum,unsigned char attr,unsigned char sprid);

//set metasprite in OAM buffer
unsigned char __fastcall__ oam_meta_spr(unsigned char x,unsigned char y,unsigned char sprid,const unsigned char *data);

void __fastcall__ oam_hide_rest(unsigned char sprid);

//wait NMI and sync to 50hz (with frameskip for NTSC)
void __fastcall__ ppu_waitnmi(void);

//play a music in FamiTone format
void __fastcall__ music_play(unsigned char song);

//stop music
void __fastcall__ music_stop(void);

void __fastcall__ music_pause(unsigned char pause);

void __fastcall__ sfx_play(unsigned char sound,unsigned char channel);

//poll controller and return flags like PAD_LEFT etc, input is pad number (0 or 1)

unsigned char __fastcall__ pad_poll(unsigned char pad);

unsigned char __fastcall__ pad_trigger(unsigned char pad);

unsigned char __fastcall__ pad_state(unsigned char pad);

void __fastcall__ scroll(unsigned int x,unsigned int y);

void __fastcall__ split(unsigned int x,unsigned int y);


//select current chr bank for sprites, 0..1

void __fastcall__ bank_spr(unsigned char n);

//select current chr bank for background, 0..1

void __fastcall__ bank_bg(unsigned char n);



//returns random number 0..255 or 0..65535

unsigned char __fastcall__ rand8(void);
unsigned int  __fastcall__ rand16(void);

//set random seed

void __fastcall__ set_rand(unsigned int seed);

void __fastcall__ set_vram_update(unsigned char len,unsigned char *buf);

void __fastcall__ vram_adr(unsigned int adr);

//put a byte at current vram address, works only when rendering is turned off

void __fastcall__ vram_put(unsigned char n);

//fill a block with a byte at current vram address, works only when rendering is turned off

void __fastcall__ vram_fill(unsigned char n,unsigned int len);

//set vram autoincrement, 0 for +1 and not 0 for +32

void __fastcall__ vram_inc(unsigned char n);

//read a block from vram, works only when rendering is turned off

void __fastcall__ vram_read(unsigned char *dst,unsigned int adr,unsigned int size);

//write a block to vram, works only when rendering is turned off

void __fastcall__ vram_write(unsigned char *src,unsigned int adr,unsigned int size);


//unpack a nametable into vram

void __fastcall__ unrle_vram(const unsigned char *data,unsigned int vram);



//like a normal memcpy, but does not return anything

void __fastcall__ memcpy(void *dst,void *src,unsigned int len);

//like memset, but does not return anything

void __fastcall__ memfill(void *dst,unsigned char value,unsigned int len);

void __fastcall__ delay(unsigned char frames);

// CNROM Bankswitch

void __fastcall__ bankswitch(unsigned char bank);

