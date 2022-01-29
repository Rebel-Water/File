#include<stdio.h>
#include<sys/type.h>
#include<sys/socket.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>

#define MESSAGE_SIZE 102400

void send_data(int sockfd) {
	char *query;
	query = (char*)malloc(MESSAGE_SIZE + 1);
	memset(query, 'a', MESSAGE_SIZE);
	query[MESSAGE_SIZE] = '\0';

	const char *cp;
	cp = query;
	size_t remaining = strlen(query);
	while(remaining) {
		int n_written = send(sockfd, cp, remaining, 0);
		fprintf(stdout, "send into buffer %ld \n", n_written);
		if(n_written <= 0) {
			error(1, errno, "send failed");
			return ;
		}
		remaining -= n_written;
		cp += n_written;
	}
}

int main(int argc, char** argv) {
	int sockfd;
	struct sockaddr_in servaddr;
	if(argc != 2) 
		error(1, 0, "usage: tcpclient <IPaddress>");

	sockfd = socket(AF_INET, SOCK_STREAM, 0); // 获取socket 句柄

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(12345); // host to net short
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	// 将点分十进制转换成整数	 
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		error(1, errno, "connect failed ");
	send_data(sockfd);
	exit(0);
}