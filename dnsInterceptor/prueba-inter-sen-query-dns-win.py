import dns.resolver
import logging
logger = logging.getLogger('dns.resolver')
logger.setLevel(logging.DEBUG)

# Crea un objeto de resolución de DNS
resolver = dns.resolver.Resolver()

# Establece el servidor DNS al que enviar la consulta (opcional, usa el predeterminado si no se establece)
resolver.nameservers = ['127.0.0.1']
resolver.port = 53  # Establece el puerto 53

# Variable global para almacenar la respuesta DNS
respuesta = None

try:
    # Realiza la consulta DNS
    respuesta = resolver.resolve('example.com', 'A')
except dns.exception.DNSException as e:
    import traceback
    traceback.print_exc()
    print("Error performing DNS query:", e)

# Verifica si la respuesta se obtuvo correctamente
if respuesta:
    # Accede a los bytes de la respuesta DNS
    wire_bytes = respuesta.response.to_wire()
    # Imprime los bytes de la respuesta
    print("Bytes de respuesta:", wire_bytes)
else:
    print("No se pudo obtener la respuesta DNS")
