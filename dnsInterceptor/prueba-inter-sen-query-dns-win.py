import dns.resolver

# Crea un objeto de resolución de DNS
resolver = dns.resolver.Resolver()

# Establece el servidor DNS al que enviar la consulta (opcional, usa el predeterminado si no se establece)
resolver.nameservers = ['127.0.0.1']
resolver.port = 53  # Establece el puerto 53

# Realiza la consulta DNS
respuesta = resolver.resolve('example.com', 'A')

# Itera sobre los registros de respuesta y muestra la información
for registro in respuesta:
    print(respuesta)
