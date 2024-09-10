#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "partie2.h"
#include"fonctions.h"
#define N 60

#define MEMORY 5000

Segment * creer_segment(int x, long int y, int lignes1){//on crée une nouvelle structure de segment avec 3 entiers en entrée.
    Segment *s=malloc(sizeof(Segment));
    if (s==NULL){
        return NULL;
    } //cas où le malloc échoue.
    s->instruction=x;//on sauvgarde le numéro de l'instruction
    s->parametre=y;//on sauvgarde le paramètre.
    s->suiv=NULL;
    s->lignes=lignes1; //on sauvgarde le numéro de ligne.
    return s;
}

Liste* liste_vide(){ // on crée une nouvelle liste vide pour la structure de segment.
    Liste  *l=malloc(sizeof(Liste ));
    if (l==NULL){
        return NULL;
    }//cas où le malloc échoue.
    l->premier=NULL;
    return l;
}

void Ajouter_segment(Liste * l,int x, long int y , int lignes){
    Segment *s=creer_segment(x,y,lignes);
    if ((l->premier)==NULL){
        l->premier=s;
    }//cas où la liste est vide, la nouvelle structure vient en première position.
    else{ //sinon, on parcourt la liste pour trouver le dernier élément.
       Segment *x2=l->premier;
        while (x2->suiv!=NULL){
            x2=x2->suiv;
        }
        x2->suiv=s;// on ajoute le nouveau segment crée apres le dernier élement de la liste.
    }
}

