#include "TCPServer.h"

TCPServer::TCPServer()
{

}

TCPServer::~TCPServer()
{

}

bool TCPServer::init(const char* port)
{
	int rv;
	m_Port = port;

	FD_ZERO(&m_Master);    // clear the master and temp sets
	FD_ZERO(&m_ReadFDs);

	memset(&m_Hints, 0, sizeof m_Hints);
	m_Hints.ai_family = AF_UNSPEC;
	m_Hints.ai_socktype = SOCK_STREAM;
	m_Hints.ai_flags = AI_PASSIVE;
	m_Hints.ai_protocol = IPPROTO_TCP;

	if ((rv = getaddrinfo(NULL, m_Port, &m_Hints, &m_Result)) != 0) {
		fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
		exit(1);
	}

	for (m_Ptr = m_Result; m_Ptr != NULL; m_Ptr = m_Ptr->ai_next) 
	{
		m_Listener = socket(m_Ptr->ai_family, m_Ptr->ai_socktype, m_Ptr->ai_protocol);
		if (m_Listener < 0) {
			continue;
		}

		// lose the pesky "address already in use" error message
		int yes = 1;
		setsockopt(m_Listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		if (bind(m_Listener, m_Ptr->ai_addr, m_Ptr->ai_addrlen) < 0) {
			close(m_Listener);
			perror("listener : bind");
			continue;
		}

		break;
	}

	// if we got here, it means we didn't get bound
	if (m_Ptr == NULL) {
		fprintf(stderr, "selectserver: failed to bind\n");
		exit(2);
	}

	freeaddrinfo(m_Result); // all done with this

					  // listen
	if (listen(m_Listener, 10) == -1) {
		perror("listen");
		exit(3);
	}

	// add the listener to the master set
	FD_SET(m_Listener, &m_Master);

	// keep track of the biggest file descriptor
	m_FDmax = m_Listener; // so far, it's this one

}

void TCPServer::acceptLoop()
{
	char buf[100];
	int nbytes;
	char remoteIP[INET6_ADDRSTRLEN];
	int newfd;        // newly accept()ed socket descriptor
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;
	int i, j;

	for (;;) 
	{
		std::cout << "accept loop" << std::endl;
		m_ReadFDs = m_Master; // copy it
		std::cout << m_FDmax << " Fds" << std::endl;
		int count = 0;
		if (count = select(m_FDmax + 1, &m_ReadFDs, NULL, NULL, NULL) == -1) 
		{
			std::cout << "select broke" << std::endl;
		}
		std::cout << "select" << count << std::endl;
		std::cout << "select check" << std::endl;
		// run through the existing connections looking for data to read
		for (i = 0; i <= m_FDmax; i++)
		{
			std::cout << "fd list" << std::endl;
			if (FD_ISSET(i, &m_ReadFDs))
			{ // we got one!!
				if (i == m_Listener)
				{
					// handle new connections
					addrlen = sizeof remoteaddr;
					newfd = accept(m_Listener,
						(struct sockaddr *)&remoteaddr,
						&addrlen);

					if (newfd == -1)
					{
						perror("accept");
					} 
					else 
					{
						FD_SET(newfd, &m_Master); // add to master set
						if (newfd > m_FDmax) 
						{    // keep track of the max
							m_FDmax = newfd;
						}
						printf("selectserver: new connection from %s on "
							"socket %d\n",
							inet_ntop(remoteaddr.ss_family,
								get_in_addr((struct sockaddr*)&remoteaddr),
								remoteIP, INET6_ADDRSTRLEN),
							newfd);
					}
				}
				else 
				{
					// handle data from a client
					if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
					{
						// got error or connection closed by client
						if (nbytes == 0) 
						{
							// connection closed
							printf("selectserver: socket %d hung up\n", i);
						}
						else 
						{
							perror("recv");
						}
						close(i); // bye!
						FD_CLR(i, &m_Master); // remove from master set
					}
					else 
					{
						// we got some data from a client
						for (j = 0; j <= m_FDmax; j++) 
						{
							// send to everyone!
							if (FD_ISSET(j, &m_Master))
							{
								// except the listener and ourselves
								if (j != m_Listener && j != i) 
								{
									if (::send(j, buf, nbytes, 0) == -1) 
									{
										perror("send");
									}
								}
							}
						}
					}
				} // END handle data from client
			} // END got new incoming connection
		} // END looping through file descriptors
		std::cout << "Loop check" << std::endl;
	} 
}

bool TCPServer::send(char* data, int msglen, int fd)
{

}