#!/bin/bash

# Obtener el directorio del script
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Cambiar al directorio del script
cd "$SCRIPT_DIR"
#Instala el api
cd ./dnsapi
docker build -t flask-dns-api .
docker run -d -p 5000:5000 flask-dns-api         
cd ..
pwd

#Instala y ejecuta el UI
cd ./UI/DNS_2.0_UI
docker build -t dns_ui .
docker run -d -it -p 80:80 dns_ui
cd ..
cd ..
pwd

#Instala y ejecuta el interceptor
cd ./dnsInterceptor
docker-compose up
cd ..
pwd

sleep 60
