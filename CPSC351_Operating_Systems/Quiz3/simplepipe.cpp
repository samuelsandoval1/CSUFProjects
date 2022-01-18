#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

// The maximum string length
#define MAX_STRING_LENGTH 1000

int main(int argc, char** argv)
{
	// The array to store pipe file descriptors
	int firstPipe[2];
	
	// The string length
	int stringLength;
	
	// The buffer to hold the received string
	char recvStr[MAX_STRING_LENGTH];

	// The variable to store the child's process ID
	pid_t pid;
	
	// Sanity check
	if(argc < 2)
	{
		fprintf(stderr, "USAGE: %s <string>\n", argv[0]);
		exit(1);
	}
	
	// ** Limit the string length **

	// Get the string length + null terminator
	stringLength = strlen(argv[1]) + 1;
	
	// Check the string length
	if(stringLength > MAX_STRING_LENGTH)
	{
		fprintf(stderr, "String too large. Maximum length = %d. Provided string length = %d", MAX_STRING_LENGTH, stringLength);
		exit(1);
	}

	// Create a pipe
	if(pipe(firstPipe) < 0)
	{
		perror("pipe");
		exit(1);
	}
	
	// Create a child
	pid = fork();	
	
	// Sanity check
	if(pid < 0)
	{
		perror("fork");
		exit(1);
	}
	// The parent code
	if(pid > 0)
	{
		// The parent will not be reading this pipe
		close(firstPipe[0]);
		
		
		// Write the string to the child
		if(write(firstPipe[1], argv[1], stringLength) < 0)
		{
			perror("write");
			exit(1);
		}
		
		// Close the no longer needed write-end
		close(firstPipe[1]);
	}
	// The child code
	else 
	{
		// The child will not be writing to this pipe
		close(firstPipe[1]);
		
		// Read the string from the pipe
		if(read(firstPipe[0], recvStr, MAX_STRING_LENGTH) < 0)
		{
			perror("read");
			exit(1);
		}
		

		// Print the string
		fprintf(stderr, "Got string: %s\n", recvStr);
		
		// Close the unused end
		close(firstPipe[0]);
		
	}

	return 0;
	
}
