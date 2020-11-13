#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

char *flag = "-e";	// sets default flag to -e
int key = 7;		// sets default key to 7
int isfile = 0;		// determines if input from file or stdin
FILE *fp; 

// Prints usage and exits program
void usage() {
	printf("Usage: caesar [flags] [key] [file]\n\n"); 
	printf("Available Flags:\n"); 
	printf("\t-e, \t\tencrypt\n"); 
	printf("\t-d, \t\tdecrypt\n\n");
	exit(1);
}

// Determines if all characters of an argument are integers
int is_int(char *arg) {
	int i = 0;
	if (arg[0] == '-') i = 1;

	for (; i < strlen(arg); i++)
		if (!isdigit(arg[i])) {
			return 0;
		}
	return 1;
}

// Determines if key is too large or small
int int_overflow(long a) {
	if (a > 0 && 123 > INT_MAX - a) usage();
	else if (a < 0 && 123 < INT_MIN - a) usage();
	return 0;
}

// Reads file and writes to buffer
char *read_file() {
	int sz;
	char *message;

	fseek(fp, 0L, SEEK_END);
	sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET); 

	message = malloc(sz * sizeof(char));
	for (int i = 0; i < sz; i++)
		message[i] = fgetc(fp);
	message[sz - 1] = '\0';

	fclose(fp);
	return message;
}

// Reads stdin and writes to buffer
char *read_stdin() {
	size_t max = 4096;
	int c, len = 0;
	char *message = malloc(max * sizeof(char));

	while ((c = fgetc(stdin)) != '\n' && !feof(stdin)) {
		message[len] = c;
		
		if (++len == max) 
			message = realloc(message, (max *= 2) * sizeof(char));
	}

	message = realloc(message, (len + 1) * sizeof(char));
	message[len] = '\0';

	return message;
}

// Determines type of argument and sets corresponding variable
void set_args(char *arg) {
	if (!strcmp(arg, "-e") || !strcmp(arg, "-d")) {
		flag = arg;
		//printf("arg [%s] is flag\n", arg);
	} 
	else if (is_int(arg) && !int_overflow(strtol(arg, 0, strlen(arg)))) {
		key = atoi(arg);
		//printf("arg [%s] is key\n", arg);
	}
	else if ((fp = fopen(arg, "r"))) {
		isfile = 1;
		//printf("arg [%s] is filepath\n", arg);
	} else usage();
}	

// Runs (argc - 1) checks for given argv
void check_args(int argc, char *argv[]) {
	switch(argc) {
		case 1: break;
		case 2: set_args(argv[1]);
			break;
		case 3: set_args(argv[1]);
			set_args(argv[2]);
			break;
		case 4: set_args(argv[1]);
			set_args(argv[2]);
			set_args(argv[3]); break;
		default: usage();
			 break;
	}
}

// Caesar cipher
void cipher() {
	int x;
	char *message;

	if (isfile) message = read_file();
	else message = read_stdin();

	if (!strcmp(flag, "-d")) key = -key;
	
	for (int i = 0; i < strlen(message); i++) {
		if (isupper(message[i])) {
			x = 65;
			if (((message[i] + key - 65) % 26) < 0) x = 91;
			printf("%c", ((int)message[i] + key - 65) % 26 + x);	
		}
		else if (islower(message[i])) {
			x = 97;
			if (((message[i] + key - 97) % 26) < 0) x = 123;
			printf("%c", ((int)message[i] + key - 97) % 26 + x);
		}				
		else printf("%c", message[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	check_args(argc, argv);
	cipher();
	return 0;	
}
