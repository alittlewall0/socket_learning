#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char** argv) {

  if (argc != 3) {
    printf("Usage:%s [ip] [port]\n", argv[0]);
    return -1;
  } 
    char* ip = argv[1];
    int port = atoi(argv[2]);
 

  //向服务器（特定的IP和端口）发起请求
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));   //每个字节都用0填充
  serv_addr.sin_family = AF_INET;             //使用IPv4地址
  serv_addr.sin_addr.s_addr = inet_addr(ip);  //具体的IP地址
  serv_addr.sin_port = htons(port);           //端口

  //读取服务器传回的数据
  char buffer[40];
  memset(buffer, 0, sizeof(buffer));
  while (1) {
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    read(sock, buffer, sizeof(buffer) - 1);

    printf("Message form server: %s\n", buffer);
    memset(buffer, 0, sizeof(buffer));
    close(sock);
  }

  //关闭套接字

  return 0;
}