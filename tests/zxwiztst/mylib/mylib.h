void scr_init(char atribute);
#ifdef FASTPIXEL
void fast_set_pix_init(void);
void fast_set_pix(unsigned char x,unsigned char y);
#define set_pix fast_set_pix
#define set_pix_init fast_set_pix_init
#else
#define set_pix_init()
void little_set_pix(unsigned char x,unsigned char y);
#define set_pix little_set_pix
#endif
extern void my_im2_init(void *);

extern unsigned char * mylib_font8x8;