void liste_libere(Liste *l){//il s'agit d'une fonction qui libère la liste en argument
    Segment *e=l->premier;
    Segment *sauv=NULL;
    while (e!=NULL){
        sauv=e;  //on sauvgarde le segment courant pour le vider après
        e=e->suiv;
        free(sauv);
    }
    free(l);//après avoir vider tous les elements, on vide la liste elle même.
}
void partie2(){
    struct Machine machine;
    machine.pc = 0;
    machine.sp = 0;
    machine.memoire=malloc(sizeof(long int)*5000);// le sujet précise que la machine doit pouvoir stocker un entier signé sur quatre octets donc on utilise le type long int.
    
    // Ouverture du fichier qui contient le programme en langage machine
    FILE* input = fopen("hexa.txt", "r");
    Liste  *l1= liste_vide();
    int ligne=0;
    char instruction[20];
    char param[20];
    while(fscanf(input, "%s %s", instruction, param) != EOF){
        int decimal_instruction;
        int decimal_param;
        sscanf(param, "%x", &decimal_param);
        sscanf(instruction, "%x", &decimal_instruction);
        Ajouter_segment(l1,decimal_instruction,decimal_param,ligne);
        ligne++;
    }
    while(machine.pc<ligne){
        long int z=0;
        Segment *e=l1->premier;
        while ((e->lignes)!=machine.pc){
            e=e->suiv;
        }//on parcourt la liste jusqu'à trouver la provhiane instruction à exécuter
        switch (e->instruction) {
            case 0: // push x : empile le contenu de l'adresse x
                machine.memoire[machine.sp] = machine.memoire[e->parametre];
                machine.sp++;
                break;
            case 1: // push# i : empile la valeur i
                machine.memoire[machine.sp] = e->parametre;
                machine.sp++;
                break;
            case 2: // ipush : empile le contenu de l'adresse n  (n:valeur du sommet de la pile)
                z=machine.memoire[--machine.sp];
                machine.memoire[machine.sp] =machine.memoire[z];
            case 3: // pop x : met le contenu du haut de la pile à l’adresse x
                machine.sp--;
                machine.memoire[e->parametre] = machine.memoire[machine.sp];
                break;
            case 4: // ipop : met le contenu du haut de la pile -1 à l’adresse n  (n:valeur du sommet de la pile)
                machine.sp--; // pop le haut de la pile
                machine.memoire[machine.memoire[machine.sp]] = machine.memoire[machine.sp - 1]; // stocke la valeur quon a pop dans l'adresse en haut de la pile
                machine.sp--; // deuxieme decrementation
                break;
            case 5: // dup : duplique le sommet de la pile
                machine.memoire[machine.sp] = machine.memoire[machine.sp - 1];
                machine.sp++;
                break;
            case 6: // op i : effectue l’opération indiquée par i
                switch (e->parametre) {
                    case 0: // additionne les deux valeurs au sommet
                        machine.sp--;
                        machine.memoire[machine.sp - 1] += machine.memoire[machine.sp];
                        break;
                    case 1: // soustraction
                        machine.sp--;
                        machine.memoire[machine.sp - 1] -= machine.memoire[machine.sp];
                        break;
                    case 2: // multiplication
                        machine.sp--;
                        machine.memoire[machine.sp - 1] *= machine.memoire[machine.sp];
                        break;
                    case 3: // division entière
                        machine.sp--;
                        if (machine.memoire[machine.sp]==0){
                            printf("erreur: division par 0 impossible");
                            return;
                        }
                        machine.memoire[machine.sp - 1] /= machine.memoire[machine.sp];
                        break;
                    case 4: // modulo
                        machine.sp--;
                        machine.memoire[machine.sp - 1] %= machine.memoire[machine.sp];
                        break;
                    case 5: // inverse la valeur au sommet de la pile
                        machine.memoire[machine.sp - 1] = -machine.memoire[machine.sp - 1];
                        break;
                    case 6: // test d'égalité
                        machine.sp--;
                        if(machine.memoire[machine.sp - 1] == machine.memoire[machine.sp]){
                            machine.memoire[machine.sp - 1] =  0;
                        }
                        else{
                            machine.memoire[machine.sp - 1] =  1;
                        }
                        break;
                    case 7: // test d'inégalité
                        machine.sp--;
                        if((machine.memoire[machine.sp - 1] != machine.memoire[machine.sp])){
                            machine.memoire[machine.sp - 1] =  0;
                        }
                        else{
                            machine.memoire[machine.sp - 1] =  1;
                        }
                        break;
                    case 8: // test >
                        machine.sp--;
                        if((machine.memoire[machine.sp - 1] > machine.memoire[machine.sp])){
                            machine.memoire[machine.sp - 1] =  0;
                        }
                        else{
                            machine.memoire[machine.sp - 1] =  1;
                        }
                        break;
                    case 9: // test >=
                        machine.sp--;
                        if((machine.memoire[machine.sp - 1] >= machine.memoire[machine.sp])){
                            machine.memoire[machine.sp - 1] =  0;
                        }
                        else{
                            machine.memoire[machine.sp - 1] =  1;
                        }
                        break;
                    case 10: // test <
                        machine.sp--;
                        if((machine.memoire[machine.sp - 1] < machine.memoire[machine.sp])){
                            machine.memoire[machine.sp - 1] =  0;
                        }
                        else{
                            machine.memoire[machine.sp - 1] =  1;
                        }
                        break;
                    case 11: // test <=
                        machine.sp--;
                        if((machine.memoire[machine.sp - 1] <= machine.memoire[machine.sp])){
                            machine.memoire[machine.sp - 1] =  0;
                        }
                        else{
                            machine.memoire[machine.sp - 1] =  1;
                        }
                        break;
                    case 12: // et logique bit à bit
                        machine.sp--;
                        machine.memoire[machine.sp - 1] &= machine.memoire[machine.sp];
                        break;
                    case 13: // ou logique bit à bit
                        machine.sp--;
                        machine.memoire[machine.sp - 1] |= machine.memoire[machine.sp];
                        break;
                    case 14: // ou-exclusif logique bit à bit
                        machine.sp--;
                        machine.memoire[machine.sp - 1] ^= machine.memoire[machine.sp];
                        break;
                    case 15: // non logique bit à bit
                        machine.memoire[machine.sp - 1] = ~machine.memoire[machine.sp - 1];
                        break;
                }
                break;
            case 7: // jmp adr : additionne Adr au registre PC
                machine.pc += e->parametre;
                break;
            case 8: // jpz adr : dépile un élément
                machine.sp--;
                if (machine.memoire[machine.sp]== 0) {
                    machine.pc +=e->parametre;
                }
                break;
            case 9: // rnd x: met au sommet de la pile un nombre aléatoire entre 0 et x-1
                machine.memoire[machine.sp] = rand() % e->parametre; //la fonction rand() % parametre retourne un entier entre 0 et parametre - 1
                machine.sp++;
                break;
            case 10: // read x : demande à l'utilisateur une valeur qui sera mise dans la variable à l'adresse x
                printf("Entrez un entier dans l'intervalle [-2147483648;2147483647] : ");
                long long x=0;
                scanf("%lld", &x);//récuperer la valeure entrée par l'utilisateur.
                if (x>2147483647 || x<-2147483648){
                    fprintf(stderr, "Erreur valeure dépasse taille maximale des entiers signés sur 4 octets\n");
                    return;
                } //s'assurer que cette valeur appartient bien à l'intervalle des entiers signés sur quatre octets [((16^8)/2)-1;(16^8)/2]
                long int y;
                y= (long int)x;// si jamais x est dans l'intervalle du long int on convertit son type
                machine.memoire[e->parametre]=y;
                break;
            case 11: // write x : affiche le contenu de la variable d’adresse x
                printf("%d\n", machine.memoire[e->parametre]);
                break;
            case 12: // call adr
                machine.memoire[machine.sp] = machine.pc+1;
                machine.sp++;
                machine.pc += e->parametre;
                break;
            case 13: // ret
                machine.sp--;
                machine.pc = machine.memoire[machine.sp];
                break;
            case 14: // halt : fin du programme
                free(machine.memoire);
                liste_libere(l1);
                return;
        }
        machine.pc+=1;//on passe à l'instruction suivante
    }
    printf("Erreur: votre fichier ne contient pas d'instruction halt");
    free(machine.memoire);// dans le cas où la fonction retourne pas car il n'y a pas de halt, on libère quand même machine.memoire et la liste l1.
    liste_libere(l1);
}
    

