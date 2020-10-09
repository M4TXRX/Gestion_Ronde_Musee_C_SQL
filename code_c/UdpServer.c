/* 
 * udpserver.c - A simple UDP echo server 
 * usage: udpserver <port>
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <my_global.h>
#include <mysql.h>

#define BUFSIZE 1024

	
  int modeleCPT = 0;
  int instanceCPT = 0; 				
  
  int numeroBaton = 0;			  //Baton selectionné (dernier caractère addr mac)
  int baton[5];					  //Etat du baton, type de ronde en cours (0 = aucune, 1 = modèle de ronde, 2 = instance de ronde)
  int batonNumeroModele[5];       //Numero du modele en cours
  int batonNumeroInstance[5]; 	  //Numero de l'instance en cours
  int batonOrdrePassage[5];		  //Ordre des points de passages du baton
  
  int selectionModele;			  //Numéro du modele de ronde sélectionné par le garde
 
  
  unsigned int sec_Duree;
  unsigned int min_Duree;
  unsigned int heure_Duree;
  
  char *heure;
  char *min;
  char *seconde;
  int heure_Dmodele[5]; //heure début du modèle
  int min_Dmodele[5];
  int seconde_Dmodele[5];
  int heure_Fmodele[5];	//heure de fin du modèle
  int min_Fmodele[5];
  int seconde_Fmodele[5];
  int heure_Dinstance[5];
  int min_Dinstance[5];
  int seconde_Dinstance[5];
  int heure_Finstance[5];
  int min_Finstance[5];
  int seconde_Finstance[5];
  
  
/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}


void gererDaemon(char *uid, char *date, char *time, char *amac, char *modeleSelect, MYSQL *con )
{
	/*
	 * C'est là que tout se passe.
	 * C'est là qu'il faut insérer du code qui accède à la base de données
	 * PARTIE GERER DEAMON!!
	 */
  char requete[BUFSIZE];
  char datetime[BUFSIZE];
  
  int i=0;

  
  MYSQL_RES *result = mysql_store_result(con);
  

  printf("\nPARTIE GESTION DU DEAMON\n");
  printf("\nINFOS DU CLIENT\nuid = %s\nTime = %s\nDate = %s\nMACadress = %s\nModele Selectionne= %s\n", uid, time, date, amac, modeleSelect);  

	
	switch(amac[16])													//En fonction de l'adresse amac reçue, on détermine le baton en cours d'utilisation
	{
		case '1':
			numeroBaton = 1;
			break;
		case '2':
			numeroBaton = 2;
			break;
		case '3':
			numeroBaton =3;
			break;
		case '4':
			numeroBaton = 4;
			break;
		case '5':
			numeroBaton = 5;
			break;
		default:
			printf("ERREUR : Mauvais numero de baton !\n");
			printf("L'adresse MAC de ce baton de rondier n'existe pas dans la base !\n");
			exit(1); //Fin du programme car le baton utilisé n'existe pas
			break;
	}

	
	
	/************** On détermine ici si il y a eu un crash du serveur en regardant si l'état du baton de rondier reçu est à l'état occupé dans la base alors que nous sommes au début d'une ronde sur le serveur ***********/	
			     sprintf(requete, "SELECT `Nom` FROM `Modele_Ronde` WHERE 1");
				 printf("\nrequete = %s\n", requete);						//On vérifie qu'au moins un modèle de ronde existe
		
				 mysql_query(con, requete);
				 
				 result = mysql_store_result(con);

				 int num_row = mysql_num_rows(result);		//nombre de modèles de ronde
		 
				if (num_row > 0 && baton[numeroBaton] == 0)							//Si il y a au moins un modèle de ronde et qu'on est pas dans la création d'un modèle de ronde ou d'une instance de ronde
				{
					 MYSQL_ROW row;
  				     char etatBatonC[BUFSIZE];
					 int  etatBatonI=0;
					 etatBatonC[0]='0';

					 //Si un baton est occupé dans un modèle de ronde ou une instance de ronde//
					 ////////////////////////////////////////////////////////
					 sprintf(requete, "SELECT `ETAT` FROM `Baton_Rondier` WHERE `ETAT` = '1' OR `ETAT` = '2' AND `Adress_Mac` ='B1:2A:B5:AA:CC:0%d'",numeroBaton); 
				     printf("\nrequete = %s\n", requete); 	
					 
					 mysql_query(con, requete);
					 
					 result = mysql_store_result(con);
					 num_row = mysql_num_rows(result);
	
				 if (num_row > 0)
				  {
				      while((row=mysql_fetch_row(result)))
					{ 
					 strcpy(etatBatonC,row[0]);						//Etat du baton
					}
					
					etatBatonI=atoi(etatBatonC);					//On convertit en int l'état du baton récupéré
					
					
					if (etatBatonI>0)				//Si l'état du baton est supérieur à 0 c'est qu'il est dans l'état 1 ou 2 (création modèle ou création instance)
					{ 
					  printf("\n\nINFORMATION IMPORTANTE : UNE COUPURE DU SERVEUR A SUREMENT EU LIEU, VOTRE BATON B1:2A:B5:AA:CC:0%d EST A L'ETAT %d ! \nSUPPRIMEZ LA RONDE EN COURS (MODELE OU INSTANCE) DE VOTRE BASE DE DONNE !\n \n", numeroBaton, etatBatonI);	
				    }					
				  }	
			     else 
				  {
					 printf("\n\nInformation serveur : Il n'y a pas eu de coupure du serveur pendant une creation de modele ou execution d'une instance de ronde !");
				  }
				   
				}
				////////////////////////////////////////////////////	
	

	
	printf("\n\nNumero du baton utilise par le client = B1:2A:B5:AA:CC:0%d\n", numeroBaton);
	printf("Etat du baton (sur le serveur) utilise par le client = %d\n", baton[numeroBaton]);
	

	/************** On détermine ici nous avons un chef, d'un garde ou un point de passage ***********/	
		if(uid[0] == '2')												//C'est un chef qui utilise le baton
		{	   
			 
			 if(baton[numeroBaton] == 0)									//Si baton libre, on peut commencer un modèle ronde
			 {
				 				 
				 //On vérifie le nombre de modèles existants dans la base
				 /////////////////////////////////////////////////////////////
				 
				 sprintf(requete, "SELECT `Nom` FROM `Modele_Ronde` WHERE 1");
				 printf("\nrequete = %s\n", requete);						//On récupère ici le nombre de modèles de rondes existants
		
				if (mysql_query(con, requete))
				{
				  finish_with_error(con);
				}
				
				else
				{
				 result = mysql_store_result(con);
				}

				if (result == NULL)
				 {
					 finish_with_error(con);
				 }
				 
				int num_row = mysql_num_rows(result);
				
				if (num_row>0)
				{
					modeleCPT = num_row;						//On stocke le nombre de modèles de ronde dans la variable modeleCPT
				}
				
				printf("\nInformation serveur : NOMBRE DE MODELES EXISTANTS DANS LA BASE %d\n",modeleCPT);
				
				//////////////////////////////////////////////////////////////////
				
				 
				printf("\nUN CHEF VIENT DE TAGUER SON BATON A LA BASE POUR COMMENCER SA RONDE !\n");

				baton[numeroBaton] = 1;									//Le baton devient occupé pour un modèle de ronde(0 = libre, 1 = modèle, 2 = instance)
				 
				sprintf(requete, "UPDATE `Baton_Rondier` SET `ETAT` = '1' WHERE `idBaton_Rondier` like '%d'", numeroBaton);
				printf("\nrequete = %s\n", requete);						//On actualise la base de données pour signaler que le baton n'est plus libre
					
 				if (mysql_query(con, requete))
				{
				 finish_with_error(con);
				}			 
				 modeleCPT++;	
				 batonNumeroModele[numeroBaton] = modeleCPT;					//On stock le numéro du modèle actuel associé au baton en cours d'utilisation
				 
				 //Heure de la création du modèle
				 heure = strtok ( time, ":" );
				 min = strtok ( NULL, ":"  );
				 seconde = strtok ( NULL, ":"  );
				
				 heure_Dmodele[numeroBaton] = atoi(heure);
				 min_Dmodele[numeroBaton] = atoi(min);
				 seconde_Dmodele[numeroBaton] = atoi(seconde);
				 printf ( "\nHEURE DEBUT MODELE : %d:%d:%d\n", heure_Dmodele[numeroBaton], min_Dmodele[numeroBaton], seconde_Dmodele[numeroBaton] );
							 
				 sprintf(requete,"INSERT INTO `Modele_Ronde` (`Nom`,`Duree`,`ETAT`,`Ident_Agent_idIdent_Agent`) VALUES('modele%d','0:0:0','0',(Select `idIdent_Agent` from `Ident_Agent` WHERE `UID_RFID` = '%s'))", batonNumeroModele[numeroBaton],uid);
				 printf("\nrequete=%s\n",requete);							//On fait une requête pour créer le nouveau modèle de ronde avec son etat en cours à '0', sa durée à 0:0:0 et son nom
				 

				if (mysql_query(con, requete))
				{
			
					finish_with_error(con);
				}
				
				printf("\nInformation serveur : Le baton de rondier %d n'est plus libre\nCreation d'un modele de ronde.\n", numeroBaton);
			 }
			 
			 
			 else if(baton[numeroBaton] == 1)								//Si le baton est utilisé on termine le modèle de ronde
			 {
				 printf("\nUN CHEF VIENT DE TAGUER SON BATON A LA BASE POUR TERMINER SA RONDE !\n");
				 
				 baton[numeroBaton] = 0;									//Ici le baton redevient libre
				 
				 
				 //Heure fin de la création du modèle
				 heure = strtok ( time, ":" );
				 min = strtok ( NULL, ":"  );
				 seconde = strtok ( NULL, ":"  );
				
				 heure_Fmodele[numeroBaton] = atoi(heure);
				 min_Fmodele[numeroBaton] = atoi(min);
				 seconde_Fmodele[numeroBaton] = atoi(seconde);
				 printf ( "\nInformation serveur : HEURE DEBUT MODELE : %d:%d:%d\n", heure_Dmodele[numeroBaton], min_Dmodele[numeroBaton], seconde_Dmodele[numeroBaton]);
				 printf ( "Information serveur : HEURE FIN MODELE : %d:%d:%d\n", heure_Fmodele[numeroBaton], min_Fmodele[numeroBaton], seconde_Fmodele[numeroBaton]);
				 
				 //On calcule la durée du modèle en heure, minutes et secondes.
				 //Pour cela on transforme tout en secondes puis on fait la différence entre l'heure qu'on vient de récupérer et l'heure récupérer lors de la création du modèle
				 //Enfin on retransforme tout en heure, minutes et secondes
				 heure_Duree = (((heure_Fmodele[numeroBaton] * 3600) + (min_Fmodele[numeroBaton] * 60) + seconde_Fmodele[numeroBaton]) - ((heure_Dmodele[numeroBaton] * 3600) + (min_Dmodele[numeroBaton] * 60) + seconde_Dmodele[numeroBaton])) / 3600;
				 min_Duree = ((((heure_Fmodele[numeroBaton] * 3600) + (min_Fmodele[numeroBaton]  * 60) + seconde_Fmodele[numeroBaton]) - ((heure_Dmodele[numeroBaton] * 3600) + (min_Dmodele[numeroBaton] * 60) +  seconde_Dmodele[numeroBaton])) - (heure_Duree * 3600)) / 60;
				 sec_Duree = ((((heure_Fmodele[numeroBaton] * 3600) + (min_Fmodele[numeroBaton] * 60) + seconde_Fmodele[numeroBaton]) - ((heure_Dmodele[numeroBaton] * 3600) + (min_Dmodele[numeroBaton] * 60) + seconde_Dmodele[numeroBaton])) - (heure_Duree * 3600)) - (min_Duree *60);
				 
				 sprintf(datetime, "%02u:%02u:%02u", heure_Duree, min_Duree, sec_Duree);

				 
				 //On actualise encore le modèle qui vient de se finir en mettant à jour la durée que l'on vient de calculer et en passant l'état à 1
				 sprintf(requete, "UPDATE `Modele_Ronde` SET `Duree` = '%s', `ETAT` = '1'  WHERE `Nom` like 'modele%d'", datetime, batonNumeroModele[numeroBaton]);
				 printf("\nrequete = %s\n", requete); 

				 if (mysql_query(con, requete))
				 {
					 finish_with_error(con);
				 }		
				 
				 sprintf(requete, "UPDATE `Baton_Rondier` SET `ETAT` = '0' WHERE `idBaton_Rondier` like '%d'", numeroBaton);
				 printf("\nrequete = %s\n", requete);						//On actualise la base de données pour signaler que le baton est maintenant libre
				 

				 if (mysql_query(con, requete))
				 {
				     finish_with_error(con);
				 }
					 
				 
				 
				 printf("\nInformation serveur : Le baton de rondier %d est maintenant libre\n", numeroBaton);	 
				 batonOrdrePassage[numeroBaton] = 0;						//On remet l'ordre de passage à 0 pour les prochains modèles ou instances
				 
				 
			 }
			 else if(baton[numeroBaton] == 2)								//Erreur car un chef ne peut pas créer d'instance de ronde, mais seulemnt un modèle de ronde
			 {
				 printf("\nERREUR : un chef ne peut pas creer d'instance de ronde mais seulement un modele de ronde !\n");
			 }
			
				
		 }
		 
		 
		 else if(uid[0] == '1')										//C'est un garde qui utilise le baton
		 {
			 	
		 	 if(baton[numeroBaton] == 0)									//Si le baton est libre, on peut commencer une instance de ronde
			 { 
				
			 
			     printf("\nUN GARDE VIENT DE TAGUER SON BATON A LA BASE POUR COMMENCER SA RONDE !\n");
				 
				 		
				selectionModele = atoi(modeleSelect); // on affecte à selectionModele le numéro du modèle de ronde à réaliser choisi par le garde				
				
				
				 sprintf(requete, "SELECT * FROM `Modele_Ronde` WHERE `Nom` = 'modele%d' AND `ETAT` = '1'",selectionModele);
				 printf("\nrequete = %s\n", requete);						//On vérifie qu'un modèle de ronde existe pour le modèle de ronde choisi et qu'il est à l'état 1
				 
				 			 
		
				 if (mysql_query(con, requete))
				 {
					 finish_with_error(con);
					 printf("ERROR 1 !\n");
				 }
		
				 result = mysql_store_result(con);

				 if (result == NULL)
				 {
					 finish_with_error(con);
				 }

				 int num_row = mysql_num_rows(result);
		 
				 if(num_row == 0)
				 {
					printf("\nERREUR : Il faut d'abord creer un modele de ronde ou finir celui-ci !\n");
					
	 
				 }
				 else
				{
				 
				 //On vérifie le nombre d'instances de ronde existantes dans la base
				 /////////////////////////////////////////////////////////////
				 

				 sprintf(requete, "SELECT * FROM `Instance_Ronde` WHERE 1");
				 printf("\nrequete = %s\n", requete);						//On récupère ici le nombre d'instances de rondes existantes
		
				if (mysql_query(con, requete))
				{
				  finish_with_error(con);
				}
				
				else
				{
				 result = mysql_store_result(con);
				}

				if (result == NULL)
				 {
					 finish_with_error(con);
				 }
				 int num_row = mysql_num_rows(result);
				
				 
				if (num_row>0)
				{ 
				instanceCPT = num_row;						//On stocke le nombre d'instances de ronde dans la variable instanceCPT
				}
				 
				printf("\nInformation serveur : NOMBRE D'INSTANCES EXISTANTES DANS LA BASE %d\n",instanceCPT);
				 
				//////////////////////////////////////////////////////////////////
				
					 

					 baton[numeroBaton] = 2;									//Le baton devient occupé pour une instance de ronde (0 = libre, 1 = modèle, 2 = instance)
					 
					 sprintf(requete, "UPDATE `Baton_Rondier` SET `ETAT` = '2' WHERE `idBaton_Rondier` like '%d'", numeroBaton);
					 printf("\nrequete = %s\n", requete);						//On actualise la base de données pour signaler que le baton n'est plus libre
					
 					 if (mysql_query(con, requete))
					 {
						finish_with_error(con);
					 }
					 
					 
					 instanceCPT++;											//nombre des différentes instance
					 batonNumeroInstance[numeroBaton] = instanceCPT;				//On stock le numéro de l'instance actuelle associée au baton en cours d'utilisation
					 
					 //Heure de la création de l'instance
					 heure = strtok ( time, ":" );
					 min = strtok ( NULL, ":"  );
					 seconde = strtok ( NULL, ":"  );
					
					 heure_Dinstance[numeroBaton] = atoi(heure);
					 min_Dinstance[numeroBaton] = atoi(min);
					 seconde_Dinstance[numeroBaton] = atoi(seconde);
					 printf ( "\nInformation serveur : HEURE DEBUT INSTANCE : %d:%d:%d\n", heure_Dinstance[numeroBaton], min_Dinstance[numeroBaton], seconde_Dinstance[numeroBaton]);
					 
					 
					 sprintf(requete,"INSERT INTO `Instance_Ronde` (`ETAT`,`Nom`,`Modele_Ronde_idModele_Ronde`,`Baton_Rondier_idBaton_Rondier`,`Ident_Agent_idIdent_Agent`) VALUES('%s','Instance%d',(Select `idModele_Ronde` from `Modele_Ronde` WHERE `Nom` = 'modele%d'),(Select `idBaton_Rondier` from `Baton_Rondier` WHERE `idBaton_Rondier`='%d'),(Select `idIdent_Agent` from `Ident_Agent` WHERE `UID_RFID` = '%s'))", "EN COURS",batonNumeroInstance[numeroBaton],selectionModele,numeroBaton, uid);

					 printf("\nrequete=%s\n",requete);							//Requête pour créer la nouvelle instance de ronde

					 if (mysql_query(con, requete))
					 {
						finish_with_error(con);			//erreur si modele n'existe pas ou uid ou baton n'existe pas
					 }
					 
					 printf("\nInformation serveur : Le baton de rondier %d n'est plus libre\nCreation d'une instance de ronde.\n", numeroBaton);
				  
				  
				}
			 }
			 else if(baton[numeroBaton] == 1)								//Erreur car un garde ne peut pas faire de modèle de ronde, mais seulement une instance de ronde.
			 {
				 printf("ERREUR : le baton de rondier %d est en train d'etre utilise pour faire un modele de ronde!\n", numeroBaton);
			 }
			 
			 
			 
			else if(baton[numeroBaton] == 2)								//Si baton utilisé, on termine l'instance de ronde
			 {
					 printf("\nUN GARDE VIENT DE TAGUER SON BATON A LA BASE POUR TERMINER SA RONDE !\n");
				    
					//On récupère ici le nombre de point de passage dans le modèle de ronde
					////////////////////////////////////////////////////// 
				    sprintf(requete, "SELECT `Num` FROM `Modele_Ronde_has_Point_Passage` WHERE `Modele_Ronde_idModele_Ronde`= (Select `idModele_Ronde` from `Modele_Ronde` where `Nom` = 'modele%d')",selectionModele);
					printf("\nrequete = %s\n", requete);						//On récupère ici tous les numéros des points de passage du modèle de ronde en cours 
		
					if (mysql_query(con, requete))
					{
						finish_with_error(con);
					}
				
					else
					{
						result = mysql_store_result(con);
					}

					if (result == NULL)
					{
						finish_with_error(con);
					}
				 
					int num_row = mysql_num_rows(result);
					///////////////////////////////////////////////////

					printf("\nInformation serveur : Nombre de points de passages a effectuer par le garde =%d",num_row);
					printf("\nInformation serveur : Etape de point de passage du modele ou en est le garde dans sa ronde =%d\n",batonOrdrePassage[numeroBaton]);
					 
					 
					 
				  if ( batonOrdrePassage[numeroBaton]==num_row)			//on vérifie si le garde a atteint son dernier point de passage
				    {		
						 
						 //Heure fin de la création de l'instance
						 heure = strtok ( time, ":" );
						 min = strtok ( NULL, ":"  );
						 seconde = strtok ( NULL, ":"  );
						
						 heure_Finstance[numeroBaton] = atoi(heure);
						 min_Finstance[numeroBaton] = atoi(min);
						 seconde_Finstance[numeroBaton] = atoi(seconde);
						 
						 printf ( "\nInformation serveur : HEURE DEBUT INSTANCE : %d:%d:%d\n", heure_Dinstance[numeroBaton], min_Dinstance[numeroBaton], seconde_Dinstance[numeroBaton]);
						 printf ( "Information serveur : HEURE FIN INSTANCE : %d:%d:%d\n", heure_Finstance[numeroBaton], min_Finstance[numeroBaton], seconde_Finstance[numeroBaton]);
						 
						 //On calcule la durée du modèle en heure, minutes et secondes.
						 //Pour cela on transforme tout en secondes puis on fait la différence entre l'heure qu'on vient de récupérer et l'heure récupérer lors de la création du modèle
						 //Enfin on retransforme tout en heure, minutes et secondes
						 heure_Duree = (((heure_Finstance[numeroBaton] * 3600) + (min_Finstance[numeroBaton] * 60) + seconde_Finstance[numeroBaton]) - ((heure_Dinstance[numeroBaton] * 3600) + (min_Dinstance[numeroBaton] * 60) + seconde_Dinstance[numeroBaton])) / 3600;
						 min_Duree = ((((heure_Finstance[numeroBaton] * 3600) + (min_Finstance[numeroBaton]  * 60) + seconde_Finstance[numeroBaton]) - ((heure_Dinstance[numeroBaton] * 3600) + (min_Dinstance[numeroBaton] * 60) +  seconde_Dinstance[numeroBaton])) - (heure_Duree * 3600)) / 60;
						 sec_Duree = ((((heure_Finstance[numeroBaton] * 3600) + (min_Finstance[numeroBaton] * 60) + seconde_Finstance[numeroBaton]) - ((heure_Dinstance[numeroBaton] * 3600) + (min_Dinstance[numeroBaton] * 60) + seconde_Dinstance[numeroBaton])) - (heure_Duree * 3600)) - (min_Duree *60);
						 
						 sprintf(datetime, "%02u:%02u:%02u", heure_Duree, min_Duree, sec_Duree);
						 
						 
						 //On actualise encore le modèle qui vient de se finir en mettant à jour la fin de l'instance
						 sprintf(requete, "UPDATE `Instance_Ronde` SET `Duree` = '%s', `Etat` = 'TERMINE'  WHERE `Nom` like 'Instance%d' ", datetime, batonNumeroInstance[numeroBaton]);
						 printf("\nrequete = %s\n", requete); 

						 if (mysql_query(con, requete))
						 {
							 finish_with_error(con);
						 }		
										 
						 sprintf(requete, "UPDATE `Baton_Rondier` SET `ETAT` = '0' WHERE `idBaton_Rondier` like '%d'", numeroBaton);
						 printf("\nrequete = %s\n", requete);						//On actualise la base de données pour signaler que le baton est maintenant libre
						 

						 if (mysql_query(con, requete))
						 {
							 finish_with_error(con);
						 }
							 
						 printf("\nInformation serveur : Le baton de rondier %d est maintenant libre !\n", numeroBaton);
						 baton[numeroBaton] = 0;				 	 
						 batonOrdrePassage[numeroBaton] = 0;
						 
				    }
					
				  else
					{
					   printf("\nERREUR : Vous n'avez pas termine votre RONDE !\n");
					}
			 }
			
		 }
		 
		 
		 
		 
		 
		else if(uid[0] == '0')								//C'est un point de passage
		{
			
			
			if(baton[numeroBaton] == 0)									//Le baton est libre et donc inutilisé
			 {
				 printf("\nERREUR : Il faut d'abord s'identifier.\n");
			 }
			 
			else if (baton[numeroBaton] == 1 ) 		//Baton utilisé pour un modèle de ronde	
			{
				 printf("\nDEMANDE DE TAG DE POINT DE PASSAGE PAR UN CHEF\n");
			 			 
				 batonOrdrePassage[numeroBaton] = batonOrdrePassage[numeroBaton] + 1;				//On incrémente l'ordre de passage correspondant au baton en cours d'utilisation à 
																							//chaque fois qu'un nouveau point de passage est tagué
				 printf("\nInformation serveur : Le chef en est a son point de passage numero %d \n", batonOrdrePassage[numeroBaton]);
				 
				 MYSQL_ROW row;
				 char modeleCreation[BUFSIZE];
				 
				 
				 sprintf(requete, "SELECT `idModele_Ronde` FROM `Modele_Ronde` WHERE `ETAT` = '0' AND `Nom` = 'modele%d'", batonNumeroModele[numeroBaton]);
				 printf("\nrequete = %s\n", requete);						//On sélectionne le modèle de ronde en cours de création avec son nom et son état à 0
		
				 if (mysql_query(con, requete))
				 {
					 finish_with_error(con);
				 }
		
				 result = mysql_store_result(con);

				 if (result == NULL)
				 {
					 finish_with_error(con);
				 }
				
	 
				 while((row=mysql_fetch_row(result)))
				 { 
					 strcpy(modeleCreation,row[0]);						//On récupère l'ID du modèle de ronde en cours de création
					
				 }
			
				 sprintf(requete,"INSERT INTO `Modele_Ronde_has_Point_Passage`(`Modele_Ronde_idModele_Ronde`, `Point_Passage_idPoint_Passage`, `Num`,`Date_event` ) VALUES ('%s',(Select `idPoint_Passage` from `Point_Passage` where `Ident_Tag`='%s'), '%d','%s''%s')", modeleCreation, uid, batonOrdrePassage[numeroBaton],date, time);
				 
				 
				 printf("\nrequete=%s\n",requete);							//On crée une nouvelle entrée dans la base de données associant le point de passage qui vient d'être tagué
																		//avec le modèle de ronde en cours de création
				 if (mysql_query(con, requete)) 
				 {
					 finish_with_error(con);
				 }
				 printf("\nInformation serveur : Le point de passage %s a ete tague pour le modele de ronde\n", uid);
				 
				 //On complète l'historique du rondier dans le cas de l'execution du modele de ronde
				 
				char batonModele[BUFSIZE];
				 
				 sprintf(requete, "SELECT `idBaton_Rondier` FROM `Baton_Rondier` WHERE `idBaton_Rondier` = '%d'", numeroBaton);
				 printf("\nrequete = %s\n", requete);						//On sélectionne le baton de rondier en cours d'utilisation
		
				 if (mysql_query(con, requete))
				 {
					 finish_with_error(con);
				 }
		
				 result = mysql_store_result(con);

				 if (result == NULL)
				 {
					 finish_with_error(con);
				 }
							
	 
				 while((row=mysql_fetch_row(result)))
				 { 
					 strcpy(batonModele,row[0]);						//On récupère l'ID du baton de rondier en cours d'utilisation
					 
				 }
				 
				 sprintf(requete,"INSERT INTO `Historique_Rondier`(`Baton_Rondier_idBaton_Rondier`, `Point_Passage_idPoint_Passage`,`Date_event`) VALUES ('%s',(Select `idPoint_Passage` from `Point_Passage` where `Ident_Tag`='%s'),'%s''%s')", batonModele, uid,date, time );
				 printf("\nrequete=%s\n",requete);							//On crée une nouvelle entrée dans la base de données associant le point de passage qui vient d'être tagué
																		//avec le baton en cours d'utilisation
				 if (mysql_query(con, requete)) 
				 {
					 finish_with_error(con);
				 }
				  printf("\nInformation serveur : VOUS ETES TRACE l'historique du rondier a ete complete avec l'identifianf du baton de rondier utilise pour ce modele de ronde !");
			
				 
		    }
			
			
			
			
			else if (baton[numeroBaton] == 2) //Baton utilisé pour une instance de ronde
			{	
				 printf("\nDEMANDE DE TAG DE POINT DE PASSAGE PAR UN GARDE\n");
				 
				 char numIdRFID[BUFSIZE];
				 
				 MYSQL_ROW row;
				 
				 //On regarde ici le point de passage selectionne et celui attendu
				 ///////////////////////////////////////////////////////////////
				 sprintf(requete, "SELECT `Ident_Tag` FROM `Point_Passage` WHERE `idPoint_Passage` IN (SELECT`Point_Passage_idPoint_Passage` FROM `Modele_Ronde_has_Point_Passage` WHERE `Num` = '%d' AND `Modele_Ronde_idModele_Ronde`=(SELECT `idModele_Ronde` FROM `Modele_Ronde` WHERE `Nom`= 'modele%d'))",  batonOrdrePassage[numeroBaton]+1, selectionModele );
				 printf("\nrequete = %s\n", requete);						//On récupère ici l'identifiant du point de passage pour respecter le modele de ronde avec l'identifiant du modèle de ronde et l'ordre de passage ou en est le garde
		
				if (mysql_query(con, requete))
				{
				  finish_with_error(con);
				}
				
				else
				{
				 result = mysql_store_result(con);

				if (result == NULL)
				 {
					 finish_with_error(con);
				 }
				 

	 		
				 while((row=mysql_fetch_row(result)))
				 { 
					 strcpy(numIdRFID,row[0]);						//On récupère le caractère qui permet d'identifier le point de passage

				 }

				 printf("\nInformation serveur : numero du point de passage selectionne : 0000000%c \n",uid[7]);
				 printf("Information serveur : numero du point de passage attendu : 0000000%c \n",numIdRFID[7]);
				 
				}
				////////////////////////////////////////////////////////
				
				 //On regarde ici le nombre de point de passage à taguer dans le modele choisi afin de comparer ce nombre aux points de passage déjà tagués
				 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				    sprintf(requete, "SELECT `Num` FROM `Modele_Ronde_has_Point_Passage` WHERE `Modele_Ronde_idModele_Ronde`= (Select `idModele_Ronde` from `Modele_Ronde` where `Nom` = 'modele%d')",selectionModele);
					printf("\nrequete = %s\n", requete);						//On récupère ici tous les numéros des points de passage du modèle de ronde en cours 
		
					if (mysql_query(con, requete))
					{
						finish_with_error(con);
					}
				
					else
					{
						result = mysql_store_result(con);
					}

					if (result == NULL)
					{
						finish_with_error(con);
					}
				 
					int num_row = mysql_num_rows(result);
					
					int nbPointsDePassage = num_row;
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
				
			 if (uid[7] == numIdRFID[7] && batonOrdrePassage[numeroBaton]<nbPointsDePassage)		//on compare le numéro uid du point de passage envoyé par le garde à celui attendu dans le modèle et on regarde si le garde a terminé sa ronde
			 
				{
				
				 batonOrdrePassage[numeroBaton] = batonOrdrePassage[numeroBaton] + 1;				//On incrémente l'ordre de passage correspondant au baton en cours d'utilisation à 
																							//chaque fois qu'un nouveau point de passage est tagué
				 printf("\nInformation serveur : Le garde en est a son point de passage numero %d apres son tag \n", batonOrdrePassage[numeroBaton]);
				 
				 char batonInstance[BUFSIZE];
				 
				 sprintf(requete, "SELECT `idBaton_Rondier` FROM `Baton_Rondier` WHERE `idBaton_Rondier` = '%d'", numeroBaton);
				 printf("\nrequete = %s\n", requete);						//On sélectionne le baton de rondier en cours d'utilisation
		
				 if (mysql_query(con, requete))
				 {
					 finish_with_error(con);
				 }
		
				 result = mysql_store_result(con);

				 if (result == NULL)
				 {
					 finish_with_error(con);
				 }
				 
	 
				 while((row=mysql_fetch_row(result)))
				 { 
					 strcpy(batonInstance,row[0]);						//On récupère l'ID du baton de rondier en cours d'utilisation
				 }
				 
			
				 sprintf(requete, "INSERT INTO `Instance_Ronde_has_Point_Passage`(`Instance_Ronde_idInstance_Ronde`, `Point_Passage_idPoint_Passage`,`Date_event`) VALUES ((SELECT `idInstance_Ronde` FROM `Instance_Ronde` WHERE `Nom` = 'Instance%d'),(Select `idPoint_Passage` from `Point_Passage` where `Ident_Tag`='%s'),'%s''%s')",batonNumeroInstance[numeroBaton],uid, date, time);
				 
				
				 printf("\nrequete=%s\n",requete);							//On crée une nouvelle entrée dans la base de données associant le point de passage qui vient d'être tagué
																		//avec l'instance en cours
				 if (mysql_query(con, requete)) 
				 {
					 finish_with_error(con);
				 }
				 
				 printf("\nInformation serveur : Le point de passage %s a ete tague pour l'instance de ronde\n", uid);
				 
				  //On complète l'historique du rondier dans le cas de l'execution de l'instance de ronde
				 
				 sprintf(requete,"INSERT INTO `Historique_Rondier`(`Baton_Rondier_idBaton_Rondier`, `Point_Passage_idPoint_Passage`,`Date_event`) VALUES ('%s',(Select `idPoint_Passage` from `Point_Passage` where `Ident_Tag`='%s'),'%s''%s')", batonInstance, uid,date, time );
				 printf("\nrequete=%s\n",requete);							//On crée une nouvelle entrée dans la base de données associant le point de passage qui vient d'être tagué
				 

														//avec le baton en cours d'utilisation
				 if (mysql_query(con, requete)) 
				 {
					 finish_with_error(con);
				 }
				  
				  printf("\nInformation serveur : VOUS ETES TRACE l'historique du rondier a ete complete avec l'identifiant du baton de rondier utilise pour cette instance de ronde !");
			    }
				
				else 
				{
				   printf("\nERREUR : ATTENTION vous devez respecter les etapes du modele de ronde ! Vous DEVEZ TAGUER le tag : %s ! Si vous avez deja tague ce tag vous avez alors termine votre ronde !",numIdRFID);
				}
			
			}
	
		}  
		 
		 else 															//L' uid ne correspond ni à un chef, ni à un garde ni à un point de passage
		 {
			 printf("\nERREUR : Votre uid \'%s\' n'est pas reconnu !\n", uid);
		 }

	
}





