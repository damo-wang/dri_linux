#include <stdio.h>

int main()
{
	FILE *fp=fopen("/dev/mydev","r");

	fclose(fp);
	return 0;
}
