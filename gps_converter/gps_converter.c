#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *f = fopen("coordinates.txt", "r");

	char * line = NULL;
	size_t len = 0;
	int read;



	if (f == NULL){
		printf("File does not exist!");
		return 0;
	}

	while ((read = getline(&line, &len, f)) != -1) {
		printf("Retrieved line of length %zu :\n", read);
		printf("%s", line);
	}
	//some comment



	char sz[] = "Hello, World!\n";	/* Hover mouse over "sz" while debugging to see its contents */
	printf("%s", sz);	
	fflush(stdout); /* <============== Put a breakpoint here */
	return 0;

}
