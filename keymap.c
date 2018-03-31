#include <linux/kernel.h>
#include "keymap.h"



#define MAKE_KEY(r,c) ((((r+1))<<4) | ((c)+1))
#define KEY_CLR 		MAKE_KEY(0,0)	// CLR
#define KEY_Y 			MAKE_KEY(0,1)	// Y
#define KEY_T 			MAKE_KEY(0,2)	// T
#define KEY_O 			MAKE_KEY(0,3)	// O
#define KEY_J 			MAKE_KEY(0,4)	// J
#define KEY_E 			MAKE_KEY(0,5)	// E
#define KEY_DIM 		MAKE_KEY(0,6)	// DIM
#define KEY_BRT		 	MAKE_KEY(0,7)	// BRT
#define KEY_LSK1_R 		MAKE_KEY(0,8)	// LSK1-R

#define KEY_OVFY 		MAKE_KEY(1,0)	// OVFY
#define KEY_X 			MAKE_KEY(1,1)	// X
#define KEY_S 			MAKE_KEY(1,2)	// S
#define KEY_N 			MAKE_KEY(1,3)	// N
#define KEY_I 			MAKE_KEY(1,4)	// I
#define KEY_D 			MAKE_KEY(1,5)	// D
#define KEY_MCDU_MENU 		MAKE_KEY(1,6)	// MCDU_MENU
#define KEY_BLANK	 	MAKE_KEY(1,7)	// BLANK
#define KEY_LSK2_R		MAKE_KEY(1,8)	// LSK2-R

#define KEY_SP 			MAKE_KEY(2,0)	// SP
#define KEY_W 			MAKE_KEY(2,1)	// W
#define KEY_R 			MAKE_KEY(2,2)	// R
#define KEY_M 			MAKE_KEY(2,3)	// M
#define KEY_H 			MAKE_KEY(2,4)	// H
#define KEY_C			MAKE_KEY(2,5)	// C
#define KEY_ATC_COMM 		MAKE_KEY(2,6)	// ATC_COMM
#define KEY_DATA	 	MAKE_KEY(2,7)	// DATA
#define KEY_LSK3_R 		MAKE_KEY(2,8)	// LSK3-R

#define KEY_SLASH 		MAKE_KEY(3,0)	// SLASH
#define KEY_V 			MAKE_KEY(3,1)	// V
#define KEY_Q 			MAKE_KEY(3,2)	// Q
#define KEY_L 			MAKE_KEY(3,3)	// L
#define KEY_G 			MAKE_KEY(3,4)	// G
#define KEY_B 			MAKE_KEY(3,5)	// B
#define KEY_SEC_F_PLAN 		MAKE_KEY(3,6)	// SEC_F_PLAN
#define KEY_INIT	 	MAKE_KEY(3,7)	// INIT
#define KEY_LSK4_R 		MAKE_KEY(3,8)	// LSK4-R

#define KEY_Z 			MAKE_KEY(4,0)	// Z
#define KEY_U 			MAKE_KEY(4,1)	// U
#define KEY_P 			MAKE_KEY(4,2)	// P
#define KEY_K 			MAKE_KEY(4,3)	// K
#define KEY_F 			MAKE_KEY(4,4)	// F
#define KEY_A 			MAKE_KEY(4,5)	// A
#define KEY_FUEL_PRED 		MAKE_KEY(4,6)	// FUEL_PRED
#define KEY_PERF	 	MAKE_KEY(4,7)	// PERF
#define KEY_LSK5_R 		MAKE_KEY(4,8)	// LSK5-R

#define KEY_PLUS_MINUS 		MAKE_KEY(5,0)	// PLUS_MINUS
#define KEY_9 			MAKE_KEY(5,1)	// 9
#define KEY_6 			MAKE_KEY(5,2)	// 8
#define KEY_3 			MAKE_KEY(5,3)	// 7

#define KEY_0 			MAKE_KEY(6,0)	// 0
#define KEY_8 			MAKE_KEY(6,1)	// 8
#define KEY_5 			MAKE_KEY(6,2)	// 5
#define KEY_2 			MAKE_KEY(6,3)	// 2
#define KEY_PAGE_DOWN 		MAKE_KEY(6,4)	// PAGE_DOWN
#define KEY_PAGE_UP 		MAKE_KEY(6,5)	// PAGE_UP
#define KEY_RAD_NAV 		MAKE_KEY(6,6)	// RAD_NAV
#define KEY_PROG	 	MAKE_KEY(6,7)	// PROG
#define KEY_LSK6_R 		MAKE_KEY(6,8)	// LSK6_R

