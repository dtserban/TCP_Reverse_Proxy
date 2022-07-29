# TCP_Reverse_Proxy
Allows a server or other device on a private network to be made publicly accessible via a reverse proxy using a TCP tunnel  
The serverside code `trps.c` is run on a cheap, publicly accessible server and accepts connections from both the clientside code `trpc.c` and outside clients wanting to connect in to the private server.  
The clientside code `trpc.c` is run on the machine hosting the user's server that is located on the private network.  
The clientside code connects out through the firewall of the private network to the serverside code on the public server, establishing a connection or tunnel through which outside traffic can be sent through and reach the user's previously inaccessible server.  
The code should be compilable and runnable on Linux.  It has also been compiled and run on Windows using gcc from MSYS2.  
Currently, only one outside connection in through the reverse proxy is supported and the port numbers are hardcoded.  
The clientside code `trpc.c` takes in as a command line argument the IP address of the cheap publicly accessible server that is running the serverside code `trps.c`.
