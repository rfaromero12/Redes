#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <time.h>
#include <locale.h>

int main(int args,char **argv){
    
	
	
    //estructura tiempo
    time_t tiempo;
	struct tm * stTm;
	char cadena_tiempo [80];


	tiempo = time(NULL);

	setlocale(LC_ALL,"");
	
	stTm = localtime(&tiempo);

	

	
	



    //Datos server
    struct sockaddr_in Servidor;
    int Descriptor_Servidor;

    //Datos cliente
    struct sockaddr_in Cliente;
    socklen_t longitudCliente;
    //Apertura del socket
    Descriptor_Servidor = socket(AF_INET,SOCK_DGRAM,0);
    
    if (Descriptor_Servidor == -1)
    {
        exit(EXIT_FAILURE);
        printf("Error en la apertura del socket\n");
    }
    //Rellenamos la estructura 
    Servidor.sin_family = AF_INET;
    Servidor.sin_addr.s_addr = htonl(INADDR_ANY);      //inet_addr(   );
    Servidor.sin_port = htons(2000);


    //Asociamos puerto al descriptor
    if (bind(Descriptor_Servidor,(struct sockaddr_in *)&Servidor, sizeof(Servidor)) == -1)
    {
        printf("No se pudo asociar el socket a un puerto\n");
        close(Descriptor_Servidor);
        exit(EXIT_FAILURE);
    }


    longitudCliente = sizeof(Cliente);

    while(1){
          //recibo los mensajes del cliente
           char cadena_recibida[80];
          //Dependiendo del paquete codifico el mensaje, lo codifico y lo devuelvo.  
          	int recibido = recvfrom (Descriptor_Servidor, &cadena_recibida, sizeof(cadena_recibida), 0,
			(struct sockaddr *) &Cliente, &longitudCliente);



		/* -----------------------------------------------------------------
			Comprobamos si hemos recibido alguna informaci\ufffdn 
		-------------------------------------------------------------------*/
		if (recibido > 0)
		{
			/*-----------------------------------------------------------------
				Incrementamos el valor que nos ha enviado el cliente 
				------------------------------------------------------------------*/
      	    if (strcmp(cadena_recibida,"DAY") == 0)
              { 
                  strftime(cadena_tiempo,80,"%A, %d de %B de %Y", stTm);
                  printf ("Recibido %s, envio %s\n", cadena_recibida,cadena_tiempo );

              }
            else if (strcmp(cadena_recibida,"TIME") == 0)
            {
                strftime(cadena_tiempo,80,"%H:%M:%S", stTm);
                  printf ("Recibido %s, envio %s\n", cadena_recibida,cadena_tiempo );
            }
            else if (strcmp(cadena_recibida,"DAYTIME") == 0)
            {
                strftime(cadena_tiempo,80,"%A, %d de %B de %Y;%H:%M:%S", stTm);
                  printf ("Recibido %s, envio %s\n", cadena_recibida,cadena_tiempo );
            }
            
       
      
      	/* ------------------------------------------------------------------
				Devolvemos el n\ufffdmero incrementado al cliente
				--------------------------------------------------------------------*/
			int enviado = sendto (Descriptor_Servidor, &cadena_tiempo, sizeof(cadena_tiempo), 0,
			(struct sockaddr *) &Cliente, longitudCliente);

		}
          




    }


    //Cierre del socket
    close(Descriptor_Servidor);
    
    
}
