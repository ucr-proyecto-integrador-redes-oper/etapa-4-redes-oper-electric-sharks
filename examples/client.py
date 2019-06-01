import socket

UDP_PORT = 8080

ip = str(input("Ingrese la Ipv4 del destino: "))


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
while True:
	try:
		message = str(input("> "))
	except KeyboardInterrupt:
		break
	sock.sendto(message.encode(), (ip, UDP_PORT))