int main(int argc, char **argv) {
  int sockfd; /* socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buf */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */
  
  char datetime[BUFSIZE];
  
  
  int batonOrdrePassage[5];
  int baton[5], i = 0;

	
	char* date;
	char* time;
	char* amac;
	char* uid;
	char* comments;
	char* modeleSelect;

  /* 
   * check command line arguments 
   */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  /* 
   * socket: create the parent socket 
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");
	
	
   for(i = 0; i < 8; i++)			//Initialisation à 0 du tableau contenant l'ordre de passage pour chaque baton
   {
	  batonOrdrePassage[i] = 0;
   }
   
   for (i = 0; i < 8; i++)
   {
	 baton[i] = 0;			// Les bâtons sont libres au début (0 = libre, 1 = modele de ronde, 2 = instance de ronde)
   }

  /* 
   * main loop: wait for a datagram, then echo it
   */
  clientlen = sizeof(clientaddr);
  
  
  
  while (1) {

    /*
     * recvfrom: receive a UDP datagram from a client
     */
    bzero(buf, BUFSIZE);
    n = recvfrom(sockfd, buf, BUFSIZE, 0,
		 (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0)
      error("ERROR in recvfrom");

    /*
     * C'est ici que vous devez implanter la logique métier, liée à votre projet.
     * Il faut supprimer le printf suivant et le remplacer par votre code
     */
	 
    MYSQL *con = mysql_init(NULL);
        
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
           
    if (mysql_real_connect(con, "localhost", "root", "",
          "RONDIER", 0, NULL, 0) == NULL)
	{
      finish_with_error(con);
	}
	
	MYSQL_RES *result = mysql_store_result(con);
	
	 /*
     * C'est ici que vous devez implanter la logique métier, liée à votre projet.
     * Il faut supprimer le printf suivant et le remplacer par votre code
     */
	printf("\n \n \n \n \n \n \nNOUVELLE TRAME RECUE DU CLIENT \n");
    printf("Received packet from %s:%d\nData: [%s]\n\n", 
                inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
				
				
	//ON RECUPERE ICI LE MESSAGE RECU PAR LE CLIENT//

    amac=strtok(buf," ");							//On récupère chaque infos contenu dans le message à décoder : l'adresse amac, l'uid, la date, l'heure et le commentaire
    uid=strtok(NULL," ");
    date=strtok(NULL," "); 
	time=strtok(NULL,","); 
    comments=strtok(NULL,","); 
	modeleSelect=strtok(NULL," "); 
    printf("amac=|%s| uid=|%s| date=|%s| time=|%s| comments=|%s| modeleSelect=|%s|\n",amac, uid, date, time, comments, modeleSelect);
  
	gererDaemon(uid,date,time,amac,modeleSelect,con);
	
	  /* 
     * sendto: echo the input back to the client 
     */
    n = sendto(sockfd, buf, strlen(buf), 0, 
	       (struct sockaddr *) &clientaddr, clientlen);
    if (n < 0) 
      error("ERROR in sendto");
  
    
  }
}
