/* Extern functions that gnuboy does not define in .h */
extern int rc_command(char*);
extern int loader_init();
extern void emu_reset();
extern void state_save();
extern void state_load();
extern void lcd_begin();
extern void pal_dirty();
extern void loader_unload();
