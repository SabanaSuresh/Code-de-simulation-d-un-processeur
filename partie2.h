#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define N 60
#define MEMORY 5000
struct Machine {
    int pc; // Registre PC
    int sp; // Registre SP
    int *memoire; // Espace mémoire de travail
};

typedef struct segment{//creation d'une structure pour sauvgarder l'instruction, parametre et numéro de ligne pour chaque ligne du fichier "hexa.txt"
    int instruction;
    long int parametre ;
    int lignes;
    struct segment *suiv;
} Segment;

typedef struct {// crée une liste chainée dont ses éléments sont des segments
    Segment *premier;
} Liste;

Segment * creer_segment(int x, long int y, int lignes1);
Liste* liste_vide();
void Ajouter_segment(Liste* l,int x, long int y , int lignes);
void liste_libere(Liste *l);
void partie2();

