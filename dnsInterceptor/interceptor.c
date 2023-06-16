#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h> // Para tipos de datos enteros de tamaño fijo
// Hilos
#include <pthread.h>
// Errores
#include <errno.h>


#define BUF_SIZE 4096
#define DNS_PORT 53
// Tamaño máximo del nombre de dominio
#define MAX_DOMAIN_NAME_LENGTH 256



// Estructura para almacenar un respuesta DNS 
struct Query {    
    char* name; // 255 bytes
    uint16_t type;
    uint16_t class;
};

// Estructura para almacenar un respuesta DNS 
struct Response {
    uint16_t name;
    uint16_t type;
    uint16_t class;
    uint16_t ttl;
    uint16_t rdlength;
    char* rdata;
};

// Estructura para almacenar un registro DNS
struct DNSHeader {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;    
    struct Query query;
    struct Response response;
};

// Esta estructura se utilizará para los parametros del hilo
typedef struct Settings{
	char *buffer;    	
	int sockfd;
    struct sockaddr_in cliaddr;
} Settings;

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
/*
// Función para extraer la dirección IP de la consulta DNS
void extract_ip_address(const unsigned char* buffer, int offset, char* ip_address) {
    sprintf(ip_address, "%u.%u.%u.%u", buffer[offset], buffer[offset + 1], buffer[offset + 2], buffer[offset + 3]);
}

// Función para pasar una string a una variable uint64_t esto para que acepte el nombre en la respuestas DNS
uint64_t string_to_uint64(const char* str) {
    uint64_t result = 0;
    int i = 0;
    while (str[i] != '\0') {
        result = result << 8;
        result += str[i];
        i++;
    }
    return result;
}

// Función para enviar una respuesta DNS
void send_dns_response(int sockfd, struct sockaddr_in* cliaddr) {
    // Preparamos la respuesta DNS
    // Declaración de la estructura
    struct DNSResponse responseHeader;
    memset(&responseHeader, 0, sizeof(responseHeader));

    // Asignar valores al registro de recursos DNS

    //  Nombre de dominio comprimido
    // const char* domainName = "example.com"; // Nombre de dominio
    // uint64_t name = string_to_uint64(domainName);


    //responseHeader.name = 0x6578616D706C652E636F6D; // Nombre de dominio comprimido
    responseHeader.type = 0x1; // Tipo A
    responseHeader.class = 0x1; // Clase IN
    responseHeader.ttl = 3600; // Tiempo de vida en segundos
    responseHeader.rdlength = 0x4; // Longitud de la dirección IP en bytes
    responseHeader.rdata = 0xC06F0001;  // RDATA con la dirección IP 192.111.0.1 en formato hexadecimal

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

    if (sentBytes == -1) {
        perror("Error al enviar la respuesta DNS");
        exit(EXIT_FAILURE);
    }
    
    printf("Respuesta DNS enviada al cliente\n");
}

// Función para leer el contenido del DNS
void read_dns(const unsigned char* buffer) {
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
    printf("\t ID: %d\n", header.id);
    printf("\t QR: %d\n", (header.flags >> 15) & 0x01);
    printf("\t Opcode: %d\n", (header.flags >> 11) & 0x0F);
    printf("\t AA: %d\n", (header.flags >> 10) & 0x01);
    printf("\t TC: %d\n", (header.flags >> 9) & 0x01);
    printf("\t RD: %d\n", (header.flags >> 8) & 0x01);
    printf("\t RA: %d\n", (header.flags >> 7) & 0x01);
    printf("\t Z: %d\n", (header.flags >> 4) & 0x07);
    printf("\t AD: %d\n", (header.flags >> 3) & 0x01);
    printf("\t CD: %d\n", (header.flags >> 2) & 0x01);
    printf("\t RCODE: %d\n", header.flags & 0x0F);
    printf("\t QDCOUNT: %d\n", header.qdcount);
    printf("\t ANCOUNT: %d\n", header.ancount);
    printf("\t NSCOUNT: %d\n", header.nscount);
    printf("\t ARCOUNT: %d\n", header.arcount);
}
*/
// Función para procesar el paquete DNS
void *dns_function(void *arg){
    Settings * sett = (Settings *)arg;
    // sett->buffer;
    // sett->sockfd;
    // sett->cliaddr;

/*
    // Analizamos el paquete DNS
    // Implementamos la lógica para examinar el paquete según el RFC2929  (TODO)
    unsigned char header2 = sett->buffer[2];

    // Extraemos el campo QR (bit más significativo del byte 2)
    int qr = (header2 >> 7) & 0x01;
    // Extraemos el campo OPCODE (bits 3 a 6 del byte 2)
    int opcode = (header2 >> 3) & 0x0F;
    // Verificamos si es una consulta estándar (QR = 0 y OPCODE = 0)
    int isStandardQuery = (qr == 0x00) && (opcode == 0x00);
    
    // Imprimimos el contenido del paquete DNS
    read_dns(sett->buffer);
    // Extraer el nombre de dominio de la consulta DNS
    char domain_name[MAX_DOMAIN_NAME_LENGTH];
    extract_domain_name(sett->buffer, 12, domain_name);
    printf("Nombre de dominio consultado: %s\n", domain_name);

*/

/* Intento de obtener IP de origen
    // Extraer la dirección IP de la consulta DNS
    char ip_address[INET_ADDRSTRLEN];
    extract_ip_address(buffer, 12 + strlen(domain_name) + 2, ip_address);
    printf("Dirección IP consultada: %s\n", ip_address);

    // Obtener la dirección IP de origen
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(cliaddr.sin_addr), clientIP, INET_ADDRSTRLEN);
    printf("IP de origen: %s\n", clientIP);
*/

/*  
    if (!isStandardQuery) { //se realiza el caso 1
        printf("Paquete DNS: Diferente a una consulta estándar\n");
    } else { //se realiza caso 2
        
        printf("Paquete DNS: Consulta estándar recibida\n");
    }
    
*/

    // Enviamos una respuesta DNS al cliente
    
    //   send_dns_response(sett->sockfd, &sett->cliaddr);

    pthread_detach(pthread_self());
}

