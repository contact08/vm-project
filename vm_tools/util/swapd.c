#include <stdio.h>
#include <string.h>

//int rename(const char *old, const char *new);

int main(int argc, char **argv)
{
	int	n1, n2;
	char	name1[1024], name2[1024];
	char	oldd[1024], newd[1024];
	if (argc != 3) {
		printf("usage: %s XX_file1 XX_file2\n", argv[0]);
		printf("example: %s 01_dir1 02_dir2\n", argv[0]);
		return 1;
	}
	if (sscanf(argv[1], "%02d_", &n1) != 1) {
		printf("error: %s\n", argv[1]);
		return 1;
	}
	if (sscanf(argv[2], "%02d_", &n2) != 1) {
		printf("error: %s\n", argv[2]);
		return 1;
	}
	strncpy(name1, argv[1]+3, sizeof(name1)-1);
	strncpy(name2, argv[2]+3, sizeof(name2)-1);
	printf("%02d_%s -> %02d_%s\n", n1, name1, n2, name1);
	printf("%02d_%s -> %02d_%s\n", n2, name2, n1, name2);

	// n1 -> temp
	sprintf(oldd, "%02d_%s", n1, name1);
	rename(oldd, "tempd");

	// n2 -> n1
	sprintf(oldd, "%02d_%s", n2, name2);
	sprintf(newd, "%02d_%s", n1, name2);
	rename(oldd, newd);

	// temp -> n2
	sprintf(newd, "%02d_%s", n2, name1);
	rename("tempd", newd);
	return 0;
}
