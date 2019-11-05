#define TAILLE 11 
#define elementE 10 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>



int error = 0;

typedef struct maillon
{
    int value;
    struct maillon *suivant;
}maillon_t;

maillon_t* maillon_creer(int value, maillon_t* suivant)
{
    maillon_t*  element = (maillon_t*)malloc(sizeof(maillon_t));
    if(element == NULL)
    {
        printf("ACHTUNG pb de memoire");
        exit(1);
    }
    element->value = value;
    element->suivant = suivant;
    return element;
}

void maillon_liberer(maillon_t* element) 
{
    if (element != NULL)
    {
        maillon_liberer(element->suivant);
        free(element);
    }
}


void maillon_aff(maillon_t* element, int i) 
{
    if ( element != NULL )
    {
        maillon_aff(element->suivant, i+1);
        printf("%d", element->value); 
        if ( i != 0 )
            printf(" ");
    }
}

maillon_t* ajout_addr(maillon_t* maillon, maillon_t* element) 
{
    if ( maillon == NULL ) 
    {
        element->suivant = NULL;
        return element;
    }
    element->suivant = maillon; 
    return element;
}

int entier (char instruction[TAILLE])
{
    int i;
    for ( i = 0; i < TAILLE && instruction[i] != '\0'; i++ )
        if ( instruction[0] != '-' && !isdigit(instruction[i]) )
            return 0;
    return 1;
}

int conv_entier (char instruction[TAILLE])
{
    char *endPtr;
    return strtol(instruction, &endPtr, elementE);
}
maillon_t* maillon_supprimer ( maillon_t* element )
{
    maillon_t* suivant = NULL;
    if ( element != NULL )
    {
        suivant = element->suivant;
        free(element);
    }
    return suivant;
}

maillon_t* maillon_ajouter ( maillon_t* element ) 
{
    if ( element == NULL )
        return NULL;
    if ( element->suivant == NULL ) 
    {
        error = 1;
        element = maillon_supprimer(element);
        return element;
    }
    int gauche = element->suivant->value, droite = element->value;
    maillon_t* suivant = element->suivant->suivant;
    free(element->suivant);
    free(element);
    return ajout_addr(suivant, maillon_creer(gauche+droite, suivant));
}

maillon_t* maillon_soustraire ( maillon_t* element ) 
{
    if ( element == NULL ) 
        return NULL;
    if ( element->suivant == NULL ) 
    {
        error = 1;
        element = maillon_supprimer(element);
        return element;
    }
    int gauche = element->suivant->value, droite = element->value;
    maillon_t* suivant = element->suivant->suivant;
    free(element->suivant);
    free(element);
    return ajout_addr(suivant, maillon_creer(gauche-droite, suivant));
}

maillon_t* maillon_produit ( maillon_t* element )
{
    if ( element == NULL )
        return NULL;
    if ( element->suivant == NULL )
    {
        error = 1;
        element = maillon_supprimer(element);
        return element;
    }
    int gauche = element->suivant->value, droite = element->value;
    maillon_t* suivant = element->suivant->suivant;
    free(element->suivant);
    free(element);
    return ajout_addr(suivant, maillon_creer(gauche*droite, suivant));
}

maillon_t* maillon_division ( maillon_t* element ) 
{
    if ( element == NULL )  
        return NULL;
    if ( element->suivant == NULL )
    {
        error = 1;
        element = maillon_supprimer(element);
        element = maillon_supprimer(element);
        return element;
    }
    int gauche = element->suivant->value, droite = element->value;
    if ( droite == 0 )
    {
        error = 1;
        element = maillon_supprimer(element);
        element = maillon_supprimer(element);
        return element;
    }
    maillon_t* suivant = element->suivant->suivant;
    free(element->suivant);
    free(element);
    return ajout_addr(suivant, maillon_creer(gauche/droite, suivant));
}

maillon_t* maillon_modulo ( maillon_t* element ) 
{
    if ( element == NULL ) 
        return NULL;
    if ( element->suivant == NULL )
    {
        error = 1;
        free(element);
        return element;
    }
    int gauche = element->suivant->value, droite = element->value;
    maillon_t* suivant = element->suivant->suivant;
    free(element->suivant);
    free(element);
    return ajout_addr(suivant, maillon_creer(gauche%droite, suivant));
}


maillon_t* maillon_dupe ( maillon_t* element ) 
{
    if ( element == NULL )
    {
        return NULL;
    }
    return ajout_addr(element, maillon_creer(element->value, element));
}

maillon_t* maillon_echange ( maillon_t* maillon )
{
    if ( maillon == NULL ) 
    {
        return NULL;
    }
    if ( maillon->suivant != NULL )
    {
        int ech = maillon->value;
        maillon->value = maillon->suivant->value;
        maillon->suivant->value = ech;
    }
    return maillon;
}

int maillon_top ( maillon_t * back, maillon_t* maillon, int i )
{
    if ( back == NULL || maillon == NULL ) 
    {
        error = 1;
        return 0;
    }
    if ( i == 1 )
    {
        maillon_t* suivant = maillon->suivant;
        int value = maillon->value;
        free(maillon);
        back->suivant = suivant;
        return value;
    }
    else
         return maillon_top ( maillon, maillon->suivant, i-1 );
}

maillon_t* operation ( maillon_t* maillon, char instruction[TAILLE] )
{
	if ( !strcmp("ADD", instruction) )
        maillon = maillon_ajouter(maillon);
    else if ( !strcmp("SUB", instruction) )
        maillon = maillon_soustraire(maillon);
	else if ( !strcmp("MUL", instruction) )
        maillon = maillon_produit(maillon);
	else if ( !strcmp("DIV", instruction) )
        maillon = maillon_division(maillon);
	else if ( !strcmp("MOD", instruction) )
        maillon = maillon_modulo(maillon);
	 else if ( !strcmp("POP", instruction) )
        maillon = maillon_supprimer(maillon);
	else if ( !strcmp("DUP", instruction) )
        maillon = maillon_dupe(maillon);
    else if ( !strcmp("SWP", instruction) )
        maillon = maillon_echange(maillon);
    else if ( !strcmp("ROL", instruction) )
    {
        int n = maillon->value;
        maillon = maillon_supprimer(maillon);
        n = maillon_top(maillon, maillon->suivant, n-1);
        maillon = ajout_addr(maillon, maillon_creer(n, maillon));
    }
    else
        error = 1;
    return maillon;
}
int main()
{
    int N, i;
    maillon_t* maillon = NULL;
    scanf("%d", &N);
    for ( i = 0; i < N && error != 1; i++ )
    {
        char instruction[TAILLE];
        scanf("%s", instruction);
        if ( entier(instruction) )
            maillon = ajout_addr(maillon, maillon_creer(conv_entier(instruction), maillon));
        else
            maillon = operation(maillon, instruction);
    }
    maillon_aff(maillon, 0);
    if ( error )
    {
        if ( maillon != NULL )
           { 
               printf(" ");
           }       
        printf("ERROR");
    }
    printf("\n");
    maillon_liberer(maillon);
    return 0;
}
