import socket

import struct

def parse_dns_flags(flags):
    QR = (flags >> 15) & 0x01
    OPCODE = (flags >> 11) & 0x0F
    AA = (flags >> 10) & 0x01
    TC = (flags >> 9) & 0x01
    RD = (flags >> 8) & 0x01
    RA = (flags >> 7) & 0x01
    Z = (flags >> 4) & 0x07
    AD = (flags >> 3) & 0x01
    CD = (flags >> 2) & 0x01
    RCODE = flags & 0x0F

    print(f"QR: {QR}")
    print(f"OPCODE: {OPCODE}")
    print(f"AA: {AA}")
    print(f"TC: {TC}")
    print(f"RD: {RD}")
    print(f"RA: {RA}")
    print(f"Z: {Z}")
    print(f"AD: {AD}")
    print(f"CD: {CD}")
    print(f"RCODE: {RCODE}")
    
def parse_dns_packet(packet):
    # Analizar el encabezado del paquete DNS
    header = struct.unpack('!HHHHHH', packet[:12])
    id, flags, qdcount, ancount, nscount, arcount = header

    print(f"ID: {id}")
    print(f"Flags: {flags}")
    parse_dns_flags(flags)
    print(f"QDCount: {qdcount}")
    print(f"ANCount: {ancount}")
    print(f"NSCount: {nscount}")
    print(f"ARCount: {arcount}")

    # Analizar la sección de preguntas
    offset = 12
    for _ in range(qdcount):
        qname, qtype, qclass = parse_dns_question(packet, offset)
        print(f"Question: {qname} - Type: {qtype} - Class: {qclass}")
        offset += len(qname) + 4

    # Analizar la sección de respuestas
    for _ in range(ancount):
        answer, offset = parse_dns_answer(packet, offset)
        print(f"Answer: {answer}")

def parse_dns_question(packet, offset):
    qname = ""
    while True:
        length = packet[offset]
        if length == 0:
            break
        if len(qname) > 0:
            qname += "."
        qname += packet[offset + 1:offset + 1 + length].decode()
        offset += length + 1
    qtype, qclass = struct.unpack('!HH', packet[offset:offset + 4])
    return qname, qtype, qclass



def dns_interceptor():
    # Crea un socket UDP
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind(("0.0.0.0", 53))  # Escucha en todas las interfaces en el puerto 53
    
    print("DNS Interceptor en ejecución...")
    
    while True:
        # Recibe los paquetes DNS
        packet, addr = udp_socket.recvfrom(4096)
        
        # Procesa el paquete DNS
        # lógica para examinar el paquete DNS según el RFC2929
        # y realizar las modificaciones necesarias        
        print("Paquete DNS de: ",addr)
        parse_dns_packet(packet)

        
        response = {
            "Name": "www.gel.com",
            "Type": 1,
            "Class": 1,
            "TTL": 9000,
            "RDLength": 4,
            "RData": '127.0.3.3'
        }


        # Empaquetar los datos de la respuesta DNS
        name = b'\xC0\x0C'
        rdata = socket.inet_aton(response['RData'])
        response_data = struct.pack('!H', response['Name']) +        struct.pack('!H', response['Type']) +        struct.pack('!H', response['Class']) +        struct.pack('!H', response['TTL']) +        struct.pack('!H', response['RDLength']) +        rdata


        udp_socket.sendto(response_data, addr)
    
        # Cierra el socket
    udp_socket.close()


if __name__ == "__main__":
    dns_interceptor()
