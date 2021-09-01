#include "us_xfr.h"

void errExit(char *str)
{
	int errvar = errno;
	printf("Error in : %s\n",str);
	printf("%s\n",strerror(errno));
	errno = errvar;
	fflush(stdout);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	struct sockaddr_un addr;
	int sfd;
	ssize_t numRead;
	char buf[BUF_SIZE];

	sfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(sfd == -1)
		errExit("socket");

	/* Construct address struct and make connection */
	
	memset(&addr,0,sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)- 1);
	
	if(connect(sfd,(struct sockaddr *)&addr,sizeof(struct sockaddr_un)) == -1)
		errExit("connect");

	/* Copy stdin to socket */

	while((numRead = read(STDIN_FILENO,buf,BUF_SIZE)) > 0)
			if(write(sfd,buf,numRead) != numRead)
				errExit("partial/failed write");
				
	if(numRead == -1)
		errExit("read");

	if(close(sfd) == -1)
		errExit("close");

	exit(EXIT_SUCCESS);
}
