#include "stdlib.h"
#include "stdio.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "pthread.h"
#include "sys/un.h"

int main(){
   int serversocket;
   int portaservidor = 50001;
   char buffer[100];
   struct sockaddr_in servidor_addr;
   struct sockaddr_in client_addr; 
   int sock_size = sizeof(client_addr);
   
   printf("Iniciando o servidor. \n");

   //Criando socket cliente
   if ((serversocket = socket(AF_INET,SOCK_DGRAM,0))<0){
      perror("Erro ao criar socket servidor: ");
   }
   
   //Prepara o endereco do servidor
   servidor_addr.sin_family = AF_INET;
   servidor_addr.sin_addr.s_addr   = INADDR_ANY;
   servidor_addr.sin_port   = htons(portaservidor);
   memset(&(servidor_addr.sin_zero), '\0', sizeof(servidor_addr.sin_zero));

   //Faz a ligacao do socket cliente ao endereco do cliente
   if (bind(serversocket, (struct sockaddr *)&servidor_addr, sizeof(struct sockaddr))<0){
      perror("Erro ao fazer ligacao no socket servidor: ");
   }

   //Aguarda mensagem do cliente
   if (recvfrom(serversocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &sock_size)<0){
      perror("Erro ao receber mensagem do cliente: ");
   }else{
      printf("Mensagem do cliente recebida: \n %s \n", buffer);
   }

   //Envia resposta ao server
   strcpy(buffer, "Cliente, o servidor recebeu sua mensagem.");
   if(sendto(serversocket, buffer, sizeof(buffer), 0, (struct sockaddr *) &client_addr, sizeof(struct sockaddr))<0){
      perror("Erro ao enviar mensagem para o cliente");
   }
  
   close(serversocket);
   printf("Servidor terminou. \n");
   exit(0);
}
