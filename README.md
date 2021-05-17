# programming-assignment-1-stickrod-jaclyn-savage-toby
programming-assignment-1-stickrod-jaclyn-savage-toby created by GitHub Classroom

Members:
- Jaclyn Stickrod, jast3573@colorado.edu
- Toby Savage, tosa5156@colorado.edu

Directory Structure/Files
- Client
    - Makefile
      - make file for udp_client.c, run make to create executable called client, and make clean to delete the executable
    - udp_client.c
      - client c program to run use ./client [server IP] [port #]
    - Commands
      -  'put [filename]': Put entered file from the client to the server.
      -  'get [filename]': Get entered file from the server to the client.
      -  'delete [filename]': Deletes the file entered in the server's directory
      -  'ls': Displays a list of the files within the server
      -  'exit': Exits the program
    - Additional Notes:
      - Client sends multiple packets to the server. First packet contains the command, and filename depending on the command. The remaining packets are as follows:
            - 'put': Remaining packets are sent with contents of the file until all contents have been sent to the server.
            - 'get': Remaining packets are sent with contents of the file until all contents have been sent to the client.
            - 'delete': Server sends confirming message packet to confirm deletion of file.
            - 'ls': Server sends packets of file names in server directory to the client. 
      - Each command sends a series of packets either from the server to the client
    
- Server
    - Makefile
      - make file for udp_server.c, run make to create executable called server, and make clean to delete the executable
    - udp_server.c
      - server c program to run use ./server [port #]
    - the following are files used to test transfer
      - foo1
      - foo2
      - foo3
    - The server contains the directory of files that are either put, retrieved, deleted, or displayed per the client's request.
      
