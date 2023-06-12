#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUF_SIZE 4096
#define DNS_PORT 53

// Estructura del encabezado DNS
typedef struct {
    unsigned short id;
    unsigned char qr_opcode_aa_tc_rd;
    unsigned char ra_z_rcode;
    unsigned short qdcount;
    unsigned short ancount;
    unsigned short nscount;
    unsigned short arcount;
} DNSHeader;

typedef struct {
    unsigned short type;
    unsigned short class;
    unsigned int ttl;
    unsigned short rdlength;
    unsigned char rdata[4];
} DNSRecord;

// Función para enviar una respuesta DNS
void send_dns_response(int sockfd, struct sockaddr_in* cliaddr, DNSHeader* dnsHeader) {
    // Preparamos la respuesta DNS
    DNSHeader responseHeader;
    memset(&responseHeader, 0, sizeof(responseHeader));
    
    // Copiamos el ID de la consulta original al ID de la respuesta
    responseHeader.id = dnsHeader->id;
    // Configuramos el bit de respuesta (QR = 1)
    responseHeader.qr_opcode_aa_tc_rd = (1 << 7);
     responseHeader.ra_z_rcode = 0;  // Establecer el campo "ra_z_rcode" a cero para indicar éxito
    responseHeader.qdcount = dnsHeader->qdcount;  // Igualamos la cantidad de consultas del encabezado original
    responseHeader.ancount = htons(1);  // Por ejemplo, aquí se establece un solo registro de respuesta   
    responseHeader.nscount = htons(0);  // No se incluyen registros de autoridad en este ejemplo
    responseHeader.arcount = 0;  // No se incluyen registros adicionales en este ejemplo

    // Crear una respuesta de ejemplo con un registro de respuesta A
    DNSRecord answer;
    answer.type = htons(1);  // Tipo A
    answer.class = htons(1);  // Clase IN
    answer.ttl = htonl(3600);  // TTL de 3600 segundos
    answer.rdlength = htons(4);  // Longitud de los datos de respuesta (dirección IPv4 de 4 bytes)
    inet_pton(AF_INET, "192.0.2.1", answer.rdata);  // Dirección IPv4 de ejemplo


    // Enviamos la respuesta al cliente
    ssize_t sentBytes = sendto(sockfd, (const char*)&answer, sizeof(answer), 0, (struct sockaddr*)cliaddr, sizeof(struct sockaddr_in));
    if (sentBytes == -1) {
        perror("Error al enviar la respuesta DNS");
        exit(EXIT_FAILURE);
    }
    
    printf("Respuesta DNS enviada al cliente\n");
}

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

            // Verificamos si es una consulta estándar (QR = 1 y OPCODE = 0)
            int isStandardQuery = (qr == 0x01) && (opcode == 0x00);


  // Analizamos el paquete DNS
        DNSHeader* dnsHeader = (DNSHeader*)buffer;
        
        if (!isStandardQuery) { //se realiza el caso 1
            printf("Paquete DNS: Diferente a una consulta estándar\n");
        } else { //se realiza caso 2
            
            printf("Paquete DNS: Consulta estándar recibida\n");
        }

        // Enviamos una respuesta DNS al cliente
        send_dns_response(sockfd, &cliaddr, dnsHeader);
        
    }

    // Cerramos el socket
    close(sockfd);

    return 0;
}
