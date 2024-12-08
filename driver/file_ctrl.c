#include "file_ctrl.h"

int get_file_size(char *path)
{
    struct stat buf;
    return (stat(path, &buf) == 0) ? buf.st_size : 0;
}

int get_file_number(char *path)
{
    int num = 0;
    DIR *dirptr = NULL;
    struct dirent *entry;

    if((dirptr = opendir(path)) == NULL)
    {
        return -1;
    }

    while((entry = readdir(dirptr)))
    {
        if(entry->d_type == DT_REG)
        {
            num++;
        }
    }
    closedir(dirptr);
    return num;
}

int load_file_data(const char *name, unsigned char *buf, int length)
{
	FILE *fp = fopen(name, "rb");

	int len = fread(buf, 1, length, fp);

	fclose(fp);

	return len;
}

int save_file_data(const char *name, unsigned char *buf, int length)
{
	int count = 0;

	FILE *fp = fopen(name, "wb");

	while(count < length)
	{
		fwrite(buf++, sizeof(char), 1, fp);
		count++;
	}

	fclose(fp);
	system("sync");
	return 0;
}