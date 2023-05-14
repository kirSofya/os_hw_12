#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXRECVSTRING 255

void DieWithError(char *errorMessage) {
    perror(errorMessage);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in broadcast_addr;
    unsigned short broadcast_port;
    char recv_string[MAXRECVSTRING + 1];
    int recv_string_len;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Broadcast Port>\n", argv[0]);
        exit(1);
    }
    broadcast_port = atoi(argv[1]);
    
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        DieWithError("socket() failed");
    }
    
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    broadcast_addr.sin_port = htons(broadcast_port);
    
    if (bind(sock, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) < 0) {
        DieWithError("bind() failed");
    }
    
    while (1) {
        if ((recv_string_len = recvfrom(sock, recv_string, MAXRECVSTRING, 0, NULL, 0)) < 0) {
            DieWithError("recvfrom() failed");
        }
        if (strcmp(recv_string, "end\n") == 0) {
            printf("finishing\n");
            break;
        }

        recv_string[recv_string_len] = '\0';
        printf("Received: %s\n", recv_string);
    }
    close(sock);
    exit(0);
}
