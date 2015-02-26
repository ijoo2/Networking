Inha Joo ijoo
HW1 CS450
README.

CLIENT INSTRUCTIONS.

myClient [ADDRESS] [PORT]

example:

./myClient 127.0.0.1 12345


The client will send a file to the server. For now, the file name is test.txt
I have not utilized the header given by the professor. This is a TODO.
the address is defaulted to 127.0.0.1 in the server

SERVER INSTRUCTIONS

myServer [PORT]

example:
./myServer 12345

The server will receive the file from the client and save it as "final"
the address is defaulted to 127.0.0.1 and is not necessary to use the command line for it.

the server will bind TCP port to address 127.0.0.1.
the server will listen to the port given. Then when the client is run, the server will receive the file
and continue listening to the port.
