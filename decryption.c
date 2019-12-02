#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main(){
    int ret, fd, msgSize;
    char * receive = malloc(200);
    char * encryptionKey = malloc(17);
    char source[1000000];
    char * toEncrypt = malloc(150 * sizeof(char));
    FILE *fp = fopen("encrypted", "r");
    if(fp != NULL){
      char symbol;
      while((symbol = getc(fp)) != EOF){
	strcat(source, &symbol);
      }
      fclose(fp);
    }

    for(int i = 0; i < 150; i++){
      toEncrypt[i] = source[i];
    }
    toEncrypt[149] = '\0';
    printf("file contents - %s\n", toEncrypt);
    fd = open("/dev/decdev", O_RDWR);
    if (fd < 0){
      printf("Failed to open the device... error #%d\n", fd);
      return errno;
    }
    msgSize = write(fd, toEncrypt, strlen(toEncrypt));
    printf("write return: %d\n", msgSize);
    printf("The received encryption key is: [%s]\n", encryptionKey);
    ret = read(fd, receive, msgSize);
    if (ret < 0){
	printf("Failed to read the message from the device. error #%d\n", ret);
	close(fd);
	return errno;
    }
    close(fd);
    printf("The received encrypted message is: [%s]\n", receive);

    FILE *file = fopen("decrypted", "w");
    fputs(receive, file);
    fclose(file);
    
    printf("End of the program\n");
    return 0;
}
