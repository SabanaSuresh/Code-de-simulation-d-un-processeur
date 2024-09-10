#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"partie2.h"
#include"fonctions.h"
#define N 60
#define MEMORY 5000

int main(int argc, char* argv[]){
    char buff1[N]; // variable pour récupérer les lignes lors de la première lecture du fichier
    char buff[N];// variable pour récupérer les lignes lors de la deuxième lecture du fichier
    int lignes=1; //variable pour compter le numéro de la ligne lors de la première lecture du fichier
    int lignes1=1; //variable pour compter le numéro de la ligne lors de la deuxième lecture du fichier
    Adresses* Adresses1= Adresses_vide(); //crèer une liste vide pour sauvgarder toutes les étiquettes dans le fichier
    FILE* f=fopen(argv[1],"r"); //ouverture du fichier d'entrée en mode lecture pour recupérer toutes les étiquettes dans une première partie.
    if(f == NULL) { // ouverture du fichier echoue.
        printf("Ouverture du fichier impossible!");
        return  EXIT_FAILURE;
    }
    while(fgets(buff1, sizeof(buff1), f) != NULL) { // première boucle while pour récuperer toutes les étiquettes du fichiers dans la liste chainée: Adresses .
        char adresse[N];
        int indicateur1=0; //variable qui serait = à 1 si jamais il ya une adresse sur le ligne lue.
        for (int i=0; buff1[i]!=' '; i++){
            if (buff1[i]==':'){
                indicateur1=1; //nous indique si il ya une etiquette sur le ligne lue.
                for (int j=0;j<i; j++){
                    adresse[j]=buff1[j];
                }
                adresse[i]='\0'; // récuperer l'étiquette dans la chaine de caractères "adresse".
            }
        }
        if (indicateur1==1){
            Etiquette *e=Adresses1->premier;
            while (e!=NULL){ //avant d'ajouter l'étiquette dans la liste chainée on doit verifier qu'elle n'est pas repétée
                if (strcmp((e->chaine),adresse)==0){
                    printf("Erreur dans votre fichier une étiquette repétée à ligne %d\n", lignes1);
                    return  EXIT_FAILURE;
                }
                e=e->suiv;
            }
            Ajout_Adresses(Adresses1, adresse, lignes1); //ajout de l'étiquette lue à la liste "Adresses".
        }
        lignes1++;
    }
    fclose(f);
    FILE *f1=fopen(argv[1],"r");
    if(f1 == NULL) { // ouverture du fichier echoue.
        printf("Ouverture du fichier impossible!");
        return  EXIT_FAILURE;
    }
    FILE* g=fopen("hexa.txt","w"); //ouverture du fichier dans lequel on va traduire les instructions.
    if(g == NULL) { // ouverture du fichier échoue.
        printf("Ouverture du fichier impossible!");
        return  EXIT_FAILURE;
    }
    while (fgets(buff, sizeof(buff), f1) != NULL) {
        char *buff3=malloc(sizeof(char)*N);
        char buff2[N];
        int indicateur=0;
        char souschaine[N]; //variable pour rècuperer la fonction sur une ligne.
        char argument[N];
        char adresse[N]; // variable pour rècuperer l'adresse sur une ligne (si jamais elle existe).
        adresse[0]='\0';
        long long arg;
        int longeur=0;
        int k=0;
        int indice=0;
        int existe=0; //variable qui nous indique si jamais la fonction lue est reconnue.
        buff3=sansespaces(buff);
        for (int i=0; buff3[i]!=' ' && buff3[i]!='\0'; i++){
            souschaine[i]=buff3[i];
            longeur++;
        } // on récupère le premier mot de la chaine dans la variable "souschaine".
        souschaine[longeur]='\0';
        if (souschaine[longeur-1]==':'){ // on verifie si le premier mot est une étiquette.
            indicateur=1;
            int indicead=0;
            for (int i=0; i<longeur;i++){
                adresse[i]=souschaine[i];
                indicead++;
            } //on récupère alors l'étiquette dans la variable "adresse".
            adresse[indicead-1]='\0';
        }
        if (indicateur==1){
            for (int i=strlen(souschaine)+1; buff3[i]!=' ' && buff3[i]!='\0' && buff3[i]!='\n'; i++){
                souschaine[k]=buff3[i];
                k++;
            } // dans le cas où il ya une adresse sur la ligne, on récupère le deuxieme mot dans la variable "souschaine".
            souschaine[k]='\0';
            for (int i=strlen(souschaine)+3+strlen(adresse); buff3[i]!=' ' && buff3[i]!='\0' && buff3[i]!='\n'; i++){
                argument[indice]=buff3[i];
                indice++;
            } //on récupère l'argument dans le cas où une étiquette est présente sur la ligne
            argument[indice]='\0';
        }
        else{
            for (int i=strlen(souschaine)+1; buff3[i]!=' ' && buff3[i]!='\0' && buff3[i]!='\n'; i++){
                argument[indice]=buff3[i];
                indice++;
            }//on récupère l'argument dans le cas où il n'ya pas d'étiquette sur la ligne
            argument[indice]='\0';
        }
        if (strcmp(souschaine,"push")==0){
            existe=1; //l'instruction dans "sous-chaine" existe.
            buff2[0]='0';
            buff2[1]='0';
            fputs(buff2,g); // traduction de l'instruction et l'écriture dans "hexa.txt"
            fputc(' ',g);
            if (strlen(argument)==0){
                fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                return  EXIT_FAILURE;
            } //cas où il ya un argument qui manque
           if (indicateur==0){ //cas où il y'a un argument de plus (on le traite en deux étapes : pour une ligne avec une étiquette et une ligne sans étiquette
                if ((strlen(souschaine)+1+strlen(argument)!=strlen(buff3))){
                    fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                    return  EXIT_FAILURE;
                }
            }
            else{
                if((strlen(souschaine)+strlen(adresse)+3+strlen(argument)!=strlen(buff3))){
                    fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                    return  EXIT_FAILURE;
                }
            }
            arg=atoi(argument); //transformer la chaine de caractere en long long.
            if (arg>=5000 || arg<0){
                fprintf(stderr, "Erreur dans votre fichier:argument dépasse taille dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
            fprintf(g,"%08llx",arg);//écrire l'argument en hexadecimale dans le fichier
            fputc('\n',g);
        }
        if (strcmp(souschaine,"push#")==0){// mêmes commentaires que pour le "push", mais avec des conditions diffèrentes sur les valeurs limites de l'argument.
            existe=1;
            buff2[0]='0';
            buff2[1]='1';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)==0){
                fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
             if (indicateur==0){
                 if ((strlen(souschaine)+1+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
             else{
                 if((strlen(souschaine)+strlen(adresse)+3+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
            arg=atoi(argument);
            if (arg>2147483647 || arg<-2147483648){
                fprintf(stderr, "Erreur dans votre fichier: argument dépasse taille maximale de conversion en hexadecimal dans ligne %d\n", lignes);
                return  EXIT_FAILURE;
            } //il s'agit de l'intervalle de conversion de l'argument en hexadecimale [((16^8)/2)-1;(16^8)/2]
            fprintf(g,"%08llx",arg);//écrire l'argument en hexadecimale dans le fichier
            fputc('\n',g);
        }
        if (strcmp(souschaine,"ipush")==0){
            existe=1;
            buff2[0]='0';
            buff2[1]='2';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)!=0){
                fprintf(stderr, "Erreur dans votre fichier: un argument en plus dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            } // si il ya un argument en plus
            fputc('\n',g);
        }
        if (strcmp(souschaine,"pop")==0){ // mêmes commentaires que pour le "push"
            existe=1;
            buff2[0]='0';
            buff2[1]='3';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)==0){
                fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
            if (indicateur==0){
                 if ((strlen(souschaine)+1+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
             else{
                 if((strlen(souschaine)+strlen(adresse)+3+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
            arg=atoi(argument);
            if (arg>=5000 || arg<0){
                fprintf(stderr, "Erreur dans votre fichier:argument dépasse taille dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
            fprintf(g,"%08llx",arg);//écrire l'argument en hexadecimale dans le fichier
            fputc('\n',g);
        }
        if (strcmp(souschaine,"ipop")==0){
            existe=1;
            buff2[0]='0';
            buff2[1]='4';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)!=0){
                fprintf(stderr, "Erreur dans votre fichier: un argument en plus dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            } // s'il ya un argument en plus
            fputc('\n',g);
        }
        if (strcmp(souschaine,"dup")==0){
            existe=1;
            buff2[0]='0';
            buff2[1]='5';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)!=0){
                fprintf(stderr, "Erreur dans votre fichier: un argument en plus dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            } // s'il ya un argument en plus
            fputc('\n',g);
        }
        if (strcmp(souschaine,"op")==0){
            existe=1;
            buff2[0]='0';
            buff2[1]='6';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)==0){
                fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
            arg=atoi(argument);
            if ((arg<0)|| (arg>15)){
                fprintf(stderr, "Erreur dans votre fichier pas bon numéro d'argument pour l'instruction <op> dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            } // arguments qui n'existent pas
            fprintf(g,"%08llx",arg);//écrire l'argument en hexadecimale dans le fichier
            fputc('\n',g);
        }
        if (strcmp(souschaine,"jmp")==0){
            int indicateur2=0;// variable pour verifier que l'étiquette passée en argument existe
            int difference=0;// variable pour calculer la différence entre la ligne et l'étiquette
            existe=1;
            buff2[0]='0';
            buff2[1]='7';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)==0){
                fprintf(stderr, "Erreur dans votre fichier pas d'adresse à ligne %d\n", lignes);
                return EXIT_FAILURE;
            } //cas où un argument manque
             if (indicateur==0){
                 if ((strlen(souschaine)+1+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
             else{
                 if((strlen(souschaine)+strlen(adresse)+3+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
            Etiquette *e=Adresses1->premier;
            while (e!=NULL){
                if (strcmp(e->chaine,argument)==0){
                    difference=(e->ligne)-lignes-1;
                    indicateur2=1;
                }
                e=e->suiv;
            }// comparer avec toutes les adresses dans la liste.
            if (indicateur2==0){
                fprintf(stderr, "Erreur dans votre fichier étiquette non reconuue dans ligne %d", lignes);
                return EXIT_FAILURE;
            }
            fprintf(g,"%08x",difference);//écrire l'argument en hexadecimale dans le fichier
            fputc('\n',g);
        }
        if (strcmp(souschaine,"jpz")==0){// mêmes commentaires que pour le "jmp"
            int indicateur2=0;
            int difference=0;
            existe=1;
            buff2[0]='0';
            buff2[1]='8';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)==0){
                fprintf(stderr, "Erreur dans votre fichier pas d'adresse à ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
            if (indicateur==0){
                 if ((strlen(souschaine)+1+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
             else{
                 if((strlen(souschaine)+strlen(adresse)+3+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
            Etiquette *e=Adresses1->premier;
            while (e!=NULL){
                if (strcmp(e->chaine,argument)==0){
                    difference=(e->ligne)-lignes-1;
                    indicateur2=1;
                }
                e=e->suiv;
            }
            if (indicateur2==0){
                fprintf(stderr, "Erreur dans votre fichier adresse non reconuue dans ligne %d", lignes);
                return EXIT_FAILURE;
            }
            fprintf(g,"%08x",difference);//écrire l'argument en hexadecimale dans le fichier
            fputc('\n',g);
        }
        if (strcmp(souschaine,"rnd")==0){// mêmes commentaires que pour le "push"
            existe=1;
            buff2[0]='0';
            buff2[1]='9';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)==0){
                fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
            if (indicateur==0){
                 if ((strlen(souschaine)+1+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
             else{
                 if((strlen(souschaine)+strlen(adresse)+3+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
            arg=atoi(argument);
            if (arg>=5000 || arg<0){
                fprintf(stderr, "Erreur dans votre fichier: argument dépasse taille dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
            fprintf(g,"%08llx",arg);//écrire l'argument en hexadecimale dans le fichier
            fputc('\n',g);
        }
        
        if (strcmp(souschaine,"read")==0){// mêmes commentaires que pour le "push"
            existe=1;
            buff2[0]='0';
            buff2[1]='a';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)==0){
                fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
            if (indicateur==0){
                 if ((strlen(souschaine)+1+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
             else{
                 if((strlen(souschaine)+strlen(adresse)+3+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
            arg=atoi(argument);
            if (arg>=5000 || arg<0){
                fprintf(stderr, "Erreur dans votre fichier: argument dépasse taille dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
            fprintf(g,"%08llx",arg);//écrire l'argument en hexadecimale dans le fichier
            fputc('\n',g);
        }
        if (strcmp(souschaine,"write")==0){// mêmes commentaires que pour le "push"
            existe=1;
            buff2[0]='0';
            buff2[1]='b';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)==0){
                fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
            if (indicateur==0){
                 if ((strlen(souschaine)+1+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
             else{
                 if((strlen(souschaine)+strlen(adresse)+3+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
            arg=atoi(argument);
            if (arg>=5000 || arg<0){
                fprintf(stderr, "Erreur dans votre fichier: argument dépasse taille dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
            fprintf(g,"%08llx",arg);//écrire l'argument en hexadecimale dans le fichier
            fputc('\n',g);
        }
        if (strcmp(souschaine,"call")==0){// mêmes commentaires que pour le "jmp"
            int indicateur2=0;
            int difference=0;
            existe=1;
            buff2[0]='0';
            buff2[1]='c';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)==0){
                fprintf(stderr, "Erreur dans votre fichier pas d'étiquette à la ligne %d\n", lignes);
                return EXIT_FAILURE;
            }
            if (indicateur==0){
                 if ((strlen(souschaine)+1+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
             else{
                 if((strlen(souschaine)+strlen(adresse)+3+strlen(argument)!=strlen(buff3))){
                     fprintf(stderr, "Erreur dans votre fichier pas bon nombre d'arguments dans ligne %d\n", lignes);
                     return EXIT_FAILURE;
                 }
             }
            Etiquette *e=Adresses1->premier;
            while (e!=NULL){
                if (strcmp(e->chaine,argument)==0){
                    difference=(e->ligne)-lignes-1;
                    indicateur2=1;
                }
                e=e->suiv;
            }
            if (indicateur2==0){
                fprintf(stderr, "Erreur dans votre fichier: étiquette non reconuue dans ligne %d", lignes);
                return EXIT_FAILURE;
            }
            fprintf(g,"%08x",difference);//écrire l'argument en hexadecimale dans le fichier
            fputc('\n',g);
        }
        if (strcmp(souschaine,"ret")==0){
            existe=1;
            buff2[0]='0';
            buff2[1]='d';
            fputs(buff2,g);
            fputc(' ',g);
            if (strlen(argument)!=0){
                fprintf(stderr, "Erreur dans votre fichier: un argument en plus dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            } // s'il ya un argument en plus
            fputc('\n',g);
        }
        if (strcmp(souschaine,"halt")==0){
            existe=1;
            buff2[0]='0';
            buff2[1]='e';
            fputs(buff2,g);
            fputc(' ',g);
            fputs("00000000",g);
            if (strlen(argument)!=0){
                fprintf(stderr, "Erreur dans votre fichier: un argument en plus dans ligne %d\n", lignes);
                return EXIT_FAILURE;
            } // s'il ya un argument en plus
            fputc('\n',g);
        }
       if (existe==0){// cas où la focntion récupèrée dans "souschaine" n'est pas reconnue
            fprintf(stderr, "Erreur dans votre fichier instruction %s n'existe pas dans ligne %d\n", souschaine, lignes);
            return EXIT_FAILURE;
        }
        free(buff3);
        lignes++;
    }
    fclose(f1);
    fclose(g);
    listelibere(Adresses1); //on libère la liste chainée des étiquettes
    //partie2
    partie2();
    return 0;
}
