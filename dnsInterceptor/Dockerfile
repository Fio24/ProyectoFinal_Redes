FROM gcc:latest 
 
WORKDIR /app 
 
COPY pruebaConexion.c /app 
 
RUN apt-get update 
 
RUN apt-get install -y dnsutils 
 
RUN apt-get install net-tools 
 
RUN apt-get install lsof 
 
RUN apt-get install netcat-openbsd 
 
RUN gcc -o conexion pruebaConexion.c -lcurl

CMD ./conexion