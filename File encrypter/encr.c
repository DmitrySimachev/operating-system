#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{   
   // int b_mas[22]; // счетчик букв
   // char c_mas[22]; // буква

    FILE *f;
    f = fopen("key","r");
        int k,i,l[100],x[100];
    for(k=0;!feof(f);k++)
        fscanf(f,"%d",&l[k]);
    for(i=0;i<k-1;i++)
    {
        x[i]=l[i];
        printf("%d ",x[i]);
    }

}