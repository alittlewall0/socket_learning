#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFF_LEN 10240
#define DEBUG 0
int main(int argc, char** argv) {
  if (argc != 4) {
    printf("Usage:%s [ip] [port] [path eg:raw.txt]\n", argv[0]);
    return -1;
  }
  char* ip = argv[1];
  int port = atoi(argv[2]);

  // 输出txt
  FILE* stream;
  stream = fopen(argv[3], "a");

  //创建套接字
  int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  //将套接字和IP、端口绑定
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));   //每个字节都用0填充
  serv_addr.sin_family = AF_INET;             //使用IPv4地址
  serv_addr.sin_addr.s_addr = inet_addr(ip);  //具体的IP地址
  serv_addr.sin_port = htons(port);           //端口
  bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
#if DEBUG
  unsigned long ip = inet_addr("127.0.0.1");
  printf("%ld\n", ip);
#endif
  printf("Listening... ...");
  //进入监听状态，等待用户发起请求
  listen(serv_sock, 20);
  
  char buf[10240];

  //接收客户端请求
  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size = sizeof(clnt_addr);
  while (1) {
    int clnt_sock =
        accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    

    //接受数据
    int count = read(clnt_sock, buf, sizeof(buf) - 1);
    if (count == -1) {
      printf("recieve data fail!\n");
      continue;
    }

    printf("recv num : %d\n", count);
    for (int i = 0; i < count; i++) {
      fprintf(stream, "%x", buf[i]);
    }
    close(clnt_sock);
    fprintf(stream, "\n");
  }

  //关闭套接字
  close(serv_sock);

  // 关闭输出文件
  fprintf(stream, "\n\n\n");
  fclose(stream);
  return 0;
}