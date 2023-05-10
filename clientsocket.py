import socket
import struct

SERVER_IP = '127.0.0.1' 
SERVER_PORT = 9999
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
integer_value = 20

while True:
    message = input('Enter inter value to move: ')
    try:
        integer_value = int(message)
        message_bytes = struct.pack('!i', integer_value)

        s.sendto(message_bytes, (SERVER_IP, SERVER_PORT))
    except ValueError:
        print('Invalid  value')