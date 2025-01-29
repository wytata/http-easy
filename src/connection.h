#ifndef CONNECTION_H
#define CONNECTION_H

void new_connection_entrypoint(int client_fd); 
void process_request(int client_fd, char *request_buf, int request_len);

#endif // !CONNECTION_H
