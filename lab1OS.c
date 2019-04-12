#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define BUF_SIZE 512

void createarch(char *dir)
{
	DIR *dp = opendir(dir);
	struct dirent *entry;
	struct stat statbuf;

	if (dp == NULL) {
		printf("I don't see any dir \\(o.o)/\n");
		return;
	}
	chdir(dir);
	while ((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode)) {
			int f1 = strcmp(".", entry->d_name);
			int f2 = strcmp("..", entry->d_name);

			if (f1 == 0 || f2 == 0)
				continue;
			createarch(entry->d_name);
		}
	}
	chdir("..");

	char archname[BUF_SIZE] = "";
	char *c = dir;

	while (strstr(c, "/") != NULL)
		c = strstr(c, "/") + 1;
	strcpy(archname, c);
	strcat(archname, ".anarchy");
	FILE *arch = fopen(archname, "wb");

	rewinddir(dp);
	chdir(dir);
	while ((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name, &statbuf);
		if (!S_ISDIR(statbuf.st_mode)) {
			long in_size;
			FILE *in = fopen(entry->d_name, "rb");

			if (in != NULL) {
				char buf[BUF_SIZE];
				size_t n_obj;

				fseek(in, 0, SEEK_END);
				in_size = ftell(in);
				rewind(in);
				fwrite(entry->d_name, sizeof(char), BUF_SIZE, arch);
				fwrite(&in_size, sizeof(long), 1, arch);
				while ((n_obj = fread(buf, 1, BUF_SIZE, in)) != 0)
					fwrite(buf, 1, n_obj, arch);
				fclose(in);
			}
			if (strstr(entry->d_name, ".anarchy") != NULL)
				remove(entry->d_name);
		}
	}
	chdir("..");
	fclose(arch);
	closedir(dp);
}

void openarch(char *dir)
{
	char *c = dir;
	char dirname[BUF_SIZE] = "";
	char updirname[BUF_SIZE] = "";

	while (strstr(c, "/") != NULL)
		c = strstr(c, "/") + 1;
	strncpy(updirname, dir, strlen(dir)-strlen(c));
	chmod(updirname, 0777);
	strncpy(dirname, dir, strlen(dir)-4);
	mkdir(dirname, 0777);

	DIR *dp = opendir(dirname);
	FILE *arch = fopen(dir, "rb");

	if (arch == NULL) {
		printf("I don't see any file \\(o.o)/\n");
		exit(0);
	}

	chdir("..");
	chdir(dirname);

	char str[BUF_SIZE];

	while (fread(str, sizeof(char), BUF_SIZE, arch) > 0) {
		char filename[BUF_SIZE*2] = "";
		long filelength;
		char buf[BUF_SIZE] = "";

		strncpy(filename, dirname, strlen(dirname));
		strcat(filename, "/");
		strncat(filename, str, strlen(str));
		FILE *out = fopen(filename, "wb");

		fread(&filelength, sizeof(long), 1, arch);
		while (filelength > 0) {
			if (filelength > BUF_SIZE) {
				fread(buf, 1, BUF_SIZE, arch);
				fwrite(buf, 1, BUF_SIZE, out);
				filelength = filelength-BUF_SIZE;
			} else {
				fread(buf, 1, filelength, arch);
				fwrite(buf, 1, filelength, out);
				filelength = 0;
			}
		}
		fclose(out);
	}
	fclose(arch);

	struct dirent *entry;
	struct stat statbuf;

	while ((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name, &statbuf);
		if (!S_ISDIR(statbuf.st_mode)) {
			if (strstr(entry->d_name, ".anarchy") != NULL) {
				char nextdir[BUF_SIZE] = "";
				char nextpath[BUF_SIZE] = "";
				size_t namelength = strlen(entry->d_name)-4;

				strncpy(nextdir, entry->d_name, namelength);
				rename(entry->d_name, nextdir);
				strcpy(nextpath, dirname);
				strcat(nextpath, "/");
				strcat(nextpath, nextdir);
				openarch(nextpath);
				remove(nextpath);
				rewinddir(dp);
			}
		}
	}
	chdir("..");
	closedir(dp);
}

void main(int argc, char *argv[])
{
	char *topdir = ".";

	if (argc >= 2)
		topdir = argv[1];
	if (strstr(topdir, ".ana") != NULL)
		openarch(topdir);
	else {
		createarch(topdir);
		char name[BUF_SIZE] = "";
		char newname[BUF_SIZE] = "";

		strcpy(newname, topdir);
		strcat(newname, ".ana");
		strcpy(name, newname);
		strcat(name, "rchy");
		rename(name, newname);
	}
	exit(0);
}
