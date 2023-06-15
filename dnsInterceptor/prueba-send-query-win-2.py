import socket
import binascii

def send_dns_query(hostname, port):
    # Crea un socket UDP
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    # Conecta el socket al servidor DNS en la dirección IP y puerto especificados
    server_address = ('127.0.0.1', port)  # Reemplaza con la dirección IP adecuada
    udp_socket.connect(server_address)
    
    # Construye la consulta DNS en formato hexadecimal
    query_id = b'\x00\x01'  # ID de consulta
    query_flags = b'\x00\x00'  # Flags
    query_qdcount = b'\x00\x01'  # QDCOUNT
    query_ancount = b'\x00\x00'  # ANCOUNT
    query_nscount = b'\x00\x00'  # NSCOUNT
    query_arcount = b'\x00\x00'  # ARCOUNT
    query_data = hostname.encode('utf-8') + b'\x00' + b'\x00\x01' + b'\x00\x01'  # Nombre, Tipo y Clase
    query = query_id + query_flags + query_qdcount + query_ancount + query_nscount + query_arcount + query_data
    
    # Envía la consulta DNS
    udp_socket.send(query)
    
    # Recibe la respuesta
    response, _ = udp_socket.recvfrom(4096)
    response_data = binascii.hexlify(response).decode('utf-8')
    
    # Cierra el socket
    udp_socket.close()
    
    return response_data

# Ejemplo de uso
hostname = 'examplesss.com'  # Reemplaza con el nombre de dominio o dirección IP a consultar
port = 53  # Reemplaza con el puerto personalizado configurado en tu programa en Python

response = send_dns_query(hostname, port)
print(response)



01000080010000010000000004000000900a1a0000000000900a1a0000000000900a1a00000000001c000000000000001c00000000000000






