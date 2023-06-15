#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUF_SIZE 4096
#define DNS_PORT 53

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[BUF_SIZE];

    // Creamos el socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configuramos la dirección del servidor
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  //el sokcet estara vinculado a todas las direcciones IP disponibles en la maquina
    servaddr.sin_port = htons(DNS_PORT);

    // Vinculamos el socket a la dirección del servidor
    if (bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error al vincular el socket");
        exit(EXIT_FAILURE);
    }

    printf("Esperando paquetes DNS en el puerto %d...\n", DNS_PORT);

    while (1) {
        socklen_t len = sizeof(cliaddr);
        ssize_t n = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&cliaddr, &len);
        if (n < 0) {
            perror("Error al recibir datos");
            exit(EXIT_FAILURE);
        }

        // Analizamos el paquete DNS
        // Implementamos la lógica para examinar el paquete según el RFC2929  (TODO)


        
        /*
        * Verificamos si el paquete DNS es una consulta estándar o no
        */

            // Obtenemos el tercer byte del paquete DNS (encabezado)
            unsigned char header = buffer[2];

            // Extraemos el campo QR (bit más significativo del byte 2)
            int qr = (header >> 7) & 0x01;

            // Extraemos el campo OPCODE (bits 3 a 6 del byte 2)
            int opcode = (header >> 3) & 0x0F;

            // Verificamos si es una consulta estándar (QR = 0 y OPCODE = 0)
            int isStandardQuery = (qr == 0x00) && (opcode == 0x00);



        if (!isStandardQuery) { //se realiza el caso 1
            printf("Paquete DNS: Diferente a una consulta estándar\n");
        } else { //se realiza caso 2
            
            printf("Paquete DNS: Consulta estándar recibida\n");
        }
    }

    // Cerramos el socket
    close(sockfd);

    return 0;
}
