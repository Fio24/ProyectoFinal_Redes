#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h> // Para tipos de datos enteros de tamaño fijo

#define BUF_SIZE 4096
#define DNS_PORT 53
// Tamaño máximo del nombre de dominio
#define MAX_DOMAIN_NAME_LENGTH 256

// Estructura para almacenar un registro DNS
struct DNSHeader {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
};

// Estructura para almacenar un respuesta DNS 
struct DNSResponse {
    uint64_t name;
    uint16_t type;
    uint16_t class;
    uint16_t ttl;
    uint16_t rdlength;
    uint32_t rdata;
};

/*
 * This will convert www.google.com to 3www6google3com 
 * got it :)
 * */
void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host) 
{
	int lock = 0 , i;
	strcat((char*)host,".");
	
	for(i = 0 ; i < strlen((char*)host) ; i++) 
	{
		if(host[i]=='.') 
		{
			*dns++ = i-lock;
			for(;lock<i;lock++) 
			{
				*dns++=host[lock];
			}
			lock++; //or lock=i+1;
		}
	}
	*dns++='\0';
}

// Función para extraer el nombre de dominio de la consulta DNS
void extract_domain_name(const unsigned char* buffer, int offset, char* domain_name) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (buffer[offset + i] != 0) {
        if (i != 0) {
            domain_name[j++] = '.';
        }
        for (k = 0; k < buffer[offset + i]; k++) {
            domain_name[j++] = buffer[offset + i + k + 1];
        }
        i += buffer[offset + i] + 1;
    }
    domain_name[j] = '\0';
}

// Función para extraer la dirección IP de la consulta DNS
void extract_ip_address(const unsigned char* buffer, int offset, char* ip_address) {
    sprintf(ip_address, "%u.%u.%u.%u", buffer[offset], buffer[offset + 1], buffer[offset + 2], buffer[offset + 3]);
}

// Función para enviar una respuesta DNS
void send_dns_response(int sockfd, struct sockaddr_in* cliaddr) {
    // Preparamos la respuesta DNS
    // Declaración de la estructura
    struct DNSResponse responseHeader;
    memset(&responseHeader, 0, sizeof(responseHeader));
  
    // Asignar valores al registro de recursos DNS

    const char* domainName = "example.com";
    uint64_t name = 0;

    for (int i = 0; i < strlen(domainName); i++) {
        // Agregar el valor ASCII del carácter al entero utilizando una operación OR
        name |= (uint64_t)domainName[i];
        // Realizar el corrimiento de bits a la izquierda para dar espacio al siguiente carácter
        name <<= 8;
    }

    responseHeader.name = name; // Nombre de dominio comprimido
    responseHeader.type = htons(1); // Tipo A
    responseHeader.class = htons(1); // Clase IN
    responseHeader.ttl = htonl(3600);
    responseHeader.rdlength = htons(4);
    // Convertir la dirección IP de cadena a formato de red y asignarla a rdata
    if (inet_pton(AF_INET, "192.111.0.1", &(responseHeader.rdata)) != 1) {
        perror("Error al convertir la dirección IP");
        exit(EXIT_FAILURE);
    }


    // Enviamos la respuesta al cliente
    ssize_t sentBytes = 
    sendto(
        sockfd, 
        &responseHeader, 
        sizeof(responseHeader), 
        0, 
        (struct sockaddr*)cliaddr, 
        sizeof(struct sockaddr_in)
    );

    // Enviamos la respuesta al cliente
    sentBytes = 
    sendto(
        sockfd, 
        &responseHeader, 
        sizeof(responseHeader), 
        0, 
        (struct sockaddr*)cliaddr, 
        sizeof(struct sockaddr_in)
    );

        // Enviamos la respuesta al cliente
    sentBytes = 
    sendto(
        sockfd, 
        &responseHeader, 
        sizeof(responseHeader), 
        0, 
        (struct sockaddr*)cliaddr, 
        sizeof(struct sockaddr_in)
    );

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

        unsigned char header2 = buffer[2];

        // Extraemos el campo QR (bit más significativo del byte 2)
        int qr = (header2 >> 7) & 0x01;

        // Extraemos el campo OPCODE (bits 3 a 6 del byte 2)
        int opcode = (header2 >> 3) & 0x0F;

        // Verificamos si es una consulta estándar (QR = 1 y OPCODE = 0)
        int isStandardQuery = (qr == 0x01) && (opcode == 0x00);
        
        // Declaración de la estructura
        struct DNSHeader header;

            // Copiar los valores del buffer a la estructura utilizando desplazamiento de bits
            header.id = (buffer[0] << 8) | buffer[1];
            header.flags = (buffer[2] << 8) | buffer[3];
            header.qdcount = (buffer[4] << 8) | buffer[5];
            header.ancount = (buffer[6] << 8) | buffer[7];
            header.nscount = (buffer[8] << 8) | buffer[9];
            header.arcount = (buffer[10] << 8) | buffer[11];

            // Imprimir los valores de la estructura
            printf("ID: %d\n", header.id);
            printf("QR: %d\n", (header.flags >> 15) & 0x01);
            printf("Opcode: %d\n", (header.flags >> 11) & 0x0F);
            printf("AA: %d\n", (header.flags >> 10) & 0x01);
            printf("TC: %d\n", (header.flags >> 9) & 0x01);
            printf("RD: %d\n", (header.flags >> 8) & 0x01);
            printf("RA: %d\n", (header.flags >> 7) & 0x01);
            printf("Z: %d\n", (header.flags >> 4) & 0x07);
            printf("AD: %d\n", (header.flags >> 3) & 0x01);
            printf("CD: %d\n", (header.flags >> 2) & 0x01);
            printf("RCODE: %d\n", header.flags & 0x0F);
            printf("QDCOUNT: %d\n", header.qdcount);
            printf("ANCOUNT: %d\n", header.ancount);
            printf("NSCOUNT: %d\n", header.nscount);
            printf("ARCOUNT: %d\n", header.arcount);
        
        // Extraer el nombre de dominio de la consulta DNS
        char domain_name[MAX_DOMAIN_NAME_LENGTH];
        extract_domain_name(buffer, 12, domain_name);
        printf("Nombre de dominio consultado: %s\n", domain_name);

        // Extraer la dirección IP de la consulta DNS
        char ip_address[INET_ADDRSTRLEN];
        extract_ip_address(buffer, 12 + strlen(domain_name) + 2, ip_address);
        printf("Dirección IP consultada: %s\n", ip_address);

        // Obtener la dirección IP de origen
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(cliaddr.sin_addr), clientIP, INET_ADDRSTRLEN);
        printf("IP de origen: %s\n", clientIP);

        if (!isStandardQuery) { //se realiza el caso 1
            printf("Paquete DNS: Diferente a una consulta estándar\n");
        } else { //se realiza caso 2
            
            printf("Paquete DNS: Consulta estándar recibida\n");
        }

        // Enviamos una respuesta DNS al cliente
        send_dns_response(sockfd, &cliaddr);
        
    }

    // Cerramos el socket
    close(sockfd);

    return 0;
}
