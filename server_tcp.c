#include "stdlib.h"
#include "stdio.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "arpa/inet.h"
#include "error.h"
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PORTNUMBER 50000
#define MAXLINE 200

struct argus {
  int connection;
  struct sockaddr_in client;
};

void * executeClient(void * arg){
  struct argus * arguments = arg;
  printf("Conexao estabelecida com cliente %s. \n", inet_ntoa(arguments->client.sin_addr));
  //Lendo mensagem enviada pelo cliente
  char buffer[MAXLINE];
  int tamr = read(arguments->connection, buffer, MAXLINE);
  buffer[tamr] = '\0';
  printf("Cliente %s enviou mensagem %s", inet_ntoa(arguments->client.sin_addr), buffer);

  // if(strstr(buffer, "desligar")){
  //    //Encerra o socket servidor
  //    close(serversocket);
  //    //Finaliza o servidor
  //    exit(0);
  // }
  //Encerra a conexao com o cliente
  close(arguments->connection);

}

int createThread(int connectionId, struct sockaddr_in clientAddress){
  pthread_t thread;
  struct argus arguments;
  arguments.connection = connectionId;
  memcpy(&arguments.client, &clientAddress, sizeof(struct sockaddr_in));
  pthread_create(&thread, NULL, executeClient, (void*) &arguments);
}


int main(){
   int serversocket;
   int tamr;
   char buffer[MAXLINE];
   struct sockaddr_in server_addr;
   struct sockaddr_in client_addr;
   int connectionsocket;
   int socketsize;
   int flag;


   //Criacao do socket no servidor
   printf("Iniciando o servidor. \n");
   if ((serversocket = socket(AF_INET,SOCK_STREAM,0))<0){
      perror("Erro ao criar socket do servidor: \n");
   }

   //Configuracao do endereco do servidor que sera ligado ao socket
   //zera toda a estrutura
   memset(&server_addr, 0, sizeof(server_addr));

   //Tipo de endereco
   server_addr.sin_family = AF_INET;
   //especificacao do endereco
   server_addr.sin_addr.s_addr   = INADDR_ANY;

   //Querendo especificar manualmente um endereco pode-se usar
   //server_addr.sin_addr.s_addr   = inet_addr("127.0.0.1");

   //Numero da porta
   server_addr.sin_port   = htons(PORTNUMBER);
   //
   //memset(&(server_addr.sin_zero), '\0', sizeof(server_addr.sin_zero));

   //Ligando o endereco do servidor ao socket
   if (bind(serversocket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))<0){
      perror("Erro de ligacao de socket no servidor: \n");
   }

   //Indica que o socket deve "escutar"
   if (listen(serversocket, 1)<0){
      perror("Erro ao inicializar a escuta do socket no servidor: \n");
   }

   printf("Servidor escutando na porta: %d .\n", PORTNUMBER);

   //Servidor fica em loop para receber conexoes
   while(1){
      socketsize = sizeof(client_addr);

      //Servidor aguardando por conexao
      connectionsocket = accept(serversocket, (struct sockaddr *)&client_addr, &socketsize);

      if(connectionsocket < 0){
         perror("Erro ao receber pedido de conexao: \n");
      }else{
        createThread(connectionsocket, client_addr);
      }

   }
}
