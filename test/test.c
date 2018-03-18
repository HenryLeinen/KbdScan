#include <stdio.h>


int main (int argc, char **argl) {
	FILE*	f;
	int	c;

	f = fopen("/dev/kbdscan", "r");
	if (f == NULL) {
		printf("Unable to open /dev/kbdscan device !\n");
		return -1;
	}

	while(1) {
		c = fgetc(f);
		if (c != -1)
			printf("N: %i\n", c);
	}

	fclose(f);
}
