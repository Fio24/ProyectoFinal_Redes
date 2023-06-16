#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

int main(void) {
    CURL *curl;
    CURLcode res;

    // Inicializar la biblioteca libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Crear una instancia de CURL
    curl = curl_easy_init();
    if (curl) {
        // Establecer la URL del API en el contenedor utilizando la dirección IP
        curl_easy_setopt(curl, CURLOPT_URL, "http://172.17.0.2:5000/api/dns_resolver?url=www.ucr.ac.cr&src=192.168.0.1");

        // // Establecer la solicitud como POST
        // curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // // Establecer los datos de la solicitud POST
        // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "url=www.uni.com&document={\"servers\": [\"10.0.0.1\", \"10.0.0.2\"], \"reg_type\": \"multi\"}");



        // Realizar la solicitud HTTP GET
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Error en la solicitud: %s\n", curl_easy_strerror(res));
        }

        // Limpiar y liberar los recursos
        curl_easy_cleanup(curl);
    }

    // Finalizar la biblioteca libcurl
    curl_global_cleanup();

    return 0;
}