// Función principal
int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

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
        
        char buffer[BUF_SIZE];
        ssize_t n = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&cliaddr, &len);
        if (n < 0) {
            perror("Error al recibir datos");
            exit(EXIT_FAILURE);
        }
        
        Settings * sett = (Settings *)malloc(sizeof(Settings));
        sett->buffer = buffer;
        sett->sockfd = sockfd;
        sett->cliaddr = cliaddr;

            /*
        pthread_t dns_thread;
        if(pthread_create(&dns_thread, NULL, &dns_function, (void*)sett) != 0){
            printf("\n \t Error dns thread\n");
            return EXIT_FAILURE;
        }   */ 

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
    printf("\t ID: %d\n", header.id);
    printf("\t QR: %d\n", (header.flags >> 15) & 0x01);
    printf("\t Opcode: %d\n", (header.flags >> 11) & 0x0F);
    printf("\t AA: %d\n", (header.flags >> 10) & 0x01);
    printf("\t TC: %d\n", (header.flags >> 9) & 0x01);
    printf("\t RD: %d\n", (header.flags >> 8) & 0x01);
    printf("\t RA: %d\n", (header.flags >> 7) & 0x01);
    printf("\t Z: %d\n", (header.flags >> 4) & 0x07);
    printf("\t AD: %d\n", (header.flags >> 3) & 0x01);
    printf("\t CD: %d\n", (header.flags >> 2) & 0x01);
    printf("\t RCODE: %d\n", header.flags & 0x0F);
    printf("\t QDCOUNT: %d\n", header.qdcount);
    printf("\t ANCOUNT: %d\n", header.ancount);
    printf("\t NSCOUNT: %d\n", header.nscount);
    printf("\t ARCOUNT: %d\n", header.arcount);
    
    // Extraer el nombre de dominio de la consulta DNS
    char domain_name[MAX_DOMAIN_NAME_LENGTH];
    extract_domain_name(buffer, 12, domain_name);
    printf("Nombre de dominio consultado: %s\n", domain_name);

        // Establecer el bit QR en 1
        header.flags |= (1 << 15);
        // Establecer el bit RA en 1
      //  header.ancount = htons(1);


        struct Query query;
        memset(&query, 0, sizeof(query));

        // Esto me deja enfrente del nombre del query (buffer + 12);

        query.name = (buffer + 12);
        query.type = htons(1);
        query.class = htons(1);

        header.query = query;

        struct Response response;
        memset(&response, 0, sizeof(response));
        response.name = htons(0xC00C);
        response.type = htons(1);
        response.class = htons(1);
        response.ttl = htons(3600);
        response.rdlength = htons(4);
        response.rdata = htonl(0xC06F0001);//inet_addr("192.111.0.1");

        header.response = response;


/*
 // Assign values to the DNS response fields 
         struct DNSResponse responseHeader;
        memset(&responseHeader, 0, sizeof(responseHeader));

    char dnsName[] = { 3, 'w', 'w', 'w', 7, 'e', 'x', 'a', 'm', 'p', 'l', 'e', 3, 'c', 'o', 'm', 0 };
    memcpy(responseHeader.name, dnsName, sizeof(dnsName));

    responseHeader.type = htons(1);  // Type A
    responseHeader.class = htons(1); // Class IN
    responseHeader.ttl = htons(3600);
    responseHeader.rdlength = htons(4);
    responseHeader.rdata = inet_addr("192.111.0.1"); // Convert IP address to network byte order
    
    struct DNSResponsePacket packet;
    memset(&packet, 0, sizeof(packet));

    packet.header = header;
    packet.response = responseHeader;
*/
        // Enviamos la respuesta al cliente
        ssize_t sentBytes = 
        sendto(
            sockfd, 
            &header, 
            sizeof(header), 
            0, 
            (struct sockaddr*)&cliaddr, 
            sizeof(struct sockaddr_in)
        );

        if (sentBytes == -1) {
            perror("Error al enviar la respuesta DNS");
            exit(EXIT_FAILURE);
        }
        
        printf("Respuesta DNS enviada al cliente\n");

    }

    // Cerramos el socket
    close(sockfd);

    return 0;
}