#define KEY_LSK6_L		MAKE_KEY(7,0)	// LSK6_L
#define KEY_LSK5_L		MAKE_KEY(7,1)	// LSK5_L
#define KEY_LSK4_L		MAKE_KEY(7,2)	// LSK4_L
#define KEY_LSK3_L		MAKE_KEY(7,3)	// LSK3_L
#define KEY_LSK2_L 		MAKE_KEY(7,4)	// LSK2_L
#define KEY_LSK1_L 		MAKE_KEY(7,5)	// LSK1_L
//#define KEY_F_PLN 		MAKE_KEY(7,6)
//#define KEY_DIR 		MAKE_KEY(7,8)

#define KEY_STAR	 	MAKE_KEY(8,0)	// STAR
#define KEY_7		 	MAKE_KEY(8,1)	// 7
#define KEY_4		 	MAKE_KEY(8,2)	// 4
#define KEY_1			MAKE_KEY(8,3)	// 1
#define KEY_NEXT_PAGE	 	MAKE_KEY(8,4)	// NEXT_PAGE
#define KEY_AIRPORT	 	MAKE_KEY(8,5)	// AIRPORT
#define KEY_F_PLN		MAKE_KEY(8,6)	// F_PLN
#define KEY_DIR			MAKE_KEY(8,7)	// DIR

#define KEY_NONE 		255

typedef	struct {
	uint8_t 	key;
	const char*	keyString;
	char		readable_char;
} KeyMap_t;
KeyMap_t KeyMap[255];

#define MAP_KEY(key, strg, ch)		KeyMap[KEY_##key].keyString=strg; KeyMap[KEY_##key].readable_char=ch


void init_key_map(void) {
	MAP_KEY(CLR, 		"CLR", 		20);
	MAP_KEY(OVFY, 		"OVFY",		21);
	MAP_KEY(SP,		" ", 		' ');
	MAP_KEY(SLASH,		"/",		'/');
	MAP_KEY(PLUS_MINUS,	"+/-",		'+');
	MAP_KEY(DIM,		"DIM",		116);
	MAP_KEY(BRT,		"BRT",		106);
	MAP_KEY(MCDU_MENU,	"MCDU MENU",	115);
	MAP_KEY(BLANK,		"BLANK",	105);
	MAP_KEY(ATC_COMM,	"ATC COMM",	114);
	MAP_KEY(DATA,		"DATA",		104);
	MAP_KEY(INIT,		"INIT",		103);
	MAP_KEY(SEC_F_PLAN,	"SEC-F-PLN",	113);
	MAP_KEY(PERF,		"PERF",		102);
	MAP_KEY(FUEL_PRED,	"FUEL PRED",	112);
	MAP_KEY(PROG,		"PROG",		101);
	MAP_KEY(RAD_NAV,	"RAD NAV",	111);
	MAP_KEY(DIR,		"DIR",		100);
	MAP_KEY(F_PLN,		"F-PLN",	110);
	MAP_KEY(AIRPORT,	"AIRPORT",	120);
	MAP_KEY(NEXT_PAGE,	"NEXT_PAGE",	130);
	MAP_KEY(PAGE_UP,	"PAGE UP",	121);
	MAP_KEY(PAGE_DOWN,	"PAGE DOWN",	131);
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
	MAP_KEY(LSK1_L,		"L0",		1);
	MAP_KEY(LSK2_L,		"L1",		2);
	MAP_KEY(LSK3_L,		"L2",		3);
	MAP_KEY(LSK4_L,		"L3",		4);
	MAP_KEY(LSK5_L,		"L4",		5);
	MAP_KEY(LSK6_L,		"L5",		6);
	MAP_KEY(LSK1_R,		"R0",		11);
	MAP_KEY(LSK2_R,		"R1",		12);
	MAP_KEY(LSK3_R,		"R2",		13);
	MAP_KEY(LSK4_R,		"R3",		14);
	MAP_KEY(LSK5_R,		"R4",		15);
	MAP_KEY(LSK6_R,		"R5",		16);
}

char get_key_map_readable_char (unsigned char keycode) {
	return KeyMap[keycode].readable_char;
}

const char *get_key_map_key_string (unsigned char keycode) {
	return KeyMap[keycode].keyString;
}
