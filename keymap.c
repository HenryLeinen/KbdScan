#include <linux/kernel.h>
#include "keymap.h"



#define MAKE_KEY(r,c) (((r)<<4) | (c)) 
#define KEY_CLR 		MAKE_KEY(0,0)
#define KEY_Y 			MAKE_KEY(0,1)
#define KEY_T 			MAKE_KEY(0,2)
#define KEY_O 			MAKE_KEY(0,3)
#define KEY_J 			MAKE_KEY(0,4)
#define KEY_E 			MAKE_KEY(0,5)
#define KEY_DIM 		MAKE_KEY(0,6)
#define KEY_SIDE_RIGHT_0 	MAKE_KEY(0,7)
#define KEY_BRT 		MAKE_KEY(0,8)

#define KEY_OVFY 		MAKE_KEY(1,0)
#define KEY_X 			MAKE_KEY(1,1)
#define KEY_S 			MAKE_KEY(1,2)
#define KEY_N 			MAKE_KEY(1,3)
#define KEY_I 			MAKE_KEY(1,4)
#define KEY_D 			MAKE_KEY(1,5)
#define KEY_MCDU_MENU 		MAKE_KEY(1,6)
#define KEY_SIDE_RIGHT_1 	MAKE_KEY(1,7)
#define KEY_BLANK 		MAKE_KEY(1,8)

#define KEY_SP 			MAKE_KEY(2,0)
#define KEY_W 			MAKE_KEY(2,1)
#define KEY_R 			MAKE_KEY(2,2)
#define KEY_M 			MAKE_KEY(2,3)
#define KEY_H 			MAKE_KEY(2,4)
#define KEY_C			MAKE_KEY(2,5)
#define KEY_ATC_COMM 		MAKE_KEY(2,6)
#define KEY_SIDE_RIGHT_2 	MAKE_KEY(2,7)
#define KEY_DATA 		MAKE_KEY(2,8)

#define KEY_SLASH 		MAKE_KEY(3,0)
#define KEY_V 			MAKE_KEY(3,1)
#define KEY_Q 			MAKE_KEY(3,2)
#define KEY_L 			MAKE_KEY(3,3)
#define KEY_G 			MAKE_KEY(3,4)
#define KEY_B 			MAKE_KEY(3,5)
#define KEY_SEC_F_PLAN 		MAKE_KEY(3,6)
#define KEY_SIDE_RIGHT_3 	MAKE_KEY(3,7)
#define KEY_INIT 		MAKE_KEY(3,8)

#define KEY_Z 			MAKE_KEY(4,0)
#define KEY_U 			MAKE_KEY(4,1)
#define KEY_P 			MAKE_KEY(4,2)
#define KEY_K 			MAKE_KEY(4,3)
#define KEY_F 			MAKE_KEY(4,4)
#define KEY_A 			MAKE_KEY(4,5)
#define KEY_FUEL_PRED 		MAKE_KEY(4,6)
#define KEY_SIDE_RIGHT_4 	MAKE_KEY(4,7)
#define KEY_PERF 		MAKE_KEY(4,8)

#define KEY_PLUS_MINUS 		MAKE_KEY(5,0)
#define KEY_9 			MAKE_KEY(5,1)
#define KEY_6 			MAKE_KEY(5,2)
#define KEY_3 			MAKE_KEY(5,3)

#define KEY_0 			MAKE_KEY(6,0)
#define KEY_8 			MAKE_KEY(6,1)
#define KEY_5 			MAKE_KEY(6,2)
#define KEY_2 			MAKE_KEY(6,3)
#define KEY_ARROW_DOWN 		MAKE_KEY(6,4)
#define KEY_ARROW_UP 		MAKE_KEY(6,5)
#define KEY_RAD_NAV 		MAKE_KEY(6,6)
#define KEY_SIDE_RIGHT_5 	MAKE_KEY(6,7)
#define KEY_PROG 		MAKE_KEY(6,8)

#define KEY_STAR		MAKE_KEY(7,0)
#define KEY_7 			MAKE_KEY(7,1)
#define KEY_4 			MAKE_KEY(7,2)
#define KEY_1 			MAKE_KEY(7,3)
#define KEY_NEXT_PAGE 		MAKE_KEY(7,4)
#define KEY_AIRPORT 		MAKE_KEY(7,5)
#define KEY_F_PLN 		MAKE_KEY(7,6)
#define KEY_DIR 		MAKE_KEY(7,8)

#define KEY_SIDE_LEFT_5 	MAKE_KEY(8,0)
#define KEY_SIDE_LEFT_4 	MAKE_KEY(8,1)
#define KEY_SIDE_LEFT_3 	MAKE_KEY(8,2)
#define KEY_SIDE_LEFT_2 	MAKE_KEY(8,3)
#define KEY_SIDE_LEFT_1 	MAKE_KEY(8,4)
#define KEY_SIDE_LEFT_0 	MAKE_KEY(8,5)

#define KEY_NONE 		255

