# TCP_Reverse_Proxy
Allows a server or other device on a private network to be made publicly accessible via a reverse proxy using TCP  
The serverside code is run on a cheap, publicly accessible server and accepts connections from both the clientside software and outside clients.  
The clientside code is run on the machine hosting the user's server on the private network.  
The clientside code connects out through the firewall to the serverside code on the public server, establishing a connection or tunnel through which outside traffic can reach the user's server.  
The code should be compilable and runnable on Linux. It has also been compiled and run on Windows using gcc from MSYS2.  
Currently, only one outside connection in through the reverse proxy is supported.  
