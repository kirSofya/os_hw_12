#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER 255

void DieWithError(char *errorMessage) {
    perror(errorMessage);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in broadcast_addr;
    char *broadcast_ip;
    unsigned short broadcast_port;
    int broadcastPermission;
    
    if (argc < 3) {
        fprintf(stderr, "Usage:  %s <IP Address> <Port>\n", argv[0]);
        exit(1);
    }
    
    broadcast_ip = argv[1];         // First arg:  broadcast IP address
    broadcast_port = atoi(argv[2]); // Second arg:  broadcast port
    
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        DieWithError("socket() failed");
    }
    
    broadcastPermission = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission,
                   sizeof(broadcastPermission)) < 0) {
        DieWithError("setsockopt() failed");
    }

    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_addr.s_addr = inet_addr(broadcast_ip);
    broadcast_addr.sin_port = htons(broadcast_port);
    
    printf("Enter 'end' to finish\n");
    printf("Enter a message: ");
    
    char buffer[BUFFER];
    while (fgets(buffer, BUFFER, stdin)) {
        printf("\n");
        if (sendto(sock, buffer, BUFFER, 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) != BUFFER){
            DieWithError("sendto() failed");
        }
        if (strcmp(buffer, "end\n") == 0) {
            printf("finishing\n");
            break;
        }
        memset(buffer, 0, BUFFER);
        printf("Enter a message: ");
    }
    
    close(sock);
    exit(0);
}
