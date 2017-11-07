#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>

#define MAXLINE 200

int main(){

   int clientSock;
   int conn;
   unsigned int tams;
   char buffer[MAXLINE];
   struct sockaddr_in s;
   int portaServidor = 50000;


   //Criando socket cliente
   clientSock = socket(AF_INET, SOCK_STREAM, 0);
   if(clientSock < 0){
		perror("Erro na criacao do socket client: ");
   }

   memset(&s, 0, sizeof(s));

   //Especificacao do endereco do servidor para efetuar conexao
   s.sin_family = AF_INET;
   s.sin_addr.s_addr = inet_addr("127.0.0.1");
   s.sin_port = htons(portaServidor);

   //Efetua conexao com o sevidor
   conn = connect(clientSock, (struct sockaddr *) &s, sizeof(s));
   if(conn < 0){
      perror("Erro ao tentar se conectar com o servidor: ");
   }

   //Le entrada do teclado
   printf("Entre com a mensagem que gostaria de enviar: \n");
   fgets(buffer,MAXLINE,stdin);

   //Envia mensagem ao servidor
   int i = 0;
   for(i = 0; i < 5; i++){
     tams = write(clientSock,buffer,strlen(buffer));
   }
   if(tams == strlen(buffer)){
      printf("Mensagem enviada. \n");

   }else{
		 	printf("Erro no servidor.\n");
   }

   conn = close(clientSock);
   printf("Cliente ecerrado. \n");
   exit(0);
}
