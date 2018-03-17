#ifndef __mygpio_h__
#define __mygpio_h__

#ifndef TRUE
# define 	TRUE 	(1==1)
# define	FALSE	(1==0)
#endif 



//	Pin Modes
#define		INPUT			0
#define		OUTPUT			1

//	Pull up/down modes
#define		PUD_OFF			0
#define		PUD_DOWN		1
#define		PUD_UP			2


//	Pi model types and version numbers
#define		PI_MODEL_A		0
#define		PI_MODEL_B		1
#define		PI_MODEL_AP		2
#define		PI_MODEL_BP		3
#define		PI_MODEL_2		4
#define		PI_ALPHA		5
#define		PI_MODEL_CM		6
#define		PI_MODEL_07		7
#define		PI_MODEL_3		8
#define		PI_MODEL_ZERO		9
#define		PI_MODEL_CM3		10
#define		PI_MODEL_ZERO_W		12

#define		PI_VERSION_1		0
#define		PI_VERSION_1_1		1
#define		PI_VERSION_1_2		2
#define		PI_VERSION_2		3

#define		PI_MAKER_SONY		0
#define		PI_MAKER_EGOMAN		1
#define		PI_MAKER_EMBEST		2
#define		PI_MAKER_UNKNOWN	3

extern const char	*piModelNames[16];
extern const char	*piRevisionNames[16];
extern const char	*piMakerNames[16];
extern const int	 piMemorySize[8];


/* Function prototypes */
#ifdef __cplusplus
extern "C" {
#endif

extern int 	mygpio_setup			(void);
extern void 	mygpio_exit			(void);

extern void	mygpio_pin_mode			(int pin, int mode);
extern void	mygpio_pull_up_dn_control	(int pin, int pud);
extern void 	mygpio_digital_write		(int pin, int value);
extern int	mygpio_digital_read		(int pin);

#ifdef __cplusplus
}
#endif

#endif
