Un container que ejecuta un REST API implementado en Python, que ejecuta los
métodos necesarios para implementar la aplicación, al menos es necesario un método
(/api/dns_resolver) con verbo HTTP (POST), este método recibe en el data un paquete DNS
(RFC2929) codificado en BASE64, el mismo deberá ser decodificado y enviado a un servidor DNS
remoto para su resolución, esto implica lo siguiente:
- Esta aplicación deberá implementar un REST API (pueden usar cualquier framework) pero también deberá implementar un cliente UDP/DNS para enviar las solicitudes hacia el servidor DNS remoto.
- Esta aplicación deberá recibir como parámetro/archivo de configuración el IP de un servidor DNS remoto.
- Esta aplicación deberá soportar múltiples peticiones al mismo tiempo.
Cualquier otro método requerido en el API, deberá ser definido por cada grupo. Toda
información nativa de DNS (paquetes) que se intercambian entre el DNS Interceptor y el DNS
API se debe codificar en BASE64.