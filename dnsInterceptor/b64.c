#include <stdio.h>
#include <stdlib.h>
#include <b64/cencode.h>
#include <b64/cdecode.h>

// Función para codificar datos en Base64
char* base64_encode(const unsigned char* data, size_t input_length, size_t* output_length) {
    base64_encodestate state;
    size_t encode_length = 2 * input_length;  // Estimación conservadora del tamaño de salida

    char* encoded_data = (char*)malloc(encode_length);
    base64_init_encodestate(&state);
    *output_length = base64_encode_block((const char*)data, input_length, encoded_data, &state);
    *output_length += base64_encode_blockend(encoded_data + *output_length, &state);

    return encoded_data;
}

// Función para decodificar datos en Base64
unsigned char* base64_decode(const char* encoded_data, size_t input_length, size_t* output_length) {
    base64_decodestate state;
    size_t decode_length = input_length;  // Estimación  del tamaño de salida

    unsigned char* decoded_data = (unsigned char*)malloc(decode_length);
    base64_init_decodestate(&state);
    *output_length = base64_decode_block(encoded_data, input_length, (char*)decoded_data, &state);

    return decoded_data;
}

int main() {
    const unsigned char input_data[] = "Hello, World!";
    size_t input_length = sizeof(input_data) - 1;  // Se excluye el carácter nulo final

    // Codifica datos en Base64
    size_t encoded_length;
    char* encoded_data = base64_encode(input_data, input_length, &encoded_length);
    printf("Datos codificados en Base64: %s\n", encoded_data);

    // Decodifica datos desde Base64
    size_t decoded_length;
    unsigned char* decoded_data = base64_decode(encoded_data, encoded_length, &decoded_length);
    printf("Datos decodificados: %.*s\n", (int)decoded_length, decoded_data);

    // Libera memoria
    free(encoded_data);
    free(decoded_data);

    return 0;
}
