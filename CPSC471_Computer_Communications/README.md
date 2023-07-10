# CPSC 471 Project

## About
This is a socket programming project for CPSC 471 Computer Communications. The project is a simple FTP server and client. The server and client communicate over TCP. The server can handle multiple clients at once. The client can get/put files from the server. The server can list files in its directory.

## Programming Language 
Python 3

## Executing Instructions
### Start the server

Start the server.py file from the command line with the port you want to listen on

ex. ```python serv.py 5001```

### Start the client

Start the client.py file from the command line with server and the port to connect to

ex. ```python cli.py 127.0.0.1 5001```

Run the client commands

### On the client side, run the commands. Examples:

```ftp> ls``` 

```ftp> get todo.txt ```

```ftp> put groceries.txt```

```ftp> quit```

## FTP Design
- Can only get/put ```.txt ``` files
- ```put``` will search ```client``` directory for file
- ```get``` will search ```server``` directory for file
- ```get``` will download contents of file in ```downloaded_file.txt```
- Server/client respond with print statements to commands
   
    ex 1. ```serv.py``` responding to put

    ```Waiting for instruction

    Received instruction: put
    Accepted connection from client:  ('127.0.0.1', 65020) 

    ACK put 65079
    File name received: downloaded_file.txt
    File size received: 43
    Received  43  bytes.
    PUT SUCCESSFUL 
    ```

    ex 2. Initial handshake establishes connection socket
    
    ```serv.py```
    ```
    The Server has been successfully invoked.

    The Server is listening on port 5001 .

    To continue, connect a client.

    Connected to by address: ('127.0.0.1', 65020)


    Waiting for instruction
    ```
    ```cli.py```
    ```
    Connecting to 127.0.0.1:5001


    Connecting successfull


    Available functions: 
    get <filename> : downloads file <filename> from the server
    put <filename> : uploads file <file name> to the server
    ls : lists files on the server
    quit : disconnects from the server and exits
    ```
- Looping used to prevent buffer overflow 
- Communication to set up transport socket
    ```
    tempSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            tempSocket.connect((server_name, transferPort))
        except socket.gaierror:
            print("Error: Could not resolve hostname")
            sys.exit(1)

        tempSocket.send(file_name.encode(FORMAT))
    ```



