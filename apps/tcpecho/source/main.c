#include <feos.h>
#include <stdio.h>
#include <string.h>

#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

const unsigned short port = 7; /* port to listen on */

int main(int argc, const char* argv[])
{
	if (!Wifi_Startup()) {
		fprintf(stderr, "Can't init Wifi!\n");
		return 1;
	}

	// Listening & accepting a connection
	struct sockaddr_in local_addr, peer_addr;
	int addrlen = sizeof(struct sockaddr_in);

	memset(&local_addr, 0, addrlen);
	memset(&peer_addr, 0, addrlen);

	local_addr.sin_port = htons(port);
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	bind(listen_socket, (struct sockaddr *) &local_addr, addrlen);
	listen(listen_socket, 5);

	int client_socket;
	while ((client_socket = accept(listen_socket, (struct sockaddr *) &peer_addr, &addrlen)) != -1) {
		char buf[1000];
		int len = recv(client_socket, buf, sizeof(buf), 0);
		fprintf(stderr, "recv(%d):%d\n", sizeof(buf), len);
		if (len == -1) break;
		int offset = 0;
		while (offset < len) {
			int len_sent = send(client_socket, buf + offset, len - offset, 0);
			fprintf(stderr, "send(%d, %d):%d\n", buf + offset, len - offset, len_sent);
			if (len_sent == -1) break;
			offset += len_sent;
		}
	}

        closesocket(client_socket);
        closesocket(listen_socket);

	Wifi_Cleanup();

	return 0;
}
