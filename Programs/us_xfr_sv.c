#include "us_xfr.h"

#define BACKLOG 5

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
	int sfd, cfd;
	ssize_t numRead;
	char buf[BUF_SIZE];

	sfd = socket(AF_UNIX, SOCK_STREAM,0);
	if(sfd==-1)
		errExit("socket()");

	/* 
	 * 	Checking if file name is not too long
	 * 	Removing file if available already. Even after closing socket, socket file doen't
	 *	gets removed unless all links to the file are removed.
	 */

	if(strlen(SV_SOCK_PATH) > sizeof(addr.sun_path)-1)
		errExit("Sock file name too long!\n");

	if(remove(SV_SOCK_PATH) == -1 && errno!= ENOENT)
		errExit("remove");

	/*
	 * 	Now ZERO-ing out memory to prevent overlapping by strncpy and
	 * 	possible memory leak.
	 */

	memset(&addr,0,sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path,SV_SOCK_PATH,sizeof(addr.sun_path)-1);

	if(bind(sfd,(struct sockaddr *)&addr,sizeof(struct sockaddr_un)) == -1)
		errExit("bind");
	
	if(listen(sfd,BACKLOG) == -1)
		errExit("listen");

	for(;;){	/* Handle client connections iteratively */

		/* 
		 * 	Accept a connection and return it to cfd, so that sfd remains "open"
		 *	 and can listen to subsequent connections
		 */
		
		cfd = accept(sfd,NULL,NULL);
		if(cfd == -1)	
			errExit("accept");
		
		while((numRead = read(cfd,buf,BUF_SIZE))>0)
			if(write(STDOUT_FILENO,buf,numRead)!=numRead)
				errExit("partial/failed write");

		if(numRead == -1)
			errExit("read");

		if(close(cfd) == -1)
			errExit("close");
	}
	exit(EXIT_SUCCESS);
}
