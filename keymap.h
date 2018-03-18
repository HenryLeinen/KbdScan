#ifndef __keymap_h__
#define __keymap_h__


extern void init_key_map(void);
extern char get_key_map_readable_char (unsigned char keycode);
extern const char *get_key_map_key_string (unsigned char keycode);


#endif
