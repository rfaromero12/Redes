#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <string.h>

int main (int argc, char **argv )
{
	if (argc != 3)
	{
		printf("Error en la linea de comandos\n");
		exit(0);
	}

    //Datos del select
    struct timeval timeout;
	fd_set lectura;
	int salida;
	char cadena_select[60];

    //Inicializar la estructua timeval
	timeout.tv_sec = atoi(argv[2]);
	timeout.tv_usec = 0;
	
	//Inicializar los conjuntos fd_set
	
	

		/*---------------------------------------------------- 
			Descriptor del socket y buffer para datos 
		-----------------------------------------------------*/
		int Socket_Cliente;
		char Datos[80];
   
   	/* -----------------------------------------------------
   		Informaci\ufffdn del Servidor
   	-----------------------------------------------------*/
   	struct sockaddr_in Servidor;  
   	socklen_t Longitud_Servidor;
  
  
   	/* --------------------------------------------------
			Se abre el socket cliente
		---------------------------------------------------*/
		Socket_Cliente = socket (AF_INET, SOCK_DGRAM, 0);
		if (Socket_Cliente == -1)
		{
			printf ("No se puede abrir el socket cliente\n");
    			exit (-1);	
		}
			FD_ZERO(&lectura);
			FD_SET(Socket_Cliente,&lectura); 		/*---------------------------------------------------------------------
			Necesitamos una estructura con la informaci\ufffdn del Servidor
			para poder solicitarle un servicio.
   		----------------------------------------------------------------------*/
		Servidor.sin_family = AF_INET;
		Servidor.sin_port = htons(2000);
		Servidor.sin_addr.s_addr =  inet_addr(argv[1]);
  	 	Longitud_Servidor = sizeof(Servidor);
	

		/*------------------------------------------------------------------------
			Vamos a preguntarle al usuario sobre el formato del paquete
		------------------------------------------------------------------------ */
        printf("Que formato de DAYTIME quieres:\n DAY , TIME o DAYTIME\n");
        scanf("%s",&Datos);
    


   	
		/*-----------------------------------------------------------------------
			Se env\ufffda mensaje al Servidor
		-----------------------------------------------------------------------*/
		int enviado = sendto (Socket_Cliente, (char *) &Datos, sizeof(Datos), 0,
			(struct sockaddr *) &Servidor, Longitud_Servidor);
   
   	if (enviado < 0)
    	{
			printf("Error al solicitar el servicio\n");
		}
    	else
		{
             salida = select(Socket_Cliente + 1,&lectura,NULL,NULL,&timeout);
        	if(salida == -1)
    	{
	    	printf("Se ha producido un error en select\n");
    	}
	    else if(salida == 0)
	    {
		    printf("Se ha agotado el tiempo\n");

	    }
	    else if(FD_ISSET(Socket_Cliente,&lectura)){
            printf("hola\n");
			int recibido = recvfrom (Socket_Cliente, (char *)&Datos, sizeof(Datos), 0,
			(struct sockaddr *) &Servidor, &Longitud_Servidor);
			
   		if (recibido > 0)
      		printf ("Leido %s\n", Datos);
   		else
      		printf ("Error al leer del servidor\n");
		
		}
		    
            
            
        } 
	
			
		
		close(Socket_Cliente);

	return 0;
}
