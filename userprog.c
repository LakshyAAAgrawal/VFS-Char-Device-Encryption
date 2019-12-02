#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main(){
    int ret, fd, msgSize;
    char * receive = malloc(100);
    printf("Starting device test code example...\n");
    fd = open("/dev/encdev", O_RDWR);
    if (fd < 0){
	printf("Failed to open the device... error #%d\n", fd);
	return errno;
    }
    char * xx = "wuehfulalalalalalalalalalalaalalaalalwiuefhweuirhfuywrgyuwgrfgwr111";
    msgSize = write(fd, xx, strlen(xx));
    printf("write return: %d\n", msgSize);
    ret = read(fd, receive, 16);
    if (ret < 0){
	printf("Failed to read the message from the device. error #%d\n", ret);
	return errno;
    }
    printf("The received encryption key is: [%s]\n", receive);
    
    ret = read(fd, receive, msgSize);
    if (ret < 0){
	printf("Failed to read the message from the device. error #%d\n", ret);
	return errno;
    }
    printf("The received encrypted message is: [%s]\n", receive);
    printf("End of the program\n");
    return 0;
}
