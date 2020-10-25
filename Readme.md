## Table of contents
* [General info](#general-info)
* [Setup](#setup)

## General info 
Ce projet permet la gestions des rondes de gardiens dans les musées avec sauvegarde des points de passage dans une DB MySQL.  
Veuillez lire le rapport "Rapport_projet.pdf" pour plus de details et comprendre le projet dans son ensemble.

Pour pouvoir éxecuter ce programme vous devez avoir installer en local WAMP server avec PHPmyAdmin qui doit être bien fonctionnel.
  
Référez vous au dossier pdf "sql/Script_SQL_Ordre.pdf" pour pouvoir installer les éléments nécessaires dans la base de donnée.
C'est le programme UdpServer.c qui se connecte à la base de données avec les informations de connexions de PHPmyAdmin par défaut "login : root et pass : (vide)".
	
## Setup

* Pour compiler le fichier UdpClient.c :
		```
		$ gcc -o client UdpClient.c
		```
* Le compilateur creera un fichier executable client
	- Pour lancer le programme tapez :
		```
		$ ./client 127.0.0.1 8881 events.txt
		```
		!! Avec le fichier events.txt (contenant les pointages du gardien qui fait sa ronde) dans le dossier ou se trouve le programme
		
		!! Pour lancer toutes les requêtes du client rapidement restez appuyer sur la touche ENTER

* Pour compiler le fichier UdpServer.c
		```
		$ gcc -o server UdpServer.c `mysql_config --cflags --libs`
		```
* Le compilateur creera un fichier executable server
	- Pour lancer le programme tapez :
		```
		$ ./server 8881
		```

## Other documents 
Dans le dossier racine :

	|
	|
	--> BigPicture.pdf
		image du fonctionnement globale du projet
		
	--> Rapport_projet.pdf
		Rapport de projet
	--> RONDIER.pdf
		Explication sur les tables de la base de donnees
	--> RONDIER.mwb
		Fichier MySQL- Workbench Pour la creation de notre base
	--> RONDIER.png
		Image de la base de donnee avec chaque table numerote
Dans le dossier SQL :

	|
	|
	--> DML :
		-Le fichier Rondier_DDL.sql
		 Fichier a importer la base, creant les tables et index ect
	--> DML :
		- sous dossier avec chaque script SQL de manipulation de base 

	--> Script_SQL_Ordre.pdf
		fichier pdf instruction de lancement des scripts

Dans le dossier UML :

	|
	|
	--> modele_ronde.pdf
		- UML decrivant la realisation d'un modele de ronde
	--> explications_modele_ronde.pdf
		- Explication des tables de la base de donnees
		
Dans le dossier code_c :

	|
	|
	--> events.txt 
		- Fichier de simulation du baton de rondier
	--> UdpClient.c 
		- Programme c de lancement du client udp
	--> UdpServer.c
		- Programme c du serveur udp

