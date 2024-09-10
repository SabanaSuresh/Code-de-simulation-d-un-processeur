
#include "fonctions.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define N 60
Etiquette* creer_etiquette(char x[N], int lignes1){ // fonction pour creer une etiquette.
    Etiquette *m=malloc(sizeof(Etiquette));
    if (m==NULL){
        return NULL;
    } // cas où le malloc échoue.
    int i=0;
    while (i<N){
        m->chaine[i]=x[i];
        i++;
    }
    m->chaine[i]='\0';
    m->suiv=NULL;
    m->ligne=lignes1; // sauvgarde de la ligne de l'etiquette.
    return m;
}

Adresses* Adresses_vide(){  // fonction pour creer une liste chainée vide nommée "Adresses".
    Adresses *l=malloc(sizeof(Adresses)); //allocation de l'espace mémoire nécessaire
    if (l==NULL){
        return NULL;
    }// cas où le malloc échoue
    l->premier=NULL;
    return l;
}

void Ajout_Adresses(Adresses* l, char x[N], int lignes1){ // ajouter une étiquette à la liste chainée.
    Etiquette *x1=creer_etiquette(x, lignes1);
    if ((l->premier)==NULL){
        l->premier=x1;
    } // cas particulier où la liste est vide: on ajoute l'étiquette en première postition.
    else{
        Etiquette *x2=l->premier;
        while (x2->suiv!=NULL){
            x2=x2->suiv;
        }
        x2->suiv=x1;
    } // ajout l'étiquette à la fin de la liste.
}
void listelibere(Adresses *l){//fonction pour libérer l'espace mémoire alloué par les mallocs
    Etiquette *e=l->premier;
    Etiquette *sauv=NULL; //étiquette de sauvgarde qui serait libérer dans la boucle while
    while (e!=NULL){
        sauv=e;
        e=e->suiv;
        free(sauv);
    }
    free(l);// après avoir libérer tout ces étiquettes on libère la liste elle-même
}


char *sansespaces(char c[]){ //fonction qui se retourne une chaine passée en argument après avoir supprimer l'espace au début.
    char *buff3=malloc(sizeof(char)*N);
    int k1=0;
    int k2=0;
    for (int i=0;c[i]==' ';i++){
        k2++;
    } //on retrouve l'indice du premier caractère différent de l'espace.
    for (int j=k2; c[k2]!='\0' && c[k2]!='\n'  ;j++){ //la nouvelle chaine commence a l'indice k2
        buff3[k1]=c[j];
        k1++;
        k2++;
    }
    buff3[k1]='\0';
    return buff3;
}
