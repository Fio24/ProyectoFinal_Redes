import socket

def send_dns_query(hostname, port):
    # Crea un socket UDP
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    # Conecta el socket al servidor DNS en la dirección IP y puerto especificados
    server_address = ('127.0.0.1', port)  # Reemplaza con la dirección IP adecuada
    udp_socket.connect(server_address)
    
    # Envía la consulta DNS
    query = hostname.encode('utf-8')
    udp_socket.send(query)
    
    # Recibe la respuesta
    response, _ = udp_socket.recvfrom(4096)
    response_data = response #.decode('utf-8')
    
    # Cierra el socket
    udp_socket.close()
    
    return response_data

# Ejemplo de uso
hostname = 'examplesss.com'  # Reemplaza con el nombre de dominio o dirección IP a consultar
port = 8080  # Reemplaza con el puerto personalizado configurado en tu programa en Python

response = send_dns_query(hostname, port)
print(response)
