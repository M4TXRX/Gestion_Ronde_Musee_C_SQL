#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFSIZE 1024

int sendDatagram(char *,int,char*);

/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

int readFileAdnSendEvent(char *hostname, int portno, char *filename)	//Fonction de lecture du documents event.txt qui simule les passage au points de passsage.
{
	FILE *fic=fopen(filename,"r");										//Gestion de l'ouverture et des erreurs possible
	//FILE *fic=fopen("/home/klikli/Bureau/GBD_PERSO/event.txt","r");
	char ligne[100];
	char *token;
	char espace = ' ';
	if (fic==NULL)
	{
		printf("Impossible d'ouvrir le fichier %s\n",filename);
		exit(1);
	}

	fgets(ligne,100,fic);
	printf("Lecture de chaque ligne\n");
	
	while (!feof(fic))												//On realise la lecture du fichier en boucle tant que on a pas lu toutes les lignes
	{
		
		printf("Appuyez sur une touche pour envoyer une trame\n");	//Il faut appuyer sur une touche pour envoyer une trame
		getchar();													//cela vous permet d'aller dans la base checker que tout fonctionne correctement
		
		ligne[strlen(ligne)-1]='\0';								//On recupere respectiovement chaque valeur en prenant en compte
		char *amac=strtok(ligne,",");								//les separateurs
		char *uid=strtok(NULL,",");
		char *datetime=strtok(NULL,",");	
		char *comments=strtok(NULL,",");
		char *num_modele=strtok(NULL,",");
		
        
		printf("amac=|%s| uid=|%s| datetime=|%s| comments=|%s| num_modele=|%s|\n",amac, uid, datetime, comments, num_modele);	//Affichage des valeurs recuperer dans la ligne du fichier

		sprintf(ligne, "%s %s %s,%s,%s", amac, uid, datetime, comments, num_modele);	//Concatenation pour respecter la reception des donnees sur l'UDP server 
		printf("Trame envoyee : %s \n", ligne);							//Check et affichage de la trame a envoyer
		sendDatagram(hostname, portno, ligne);							//On utilise la fonction d'envoi pour envoyer les trames
		fgets(ligne,100,fic);
	}

	return 0;
}

int sendDatagram(char *hostname, int portno, char *datagram)			//Fonction d'envoi des trames
{
    int sockfd, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char buf[BUFSIZE];

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    /* get a message from the user */
    bzero(buf, BUFSIZE);
    strcpy(buf,datagram);
	
	//printf("Trames :\n");
	//printf("%s\n",buf);
    /* send the message to the server */
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&serveraddr, serverlen);	//Commande d'envoi des commandes
    if (n < 0) 
      error("ERROR in sendto");
    close(sockfd);
}

int main(int argc, char **argv) {
    char *hostname;
    char *filename;
    int portno;

    /* check command line arguments */
    if (argc != 4) {
       fprintf(stderr,"usage: %s <hostname> <port> <eventfile>\n", argv[0]);
       exit(0);
    }

    hostname = argv[1];
    portno = atoi(argv[2]);
    filename = argv[3];

    readFileAdnSendEvent(hostname, portno, filename);
    return 0;
}