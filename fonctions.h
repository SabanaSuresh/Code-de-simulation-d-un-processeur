#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define N 60
typedef struct etiquette {
    char chaine[N];
    int ligne;
    struct etiquette *suiv;
} Etiquette; // enregistrement des adresses dans des structures en sauvagradant leurs lignes.

typedef struct {
    Etiquette *premier;
} Adresses; // liste simplement chainée pour enregistrer les adresses.

char *sansespaces(char c[]);
void listelibere(Adresses *l);
void Ajout_Adresses(Adresses* l, char x[N], int lignes1);
Adresses* Adresses_vide();
Etiquette* creer_etiquette(char x[N], int lignes1);
