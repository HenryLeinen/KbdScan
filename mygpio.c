#include <linux/fs.h>
#include <linux/ctype.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/delay.h>
#include "mygpio.h"

static volatile unsigned int	GPIO_BASE;

#define _BLOCK_SIZE	(4*1024)
#define _PAGE_SIZE	(4*1024)

/**
 * pointer to hardware registers
 */
static volatile uint32_t	*gpio = 0;
struct resource *iomem;



/**
 * Hardware register base addresses
 **/
#define	GPIO_PERI_BASE_OLD		0x20000000
#define GPIO_PERI_BASE_NEW		0x3F000000

static volatile unsigned int		piGpioBase;

/**
 * Array with model names
 **/
const char *piModelNames [16] =
{
  "Model A",	//  0
  "Model B",	//  1
  "Model A+",	//  2
  "Model B+",	//  3
  "Pi 2",	//  4
  "Alpha",	//  5
  "CM",		//  6
  "Unknown07",	// 07
  "Pi 3",	// 08
  "Pi Zero",	// 09
  "CM3",	// 10
  "Unknown11",	// 11
  "Pi Zero-W",	// 12
  "Unknown13",	// 13
  "Unknown14",	// 14
  "Unknown15",	// 15
};


// gpioToGPFSEL:
//	Map a BCM_GPIO pin to it's Function Selection
//	control port. (GPFSEL 0-5)
//	Groups of 10 - 3 bits per Function - 30 bits per port

static uint8_t gpioToGPFSEL [] = {
  0,0,0,0,0,0,0,0,0,0,
  1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,
  4,4,4,4,4,4,4,4,4,4,
  5,5,5,5,5,5,5,5,5,5,
};


// gpioToShift
//	Define the shift up for the 3 bits per pin in each GPFSEL port

static uint8_t gpioToShift [] = {
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
  0,3,6,9,12,15,18,21,24,27,
};


// gpioToGPSET:
//	(Word) offset to the GPIO Set registers for each GPIO pin

static uint8_t gpioToGPSET [] = {
   7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
};


// gpioToGPCLR:
//	(Word) offset to the GPIO Clear registers for each GPIO pin

static uint8_t gpioToGPCLR [] = {
  10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
  11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
};


// gpioToGPLEV:
//	(Word) offset to the GPIO Input level registers for each GPIO pin

static uint8_t gpioToGPLEV [] = {
  13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
  14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
};


/* GPIO Pin pull up/down register */
#define GPPUD	37


// gpioToPUDCLK
//	(Word) offset to the Pull Up Down Clock regsiter

static uint8_t gpioToPUDCLK [] = {
  38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,
  39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,
};



int readLine (struct file *cpuFd, char *line, int maxCount) {
	int c = 0;		//	Number of characters read

	do {
		if (cpuFd->f_op->read(cpuFd, &line[c], 1, &cpuFd->f_pos) <= 0)
			break;
		c++;
	} while ((c <maxCount) && (line[c-1] != '\n'));

	line[c] = 0;
	return c == 0 ? -1 : c;
}




/*
 * piBoardId:
 *	Return the real details of the board we have.
 *
 *********************************************************************************
 */

