/*
 
 This program cracks an MD5 Hash where most of the characters 
 are given and the hash is known.
 
 Don't expect to much: It is only rpughly implemented and hard coded. 
 
 made by Severin Haas (2012)
 
 */

#include "md5.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * This file builds an executable that performs various functions related
 * to the MD5 library.  Typical compilation:
 *	gcc -o md5cracker -lm md5cracker.c md5.c
 */

static const char *const usage = "\
Usage:\n\
md5main -t				# run the self-test (A.5 of RFC 1321)\n\
md5main -c				# start calculation \n\
md5main -tv				# print the T values \n\
";

static const char *const version = "2002-04-13";

/* Run the self-test. */
static int
do_test(void)
{
    static const char *const test[7*2] = {
		"", "d41d8cd98f00b204e9800998ecf8427e",
		"a", "0cc175b9c0f1b6a831c399e269772661",
		"abc", "900150983cd24fb0d6963f7d28e17f72",
		"message digest", "f96b697d7cb7938d525a2f31aaf161d0",
		"abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		"d174ab98d277d9f5a5611c2c9f419d9f",
		"12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a"
    };
    int i;
    int status = 0;
	
    for (i = 0; i < 7*2; i += 2) {
		md5_state_t state;
		md5_byte_t digest[16];
		char hex_output[16*2 + 1];
		int di;
		
		md5_init(&state);
		md5_append(&state, (const md5_byte_t *)test[i], strlen(test[i]));
		md5_finish(&state, digest);
		for (di = 0; di < 16; ++di)
			sprintf(hex_output + di * 2, "%02x", digest[di]);
		if (strcmp(hex_output, test[i + 1])) {
			printf("MD5 (\"%s\") = ", test[i]);
			puts(hex_output);
			printf("**** ERROR, should be: %s\n", test[i + 1]);
			status = 1;
		}
    }
    if (status == 0)
		puts("md5 self-test completed successfully.");
    return status;
}


//////////////////////////////////////////////////////////////
/* Run the self-test. */
void calculateMD5(char* koord) {
	
	static const char *const sollHash[3] = {
		"d614e8685f8ca15629a2a6f8faae10ee", // N50°37.???' E006°56.???'
		"D614E8685F8CA15629A2A6F8FAAE10EE", // N50°37.???' E006°56.???'
		"c81054589e8a05a5845fe66df9e8683f"  // N50°37.980' E006°56.980'
	};
	
		// Zum gegenprüfen des gefundenen Hashs:
	char md5Test[35];
	char md5Anfang[] = "md5 -s \"";
	char md5Abschluss[] = "\"";

	md5_state_t state;
	md5_byte_t digest[16];
	char hex_output[16*2 + 1];
	int di;

	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)koord, strlen(koord));
	md5_finish(&state, digest);

	for (di = 0; di < 16; ++di) {
		sprintf(hex_output + di * 2, "%02x", digest[di]);
	}

	for (int i = 0; i < 3; i++) {
		if (strcmp(hex_output, sollHash[i]) == 0) {
			strncat(md5Test, md5Anfang, strlen(md5Anfang));
			strncat(md5Test, koord, strlen(koord));
			strncat(md5Test, md5Abschluss, strlen(md5Abschluss));
			
			printf("gefundene Koordinaten %s  :-)\n", koord);
			printf("MD5 (\"%s\") = ", koord);
			puts(hex_output);
			system(md5Test);
			
			strcpy(md5Test, "");
		}
	}

	strcpy(koord, "");
	strcpy(hex_output, "");
}  

void startCalculation() {
	// N50°36.nnn' E006°57.nnn'
	//        ^^^          ^^^

	char koordinaten[26];
	char	k_teil1[] = "N50°37.", 
			k_teil2[] = "' E006°56.", 
			k_teil3[] = "'";
	
	static const char *const zahlenFoo[10] = {
		"0", "1", "2", "3", "4", 
		"5", "6", "7", "8", "9"
	};

	printf("Berechnung wird gestartet.\n");	
	
	for (unsigned short n_hunderter = 0; n_hunderter < 10; n_hunderter++) {		
		for (unsigned short n_zehner = 0; n_zehner < 10; n_zehner++) {
			for (unsigned short n_einer = 0; n_einer < 10; n_einer++) {
				for (unsigned short e_hunderter = 0; e_hunderter < 10; e_hunderter++) {					
					for (unsigned short e_zehner = 0; e_zehner < 10; e_zehner++) {
						for (unsigned short e_einer = 0; e_einer < 10; e_einer++) {
							strcpy(koordinaten, "");
							
								// N- Koordinaten:
							strncat(koordinaten, k_teil1, strlen(k_teil1));
							strncat(koordinaten, zahlenFoo[n_hunderter], strlen(zahlenFoo[n_hunderter]));
							strncat(koordinaten, zahlenFoo[n_zehner], strlen(zahlenFoo[n_zehner]));
							strncat(koordinaten, zahlenFoo[n_einer], strlen(zahlenFoo[n_einer]));

								// E- Koordinaten:
							strncat(koordinaten, k_teil2, strlen(k_teil2));
							strncat(koordinaten, zahlenFoo[e_hunderter], strlen(zahlenFoo[e_hunderter]));
							strncat(koordinaten, zahlenFoo[e_zehner], strlen(zahlenFoo[e_zehner]));
							strncat(koordinaten, zahlenFoo[e_einer], strlen(zahlenFoo[e_einer]));
							strncat(koordinaten, k_teil3, strlen(k_teil3));
							
								// MD5 Hash berechnen:
							calculateMD5(koordinaten);
						}
					}
				}
			}
		}
	} 
	
	
}


//////////////////////////////////////////////////////////////

/* Print the T values. */
static int
do_t_values(void)
{
    int i;
    for (i = 1; i <= 64; ++i) {
		unsigned long v = (unsigned long)(4294967296.0 * fabs(sin((double)i)));
		
		/*
		 * The following nonsense is only to avoid compiler warnings about
		 * "integer constant is unsigned in ANSI C, signed with -traditional".
		 */
		if (v >> 31) {
			printf("#define T%d /* 0x%08lx */ (T_MASK ^ 0x%08lx)\n", i,
				   v, (unsigned long)(unsigned int)(~v));
		} else {
			printf("#define T%d    0x%08lx\n", i, v);
		}
    }
    return 0;
}


/* Main program */
int main(int argc, char *argv[]) {
    if (argc == 2) {
		if (!strcmp(argv[1], "-t")) {
			return do_test();
		}
		
		if (!strcmp(argv[1], "-c")) {
			startCalculation();
		}
		
		if (!strcmp(argv[1], "-h")) {
			puts(usage);
		}
		
		if (!strcmp(argv[1], "-t-values")) {
			return do_t_values();
		}
    }
		//puts(usage);
    return 0;
}
