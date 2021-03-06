/*Функция для просмотра содержимого текущего каталога*/
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void printdir(char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if ((dp = opendir(dir)) == NULL) 
    {
    fprintf(stderr, "cannot open directory: %s\n", dir);
    return;
    }
    chdir(dir);

while((entry = readdir(dp)) != NULL) 
    {
    lstat(entry->d_name, &statbuf);
    if (S_ISDIR(statbuf.st_mode)) 
        {
        /* Находит каталог, но игнорирует . и .. */
        if (strcmp(".", entry->d_name) == 0 || strcmp ("..", entry->d_name) == 0)
            continue;
        printf("%*s%s/\n", depth, "", entry->d_name);
        /* Рекурсивный вызов с новым отступом */
        printdir(entry->d_name, depth+4);
        }
    else printf("%*s%s\n", depth, " ", entry->d_name);
    }
    chdir("..");
    closedir(dp);
}
int main()
{
    /* Обзор каталога /home */
    printf("Directory scan of /home/dmitry/Рабочий стол/Опирационные системы:\n");
    printdir("/home/dmitry/Рабочий стол/Опирационные системы", 0);
    printf("done.\n");
    exit (0);
}