typedef	struct {
	uint8_t 	key;
	const char*	keyString;
	char		readable_char;
} KeyMap_t;
KeyMap_t KeyMap[255];

#define MAP_KEY(key, strg, ch)		KeyMap[KEY_##key].keyString=strg; KeyMap[KEY_##key].readable_char=ch


void init_key_map(void) {
	MAP_KEY(CLR, 		"CLR", 		'#');
	MAP_KEY(OVFY, 		"OVFY",		'#');
	MAP_KEY(SP,		" ", 		' ');
	MAP_KEY(SLASH,		"/",		'/');
	MAP_KEY(PLUS_MINUS,	"+/-",		'#');
	MAP_KEY(DIM,		"DIM",		'#');
	MAP_KEY(BRT,		"BRT",		'#');
	MAP_KEY(MCDU_MENU,	"MCDU MENU",	'#');
	MAP_KEY(BLANK,		"BLANK",	'#');
	MAP_KEY(ATC_COMM,	"ATC COMM",	'#');
	MAP_KEY(DATA,		"DATA",		'#');
	MAP_KEY(INIT,		"INIT",		'#');
	MAP_KEY(SEC_F_PLAN,	"SEC-F-PLN",	'#');
	MAP_KEY(PERF,		"PERF",		'#');
	MAP_KEY(FUEL_PRED,	"FUEL PRED",	'#');
	MAP_KEY(PROG,		"PROG",		'#');
	MAP_KEY(RAD_NAV,	"RAD NAV",	'#');
	MAP_KEY(DIR,		"DIR",		'#');
	MAP_KEY(F_PLN,		"F-PLN",	'#');
	MAP_KEY(AIRPORT,	"AIRPORT",	'#');
	MAP_KEY(NEXT_PAGE,	"NEXT_PAGE",	'#');
	MAP_KEY(ARROW_UP,	"ARROW UP",	'#');
	MAP_KEY(ARROW_DOWN,	"ARROW DOWN",	'#');
	MAP_KEY(STAR,		"*",		'*');
	MAP_KEY(0,		"0",		'0');
	MAP_KEY(1,		"1",		'1');
	MAP_KEY(2,		"2",		'2');
	MAP_KEY(3,		"3",		'3');
	MAP_KEY(4,		"4",		'4');
	MAP_KEY(5,		"5",		'5');
	MAP_KEY(6,		"6",		'6');
	MAP_KEY(7,		"7",		'7');
	MAP_KEY(8,		"8",		'8');
	MAP_KEY(9,		"9",		'9');
	MAP_KEY(A,		"A",		'A');
	MAP_KEY(B,		"B",		'B');
	MAP_KEY(C,		"C",		'C');
	MAP_KEY(D,		"D",		'D');
	MAP_KEY(E,		"E",		'E');
	MAP_KEY(F,		"F",		'F');
	MAP_KEY(G,		"G",		'G');
	MAP_KEY(H,		"H",		'H');
	MAP_KEY(I,		"I",		'I');
	MAP_KEY(J,		"J",		'J');
	MAP_KEY(K,		"K",		'K');
	MAP_KEY(L,		"L",		'L');
	MAP_KEY(M,		"M",		'M');
	MAP_KEY(N,		"N",		'N');
	MAP_KEY(O,		"O",		'O');
	MAP_KEY(P,		"P",		'P');
	MAP_KEY(Q,		"Q",		'Q');
	MAP_KEY(R,		"R",		'R');
	MAP_KEY(S,		"S",		'S');
	MAP_KEY(T,		"T",		'T');
	MAP_KEY(U,		"U",		'U');
	MAP_KEY(V,		"V",		'V');
	MAP_KEY(W,		"W",		'W');
	MAP_KEY(X,		"X",		'X');
	MAP_KEY(Y,		"Y",		'Y');
	MAP_KEY(Z,		"Z",		'Z');
	MAP_KEY(SIDE_LEFT_0,	"L0",		'#');
	MAP_KEY(SIDE_LEFT_1,	"L1",		'#');
	MAP_KEY(SIDE_LEFT_2,	"L2",		'#');
	MAP_KEY(SIDE_LEFT_3,	"L3",		'#');
	MAP_KEY(SIDE_LEFT_4,	"L4",		'#');
	MAP_KEY(SIDE_LEFT_5,	"L5",		'#');
	MAP_KEY(SIDE_RIGHT_0,	"R0",		'#');
	MAP_KEY(SIDE_RIGHT_1,	"R1",		'#');
	MAP_KEY(SIDE_RIGHT_2,	"R2",		'#');
	MAP_KEY(SIDE_RIGHT_3,	"R3",		'#');
	MAP_KEY(SIDE_RIGHT_4,	"R4",		'#');
	MAP_KEY(SIDE_RIGHT_5,	"R5",		'#');
}

char get_key_map_readable_char (unsigned char keycode) {
	return KeyMap[keycode].readable_char;
}

const char *get_key_map_key_string (unsigned char keycode) {
	return KeyMap[keycode].keyString;
}
