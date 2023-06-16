FROM gcc:latest

WORKDIR /app

COPY interceptor.c /app

COPY live.c /app

RUN apt-get update

RUN gcc -o interceptor interceptor.c

RUN gcc -o live live.c

RUN apt-get install -y dnsutils

RUN apt-get install net-tools

RUN apt-get install lsof

RUN apt-get install netcat-openbsd

CMD ./live

# echo -n "Hola, esto es un paquete DNS" | nc -u localhost 53
# echo Hola, esto es un paquete DNS | nslookup -port=53 127.0.0.1
#

# echo -n "example.com" | nc -u localhost 53