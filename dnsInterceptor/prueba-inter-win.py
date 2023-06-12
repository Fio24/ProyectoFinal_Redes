import socket

def dns_interceptor():
    # Crea un socket UDP
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind(("0.0.0.0", 53))  # Escucha en todas las interfaces en el puerto 53
    
    print("DNS Interceptor en ejecución...")
    
    while True:
        # Recibe los paquetes DNS
        data, addr = udp_socket.recvfrom(4096)
        
        # Procesa el paquete DNS
        # lógica para examinar el paquete DNS según el RFC2929
        # y realizar las modificaciones necesarias        
        print("Paquete DNS recibido de {}: {}".format(addr, data))
        
        response_data = "GG \n".encode('utf-8')
       
        udp_socket.sendto(response_data, addr)
    
    # Cierra el socket
    udp_socket.close()


if __name__ == "__main__":
    dns_interceptor()
