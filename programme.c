#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	char *hex;
	char *braille;
} Braille;

int remplacerbraille(Braille carac[],int tailleCarac,FILE *in,FILE *out){
	int i = 0, now_replacing = 0,j,converDansBalise,k;
	char mot[4096]; //tableau de char qui va contenir chaque mot lu
	while (!feof(in)) //tant que la lecture du fichier n'est pas terminée faire :
	{
		fgets(mot,sizeof(mot),in);

		for (i = 0; mot[i] != '\0';) 
		{
			converDansBalise=1;
			if (strncmp(&mot[i], "|(", 2) == 0)
			{
				now_replacing = 1;
				i += 2;
				fprintf(out, "|(");
				continue;
			}
			if (strncmp(&mot[i], "|)", 2) == 0)
			{
				now_replacing = 0;
				i += 2;
				fprintf(out, "|)");
				continue;
			}
			if (now_replacing == 1)
			{
				for (j=0;j<tailleCarac;j++)
				{
					if (strncmp(&mot[i], carac[j].hex, strlen(carac[j].hex)) == 0)
					{
						for (k=0;k<strlen(carac[j].braille);k++)
						{
							fputc(carac[j].braille[k],out);
						}
							i+= strlen(carac[j].hex);
						converDansBalise=0;
						break;
					}
				}
				if (converDansBalise == 1){ fputc(mot[i], out); i++;}
			}
			else
			{
				fputc(mot[i], out);
				i++;
			}
//			if (mot[i] == '\0') fputc('', out);
		}
	}
		return 0;
}

int main(void)
{
	char nomFichier[50],nouveauFichier[50],commande[60]="pdftotext -enc UTF-8 ";
    Braille carac[] = {
		{"\xe2\x88\x9e","¤c"}, // ∞
            {"\x3d","\""}, //=
            {"\xe2\x88\xaa","¸!"}, // ∪
            {"\x2b","!"}, //+
            {"\xc3\x97","*"}, //×
            {"\x3e","@"}, // >
            {"\x3c","2"}, // <
            {"\x5d","à"}, // [
            {"\x5b","ù"}, // ]
            {"\xe2\x8b\x82","¤!"}, //⋂
            {"\xe2\x89\x85","null"}, //≅
            {"\x3d\x3e","¤2"}, // <=
            {"\xe2\x88\x9a","@"},// √
            {"\xe2\x88\xab","ç"}, // ∫
            {"\xe2\x88\xa7","é"}, // ∧
            {"\xe2\x88\xa8","¸"}, // ∨
            {"\xe2\x88\x8b","¤\""}, // ∋
            {"\xe2\x88\x85","¤0"}, // ∅
            {"\xe2\x88\xa5","¸8"}, // ∥
    };
	int tailleCarac=19;
    FILE *in, *out;
	printf("saisissez le nom du fichier que vous voulez convertir:");
	fgets(nomFichier,sizeof(nomFichier),stdin);
	strcat(commande,nomFichier);
	system(commande);
	nomFichier[strlen(nomFichier)-5]='\0';
	strcat(nomFichier,".txt");

    in = fopen(nomFichier, "r"); // fichier qui contient le texte à transformer
	if (in==NULL) return 0;
	strcpy(nouveauFichier,nomFichier);
	nouveauFichier[strlen(nouveauFichier)-4]='\0';
	strcat(nouveauFichier,"-braille.txt");
    out = fopen(nouveauFichier, "w");
	if (out==NULL) return 0;
	if (remplacerbraille(carac,tailleCarac,in,out)==-1) printf("erreur dans la convertion");
	fclose(in);
	fclose(out);
	remove(nomFichier);
	return 0;
}
