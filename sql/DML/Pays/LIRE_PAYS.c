#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	char line[200];
	char *token;
	FILE *file1=fopen("pays.csv","r");
	FILE *file2=fopen("Pays_DML.sql","w");
		

	while (!feof(file1))
	{
		fgets(line,200,file1);
		token=strtok(line,",");
		token=strtok(NULL,",");
		token=strtok(NULL,",");
		token=strtok(NULL,",");
		token=strtok(NULL,",");
		if(token==0){
		break;
		}
		printf("INSERT INTO Pays(Pays) VALUES(%s);\n",token);
		fprintf(file2,"INSERT INTO Pays(Pays) VALUES(%s);\n",token);
	}
	fclose(file1);
	fclose(file2);
	return 0;
}

