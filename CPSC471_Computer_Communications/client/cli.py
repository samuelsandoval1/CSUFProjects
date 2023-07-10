import os
import sys
import json
import socket

# grab server name and server port from user input
if len(sys.argv) != 3:
    print("Usage: cli.py <server machine> <server port>")
    sys.exit(1)

# variables
server_name = sys.argv[1]
server_port = int(sys.argv[2])
BUFFER_SIZE = 1024  # Standard chioce
FORMAT = "utf-8"
ADDR = (server_name, server_port)
controlSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


try:
    controlSocket.connect(ADDR)
except socket.gaierror:
    print("Error: Could not resolve hostname")
    sys.exit(1)

print(f"\nConnecting to {server_name}:{server_port}\n")
print("\nConnecting successfull\n")
print("\nAvailable functions: \nget <filename> : downloads file <filename> from the server\nput <filename> : uploads file <file name> to the server\nls : lists files on the server\nquit : disconnects from the server and exits\n")


def quit():
    controlSocket.send("quit".encode())
    response = controlSocket.recv(BUFFER_SIZE)
    print(response.decode())
    controlSocket.close()
    print("Connection Closed")
    return


def get(file_name):  # assume file name is local directory
    controlSocket.send("get".encode(FORMAT))
    response = controlSocket.recv(
        BUFFER_SIZE).split()  # Wait for server go-ahead

    typeOfResponse = str(response[0].decode(FORMAT))
    typeOfRequest = str(response[1].decode(FORMAT))
    transferPort = int(response[2].decode(FORMAT))

    if (typeOfResponse != "ACK" or typeOfRequest != "get"):
        # open new port for transfer otherwise
        print("Server had an issue, please try again later")
        return

    tempSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        tempSocket.connect((server_name, transferPort))
    except socket.gaierror:
        print("Error: Could not resolve hostname")
        sys.exit(1)

    tempSocket.send(file_name.encode(FORMAT))

    with open("downloaded_file.txt", "wb") as output_file:
        bytes_recieved = 0
        print("\nDownloading...")
        while bytes_recieved < BUFFER_SIZE:
            response = tempSocket.recv(BUFFER_SIZE).decode(FORMAT)
            if response.startswith("ERROR:"):
                print(response)
                return
            output_file.write(response[10:].encode(FORMAT))
            bytes_recieved += BUFFER_SIZE
    output_file.close()
    tempSocket.close()
    return


def put(file_name):
    print("file name =", file_name)

    # Send server request for put
    controlSocket.send("put".encode(FORMAT))
    # Wait for server go-ahead
    response = controlSocket.recv(BUFFER_SIZE).split()
    typeOfResponse = str(response[0].decode(FORMAT))
    typeOfRequest = str(response[1].decode(FORMAT))
    transferPort = int(response[2].decode(FORMAT))

    # check if server responded to put request
    if (typeOfResponse != "ACK" or typeOfRequest != "put"):
        print("Server had an issue with put, please try again later")

    # create and connect to ephemeral port for transfer
    tempSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        tempSocket.connect((server_name, transferPort))
    except socket.gaierror:
        print("Error: Could not resolve hostname")
        sys.exit(1)

    # check if file exists
    if not os.path.exists(file_name):
        print("Error: file does not exist")
    else:
        print("File exists")

    # send file name
    tempSocket.send(file_name.encode(FORMAT))
    print("sending file name")

    tempSocket.recv(BUFFER_SIZE).decode(FORMAT)

    # send file size
    file_size = os.path.getsize(file_name)
    tempSocket.send(str(file_size).encode(FORMAT))

    # receive server ack for file contents
    tempSocket.recv(BUFFER_SIZE).decode(FORMAT)

    # send file contents
    with open(file_name, "rb") as fileObj:
        bytes_read = fileObj.read(BUFFER_SIZE)
        while bytes_read:
            tempSocket.send(bytes_read)
            bytes_read = fileObj.read(BUFFER_SIZE)
    # 'print number of bytes sent
    print("Sent", file_size, "bytes")
    print("put complete")
    pass


def ls():
    controlSocket.send("ls".encode('utf-8'))

    response = controlSocket.recv(BUFFER_SIZE).split()

    typeOfResponse = str(response[0].decode(FORMAT))
    typeOfRequest = str(response[1].decode(FORMAT))
    sizeOfResponse = int(response[2].decode(FORMAT))
    transferPort = int(response[3].decode(FORMAT))

    if (typeOfResponse != "ACK" or typeOfRequest != "ls"):
        # open new port for transfer otherwise
        print("Server had an issue, please try again later")
        return

    # connect to new socket for transfer
    tempSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        tempSocket.connect((server_name, transferPort))
    except socket.gaierror:
        print("Error: Could not resolve hostname")
        sys.exit(1)

    # parse the batches
    fileNames = ""
    numBatches = int(sizeOfResponse / BUFFER_SIZE)
    numBatchesLeftovers = sizeOfResponse % BUFFER_SIZE
    for i in range(numBatches):
        fileNames += str(tempSocket.recv(BUFFER_SIZE).decode(FORMAT))

    if (numBatchesLeftovers > 0):
        fileNames += str(tempSocket.recv(numBatchesLeftovers).decode(FORMAT))

    # close temp socket
    tempSocket.close()

    parsedNames = json.loads(fileNames)
    for name in parsedNames:
        if name == "serv.py":
            continue
        print("\t" + name)
    return


while True:
    user_input = input("ftp> ")
    if user_input.startswith("get"):
        get(user_input.split()[1])
    elif user_input.startswith("put"):
        put(user_input.split()[1])
    elif user_input.startswith("ls"):
        ls()
    elif user_input.startswith("quit"):
        quit()
        break
    else:
        print("Invalid command")
