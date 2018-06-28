#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
   FILE *fp = fopen("./input.txt","r");
   char buff[255][255];
   char temp[255];
   int n = 0;
   int i = 0;
   int j = 0;
   int k = 0;
   int l = 0;
   while(!feof(fp))
   {
       fgets(buff[n],255,fp);
//       printf("get the %d line :%s\n",n,buff[n]);
       n++;
   }
   n--;
   for(i = 0;i < n-1;i++)
   { 
       for(j = 0;j < n-1-i;j++)
       {                  
            for(k = 0;k < 255;k++)
            {    
               if(buff[j][k] < buff[j+1][k])
               {
                   for(l = 0;l < 255;l++)
                   {
                       temp[l] = buff[j][l];
                   }   
                   for(l = 0;l < 255;l++)
                   {
                       buff[j][l] = buff[j+1][l];
                   }
                   for(l = 0;l < 255;l++)
                   {
                       buff[j+1][l] = temp[l];
                   }
                   break;
                }
                else if(buff[j][k] > buff[j+1][k])
                    break;
              }
       }
   }
   for(i = 0;i < n;i++)
   {
       printf("%s",buff[i]);
   }
   return 0;
}

