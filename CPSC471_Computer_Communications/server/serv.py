import os
import sys
import socket
import json


def main():
    if len(sys.argv) < 2:
        print("Usage: python serv.py <PORTNUMBER>")
        return

    local_server = "127.0.0.1"
    BUFFER_SIZE = 1024  # Standard size
    FORMAT = "utf-8"
    port = int(sys.argv[1])

    serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serverSocket.bind((local_server, port))

    print("\nThe Server has been successfully invoked.\n")
    print("The Server is listening on port", port, ".\n")
    print("To continue, connect a client.")

    serverSocket.listen()
    controlSocket, addr = serverSocket.accept()
    print("\nConnected to by address: {}".format(addr))

    def quit():
        try:
            controlSocket.send("quit command received".encode())
            controlSocket.close()  # close the connection
            print("Conection Closed")
            print("QUIT SUCCESSFUL")
            return
        except Exception as e:
            print(e)
            print("QUIT UNSUCCESSFUL")
            return

    def get():
        tempSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        tempSocket.bind((local_server, 0))
        tempSocket.listen()
        data_port = tempSocket.getsockname()[1]

        ack = "ACK get " + str(data_port)
        controlSocket.send(ack.encode(FORMAT))
        print(ack)

        tempConn, addr = tempSocket.accept()

        # write a line of code to receive the filename from the client
        file_name = tempConn.recv(BUFFER_SIZE).decode(FORMAT)
        print("File name received:", file_name)

        if not os.path.exists(file_name):
            print("Error: file does not exist")
            tempConn.send("ERROR: file does not exist".encode(FORMAT))
            print("GET FAILURE")
            return

        with open(file_name, "rb") as fileObj:
            while True:
                fileData = fileObj.read(65536)
                if fileData:
                    dataSizeStr = str(len(fileData))

                    while len(dataSizeStr) < 10:
                        dataSizeStr = f"0{dataSizeStr}"

                    fileData = dataSizeStr.encode(FORMAT) + fileData

                    numSent = 0

                    while len(fileData) > numSent:
                        numSent += tempConn.send(fileData[numSent:])

                    print("Sent ", numSent, " bytes.")
                    print("GET SUCCESSFUL")
                else:
                    break
        tempSocket.close()
        return

    def put():
        tempSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        tempSocket.bind((local_server, 0))
        tempSocket.listen()
        data_port = tempSocket.getsockname()[1]
        # get server go-ahead to receive file name
        ack = "ACK put " + str(data_port)
        controlSocket.send(ack.encode(FORMAT))
        print(ack)

        tempConn = tempSocket.accept()[0]

        # get file name from client
        file_name = tempConn.recv(BUFFER_SIZE).decode(FORMAT)
        print("File name received:", file_name)

        # send ack
        tempConn.send("ACK".encode(FORMAT))

        # get file size from client
        file_size = int(tempConn.recv(BUFFER_SIZE).decode(FORMAT))
        print("File size received:", file_size)

        # send ack for file contents
        tempConn.send("ACK".encode(FORMAT))

        bytes_received = 0
        try:
            with open(file_name, "wb") as fileObj:
                while bytes_received < file_size:
                    data = tempConn.recv(BUFFER_SIZE)
                    fileObj.write(data)
                    bytes_received += len(data)
                    print("Received ", bytes_received, " bytes.")
            fileObj.close()
            tempSocket.close()
            print("PUT SUCCESSFUL")
            return
        except Exception as e:
            print(e)
            tempSocket.close()
            print("PUT UNSUCCESSFUL")
            return

    def ls():
        try:
            list = json.dumps(os.listdir()).encode('utf-8')

            tempSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            # tempSocket.bind((local_server, port+1))
            tempSocket.bind((local_server, 0))
            tempSocket.listen()
            data_port = tempSocket.getsockname()[1]

            lengthOfList = len(list)
            ack = "ACK ls " + str(lengthOfList) + " " + str(data_port)
            controlSocket.send(ack.encode('utf-8'))
            print(ack)

            tempConn, addr = tempSocket.accept()

            if (lengthOfList <= BUFFER_SIZE):
                tempConn.send(list)
            else:
                numBatches = int(lengthOfList / BUFFER_SIZE)
                numBatchesLeftovers = lengthOfList % BUFFER_SIZE
                for i in range(numBatches):
                    batch = list[BUFFER_SIZE * i: BUFFER_SIZE * (i+1)]
                    tempConn.send(batch)

                if (numBatchesLeftovers > 0):
                    batch = list[BUFFER_SIZE *
                                 numBatches:((BUFFER_SIZE * numBatches) + numBatchesLeftovers)]
                    tempConn.send(batch)

            tempSocket.close()
            print("LS SUCCESSFUL")
            return
        except Exception as e:
            print(e)
            tempSocket.close()
            print("LS UNSUCCESSFUL")
            return

    while True:

        # Enter into a while loop to recieve commands from client
        print("\n\nWaiting for instruction")

        output = controlSocket.recv(BUFFER_SIZE).decode(FORMAT)

        if not output:
            break
        if "/" in output:
            output = output.split("/")[0].strip()

        print("\nReceived instruction: {}".format(output))

        print("Accepted connection from client: ", addr, "\n")

        # Check the command and respond correctly
        if output == "ls":
            ls()

        elif output == "get":
            get()

        elif output == "put":
            put()

        elif output == "quit":
            quit()
            break

        else:
            output = None


if __name__ == "__main__":
    main()
