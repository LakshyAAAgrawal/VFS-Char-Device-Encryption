#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
int main(){
   int ret, fd;
   char * receive = malloc(17);
   printf("Starting device test code example...\n");
   fd = open("/dev/encdev", O_RDWR);
   if (fd < 0){
     printf("Failed to open the device... error #%d\n", fd);
     return errno;
   }
   ret = write(fd, "wuehfu", strlen("wuehfu"));
   printf("write return: %d\n", ret);
   ret = read(fd, receive, 16);        // Read the response from the LKM
   if (ret < 0){
     printf("Failed to read the message from the device. error #%d\n", ret);
     return errno;
   }
   printf("The received message is: [%s]\n", receive);
   printf("End of the program\n");
   return 0;
}
