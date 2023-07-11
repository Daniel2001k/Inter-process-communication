#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <sys/wait.h>

static const char *fifoServerPath = "/tmp/FIFO-TMP-SERWER";

int fifoServerRead;
int fifoServerWrite;

void handleSignals(int i)
{
	close(fifoServerRead);
	close(fifoServerWrite);

	unlink(fifoServerPath);

	exit(0);
}

int main()
{
	int clientRead;
	int clientWrite;

	char buffer[1000];
	char *pointer = buffer;

	pid_t clientPID;
	pid_t forkedProcessID;

	signal(SIGTERM, handleSignals);
	signal(SIGINT, handleSignals);

	mknod(fifoServerPath, S_IFIFO | 0600, 0);

	fifoServerRead = open(fifoServerPath, O_RDONLY);
	fifoServerWrite = open(fifoServerPath, O_WRONLY);

	for (;;)
	{
		read(fifoServerRead, &clientPID, sizeof(pid_t));

		forkedProcessID = fork();

		if (forkedProcessID == 0)
		{
			strcpy(pointer, "/tmp/FIFO-TMP-2-");
			pointer += strlen(pointer);
			sprintf(pointer, "%d", (int)clientPID);

			clientRead = open(buffer, O_RDONLY);

			*(pointer - 2) = '1';

			if ((clientWrite = open(buffer, O_WRONLY)) != -1)
			{
				double a, b;

				read(clientRead, &a, sizeof(double));
				printf("Otrzymana wiadomość %lf:\n", a);

				b = a * a;

				write(clientWrite, &b, sizeof(double));
			}

			close(clientRead);
			close(clientWrite);
			close(fifoServerRead);
			return 0;
		}
		else
		{
			wait(NULL);
		}
	}
}