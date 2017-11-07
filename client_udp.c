#include "stdlib.h"
#include "stdio.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "pthread.h"
#include "sys/un.h"

int main(){
   int clientsocket;
   int porta = 50000;
   int portaservidor = 50001;
   char sendbuffer[100]="Mensagem de teste.\0";
   char receivebuffer[100];
   struct sockaddr_in servidor_addr;
   struct sockaddr_in client_addr; 
   int sock_size = sizeof(servidor_addr);
   
   printf("Iniciando o cliente. \n");

   //Criando socket cliente
   if ((clientsocket = socket(AF_INET,SOCK_DGRAM,0))<0){
      perror("Erro ao criar socket cliente: ");
   }
   
   //Prepara o endereco do cliente
   client_addr.sin_family = AF_INET;
   //peer_addr.sin_addr.s_addr   = INADDR_ANY; 
   client_addr.sin_addr.s_addr   = INADDR_ANY;
   client_addr.sin_port   = htons(porta);
   memset(&(client_addr.sin_zero), '\0', sizeof(client_addr.sin_zero));
   
   //Faz a ligacao do socket cliente ao endereco do cliente
   if (bind(clientsocket, (struct sockaddr *)&client_addr, sizeof(struct sockaddr))<0){
      perror("Erro ao fazer ligacao no socket cliente");
   }

   //Prepara o endereco do servidor
   servidor_addr.sin_family = AF_INET;
   servidor_addr.sin_addr.s_addr   = inet_addr("127.0.0.1");
   servidor_addr.sin_port   = htons(portaservidor);
   memset(&(servidor_addr.sin_zero), '\0', sizeof(servidor_addr.sin_zero));


   //Envia mensagem para o servidor
   if(sendto(clientsocket, sendbuffer, sizeof(sendbuffer), 0, (struct sockaddr *) &servidor_addr, sock_size)<0){
      perror("Erro ao enviar mensagem ao servidor");
   }
   
   //Aguarda a resposta do servidor
   if(recvfrom(clientsocket, receivebuffer, sizeof(receivebuffer), 0, (struct sockaddr *)&servidor_addr, &sock_size)<0){
      perror("Erro ao receber resposta do servidor: ");
   }else{
      printf("Mensagem recebida do servidor: \n %s .\n", receivebuffer);
   }

   close(clientsocket);
   printf("Cliente terminou. \n");
   exit(0);
}
