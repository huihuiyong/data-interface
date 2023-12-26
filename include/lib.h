#ifndef LIB_H
#define LIB_H

#define CHANNLE_NAME "/tmp/my_socket"

int open_server(char * channle_name);
int open_client(char * channle_name);
int recv_data(int fd, void * buf, size_t len);
int send_data(int fd, const void * buf, size_t len);

#endif 