void piBoardId (int *model) {
	struct file *cpuFd ;
	mm_segment_t fs;
	char line [120] ;
	char *c ;
	unsigned long revision ;
	int bType;

	//	Will deal with the properly later on - for now, lets just get it going...
	if ((cpuFd = filp_open ("/proc/cpuinfo", O_RDONLY, 0)) == NULL) {
		printk (KERN_ALERT "Unable to open /proc/cpuinfo") ;
		return;
	}

	fs = get_fs();
	set_fs(get_ds());

	while (readLine (cpuFd, line, 120) > 0)
		if (strncmp (line, "Revision", 8) == 0)
			break ;

	set_fs(fs);
	filp_close (cpuFd, NULL) ;

	if (strncmp (line, "Revision", 8) != 0)
		printk (KERN_INFO "No \"Revision\" line") ;

	// Chomp trailing CR/NL
	for (c = &line [strlen (line) - 1] ; (*c == '\n') || (*c == '\r') ; --c)
		*c = 0 ;

	// Need to work out if it's using the new or old encoding scheme:
	// Scan to the first character of the revision number

	for (c = line ; *c ; ++c)
		if (*c == ':')
			break ;

	if (*c != ':')
		printk (KERN_INFO "Bogus \"Revision\" line (no colon)") ;

	// Chomp spaces
	++c ;
	while (isspace (*c))
		++c ;

	if (!isxdigit (*c))
		printk (KERN_INFO "Bogus \"Revision\" line (no hex digit at start of revision)") ;

	if (kstrtoul (c, 16, &revision)== 0) ; // Hex number with no leading 0x

	// Check for new way:
	if ((revision &  (1 << 23)) != 0) {	// New way 
		printk (KERN_INFO "piBoardId: New Way: revision is: 0x%08lX\n", revision) ;

		bType     = (revision & (0xFF <<  4)) >>  4 ;

		*model    = bType ;
	}
	else {					// Old way
		if (!isdigit (*c))
			printk (KERN_INFO "Bogus \"Revision\" line (no digit at start of revision)") ;

		// Make sure its long enough

		if (strlen (c) < 4)
			printk (KERN_INFO "Bogus \"Revision\" line (not long enough)") ;

		// If longer than 4, we'll assume it's been overvolted
		// Extract last 4 characters:
		c = c + strlen (c) - 4 ;
		// Fill out the replys as appropriate
		if ( (strncmp (c, "000", 3) == 0)) {	//	Can be MODEL_A or MODEL_B
			switch (c[3]) {
				case '2': case '3': case '4': case '5':	case '6': case 'd': case 'e': case 'f': 	*model = PI_MODEL_B;	break;
				case '7': case '8': case '9':								*model = PI_MODEL_A;	break;
				default:										*model = 0;		break;
			}
		} else if (strncmp (c, "001", 3) == 0) {	//	can be MODEL_BP, MODEL_AP or MODEL_CM
			switch (c[3]) {
				case '0': case '3': case '6': case '9':							*model = PI_MODEL_BP;	break;
				case '1': case '4': case '7': case 'a':							*model = PI_MODEL_CM;	break;
				case '2': case '5': case '8': case 'b':							*model = PI_MODEL_AP;	break;
				default:										*model = 0;		break;
			}
		} else *model = 0;
	}
}







void mygpio_pin_mode(int pin, int mode) {
	int fsel;
	int shift;

	if (pin > 64)
		return;	/* invalid pin reference */

	fsel = gpioToGPFSEL [pin];
	shift= gpioToShift  [pin];

	if (mode == INPUT) {
		writel(readl(gpio+fsel) & ~(7<<shift),gpio+fsel);
	} else if (mode == OUTPUT) {
		writel((readl(gpio+fsel) &  ~(7<<shift))|(1<<shift), gpio+fsel);
	}
}

void mygpio_pull_up_dn_control(int pin, int pud) {
	if (pin > 64)
		return; /* invalid pin reference */

	writel(pud&3, gpio+GPPUD);				udelay (5);
	writel(1<<(pin&31), gpio+gpioToPUDCLK[pin]);		udelay (5);

	writel(0, gpio+GPPUD);					udelay (5);
	writel(0, gpio+gpioToPUDCLK[pin]);			udelay (5);
}

int mygpio_digital_read(int pin) {
	if (pin > 64)
		return -1; /* invalid pin reference */
	if ((readl(gpio+gpioToGPLEV[pin]) & (1 << (pin&31))) != 0)
		return 1;
	else
		return 0;
}

void mygpio_digital_write(int pin, int value) {
	if (pin > 64)
		return; /* invalid pin reference */

	if (value ==0)
		writel(1<<(pin&31), gpio+gpioToGPCLR[pin]);
	else
		writel(1<<(pin&31), gpio+gpioToGPSET[pin]);
}




int mygpio_setup(void) {
	static bool initialized = FALSE;
	int model;

	/* only initialize once */
	if (initialized)
		return 0;

	initialized = TRUE;

	piBoardId(&model);

	switch (model) {
		case PI_MODEL_A:
		case PI_MODEL_B:
		case PI_MODEL_AP:
		case PI_MODEL_BP:
		case PI_ALPHA:
		case PI_MODEL_CM:
		case PI_MODEL_ZERO:
		case PI_MODEL_ZERO_W:
			piGpioBase = GPIO_PERI_BASE_OLD;
			break;

		default:
			piGpioBase = GPIO_PERI_BASE_NEW;
			break;
	}

	printk (KERN_INFO "The following hardware has been detected : model=%s\n", piModelNames[model]);

	/* set the offsets into memory interface */
	GPIO_BASE	= piGpioBase + 0x00200000;

	/* Map the hardware registers */
	iomem = request_mem_region(GPIO_BASE, 256, "mygpio");
	gpio = (uint32_t *)ioremap(GPIO_BASE, 256);
	return 0;
}

void mygpio_exit(void) {
	if (gpio) {
		iounmap(gpio);
		release_mem_region(GPIO_BASE, 256);
	}
	gpio = 0;
}
