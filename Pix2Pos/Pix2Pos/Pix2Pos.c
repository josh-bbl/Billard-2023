// Pix2Pos.c : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int Ballsize;
int BallminScore;

struct dimensions_billard{
        long int Lmin;
        long int Lmax;
        long int Cmin;
        long int Cmax;
    } ;

 struct couleur{
        long int Rmin;
        long int Rmax;
        long int Gmin;
        long int Gmax;
        long int Bmin;
        long int Bmax;
        };

struct range{
        struct couleur Red;
        struct couleur Yellow;
        struct couleur White;
        struct couleur Background;
    } Ranges;

struct dimensions_billard Billardbox;

unsigned int** Pix_tab;

unsigned int** pixnbtocoord(unsigned int array[], unsigned int nb_of_elmts, unsigned int largeur, unsigned int hauteur) {
        unsigned int** Pix_coord = (unsigned int**)malloc(largeur * sizeof(unsigned int*));
        for (unsigned int i = 0; i < largeur; i++) {
            Pix_coord[i] = (unsigned int*)malloc(hauteur * sizeof(unsigned int));
            for (unsigned int j = 0; j < hauteur; j++) {
                unsigned int index = i + j * largeur;
                if (index < nb_of_elmts) {
                    Pix_coord[i][j] = array[index];
                }
            }
        }
        return Pix_coord;
    }

typedef struct {
    long int red;
    long int green;
    long int blue;
} RGB;

RGB GetRGBcode(unsigned int value) {
    RGB rgb;


    unsigned int red = (value >> 16) & 0xFF;
    unsigned int green = (value >> 8) & 0xFF;
    unsigned int blue = value & 0xFF;

    char redhexStr[3];
    char greenhexStr[3];
    char bluehexStr[3];

    sprintf(redhexStr, "%02X", red);
    sprintf(greenhexStr, "%02X", green);
    sprintf(bluehexStr, "%02X", blue);

    const char* hexString_red = redhexStr;
    const char* hexString_green = greenhexStr;
    const char* hexString_blue = bluehexStr;

    char* endPtr;

    rgb.red = strtol(hexString_red, &endPtr, 16);
    rgb.green = strtol(hexString_green, &endPtr, 16);
    rgb.blue = strtol(hexString_blue, &endPtr, 16);

    return rgb;
}

int IsRed(long int Red_value, long int Green_value, long int Blue_value){

    if ((Red_value >= Ranges.Red.Rmin) && (Red_value <= Ranges.Red.Rmax) && (Green_value >= Ranges.Red.Gmin) && (Green_value <= Ranges.Red.Gmax) && (Blue_value >= Ranges.Red.Bmin) && (Blue_value <= Ranges.Red.Bmax)){
        return 1;
    }
    else {
        return 0;
    }

}

int IsYellow(long int Red_value, long int Green_value, long int Blue_value){

    if ((Red_value >= Ranges.Yellow.Rmin) && (Red_value <= Ranges.Yellow.Rmax) && (Green_value >= Ranges.Yellow.Gmin) && (Green_value <= Ranges.Yellow.Gmax) && (Blue_value >= Ranges.Yellow.Bmin) && (Blue_value <= Ranges.Yellow.Bmax)){
        return 1;
    }
    else {
        return 0;
    }
}

int IsWhite(long int Red_value, long int Green_value, long int Blue_value){

    if ((Red_value >= Ranges.White.Rmin) && (Red_value <= Ranges.White.Rmax) && (Green_value >= Ranges.White.Gmin) && (Green_value <= Ranges.White.Gmax) && (Blue_value >= Ranges.White.Bmin) && (Blue_value <= Ranges.White.Bmax)){
        return 1;
    }
    else {
        return 0;
    }
}

int IsBackground(long int Red_value, long int Green_value, long int Blue_value){

    if ((Red_value >= Ranges.Background.Rmin) && (Red_value <= Ranges.Background.Rmax) && (Green_value >= Ranges.Background.Gmin) && (Green_value <= Ranges.Background.Gmax) && (Blue_value >= Ranges.Background.Bmin) && (Blue_value <= Ranges.Background.Bmax)){
        return 1;
    }
    else {
        return 0;
    }

}

typedef struct {
    int Red_score;
    int Yellow_score;
    int White_score;
} Scorestruct;

Scorestruct Getsquarescore(unsigned int X_coord, unsigned int Y_coord){

    Scorestruct Squarescore = {0, 0, 0};

    int centerpix_offset;

    if(Ballsize % 2){
        centerpix_offset = (Ballsize / 2) + 1;
    }
    else centerpix_offset = (Ballsize / 2);

    RGB rgb_centerpix = GetRGBcode(Pix_tab[X_coord + centerpix_offset][Y_coord + centerpix_offset]);

    if(IsBackground(rgb_centerpix.red, rgb_centerpix.green, rgb_centerpix.blue)){
        
        return Squarescore;
    }

    for(int i = Y_coord; i < Y_coord + Ballsize; i++){

        for(int j = X_coord; j < X_coord + Ballsize; j++){

            RGB rgb = GetRGBcode(Pix_tab[j][i]);

            if(IsRed(rgb.red, rgb.green, rgb.blue)){

                Squarescore.Red_score++;
            }
            else if(IsYellow(rgb.red,rgb.green,rgb.blue)){

                Squarescore.Yellow_score++;
            }
            else if(IsWhite(rgb.red,rgb.green,rgb.blue)){

                Squarescore.White_score++;
            }
        }
    }

    return Squarescore;

}

struct Highscore {
    long int X_coord;
    long int Y_cood;
    int Highscore;
};

typedef struct {
    struct Highscore RedBall;
    struct Highscore YellowBall;
    struct Highscore WhiteBall;
} Ball_coord;

Ball_coord GetBallcoord(){

    Ball_coord Ballcoord;

    Ballcoord.RedBall.Highscore = 0;
    Ballcoord.RedBall.X_coord = 0;
    Ballcoord.RedBall.Y_cood = 0;

    Ballcoord.YellowBall.Highscore = 0;
    Ballcoord.YellowBall.X_coord = 0;
    Ballcoord.YellowBall.Y_cood = 0;

    Ballcoord.WhiteBall.Highscore =0; 
    Ballcoord.WhiteBall.X_coord = 0;
    Ballcoord.WhiteBall.Y_cood = 0;

    for(long int k = 100; k < 394; k++ ){

        for(long int l = 127; l < 727; l++){

            Scorestruct squarescore = Getsquarescore(l,k);

            

            if(squarescore.Red_score < BallminScore){

                squarescore.Red_score = 0;
            }

            if(squarescore.Yellow_score < BallminScore){

                squarescore.Yellow_score = 0;
            }

            if(squarescore.White_score < BallminScore){

                squarescore.White_score = 0;
            }

            if(squarescore.Red_score > Ballcoord.RedBall.Highscore){

                Ballcoord.RedBall.Highscore = squarescore.Red_score;
                Ballcoord.RedBall.X_coord = l;
                Ballcoord.RedBall.Y_cood = k;
            }

            if(squarescore.Yellow_score > Ballcoord.YellowBall.Highscore){

                Ballcoord.YellowBall.Highscore = squarescore.Yellow_score;
                Ballcoord.YellowBall.X_coord = l;
                Ballcoord.YellowBall.Y_cood = k;
            }

            if(squarescore.White_score > Ballcoord.YellowBall.Highscore){

                Ballcoord.WhiteBall.Highscore = squarescore.White_score;
                Ballcoord.WhiteBall.X_coord = l;
                Ballcoord.WhiteBall.Y_cood = k;
            }

            if(Ballcoord.RedBall.Highscore < BallminScore) {
                Ballcoord.RedBall.Highscore = 0;
                Ballcoord.RedBall.X_coord = -1;
                Ballcoord.RedBall.Y_cood = -1;
            }

            if(Ballcoord.YellowBall.Highscore < BallminScore){
                Ballcoord.YellowBall.Highscore = 0;
                Ballcoord.YellowBall.X_coord = -1;
                Ballcoord.YellowBall.Y_cood = -1;
            }

            if(Ballcoord.WhiteBall.Highscore < BallminScore){
                Ballcoord.WhiteBall.Highscore = 0;
                Ballcoord.WhiteBall.X_coord = -1;
                Ballcoord.WhiteBall.Y_cood = -1;
            }



        }
    }

    return Ballcoord;
}



int main(int argc,char* argv[])
{
    
   if(argc != 30){
        fprintf(stderr, "Pas le bon nombre d'arguments\n");
        return -1;
    }

    int arg[30];

    for (int i = 1; i < 30; i++) {

        arg[i] = atoi(argv[i]);
    }
    

    //Initialisation de la taille du billard
    Billardbox.Lmin = arg[1];
    Billardbox.Lmax = arg[2];
    Billardbox.Cmin = arg[3];
    Billardbox.Cmax = arg[4];
    
    //Initailisation des ranges rouges 
    Ranges.Red.Rmin = arg[5];
    Ranges.Red.Rmax = arg[6];
    Ranges.Red.Gmin = arg[7];
    Ranges.Red.Gmax = arg[8];
    Ranges.Red.Bmin = arg[9];
    Ranges.Red.Bmax = arg[10];

    //Initailisation des ranges jaunes
    Ranges.Yellow.Rmin = arg[11];
    Ranges.Yellow.Rmax = arg[12];
    Ranges.Yellow.Gmin = arg[13];
    Ranges.Yellow.Gmax = arg[14];
    Ranges.Yellow.Bmin = arg[15];
    Ranges.Yellow.Bmax = arg[16];

    //Initailisation des ranges blanches
    Ranges.White.Rmin = arg[17];
    Ranges.White.Rmax = arg[18];
    Ranges.White.Gmin = arg[19];
    Ranges.White.Gmax = arg[20];
    Ranges.White.Bmin = arg[21];
    Ranges.White.Bmax = arg[22];

    //Initailisation des ranges du tapis
    Ranges.Background.Rmin = arg[23];
    Ranges.Background.Rmax = arg[24];
    Ranges.Background.Gmin = arg[25];
    Ranges.Background.Gmax = arg[26];
    Ranges.Background.Bmin = arg[27];
    Ranges.Background.Bmax = arg[28];

    Ballsize = arg[29];
    BallminScore = 50;

    if((Ballsize < 5) || (Ballsize > 25)){

        fprintf(stderr, "Taille de boule incorrecte\n");
        return -1;
    }

    for(int i = 1; i < 30; i++){

        if(arg[i] < 0){
            fprintf(stderr, "Parametre No %d est invalide : %d (taille negative)\n",i, arg[i]);
            return -1;
        }
    }

    if ((Ranges.Red.Rmin != 91) || (Ranges.Red.Rmax!=255) || (Ranges.Red.Gmin!=0) || (Ranges.Red.Gmax!=80) || (Ranges.Red.Bmin!=0) || (Ranges.Red.Bmax!=130)){
        fprintf(stderr,"Range de la Boule Rouge errone");
        return -1;
    }

    if ((Ranges.Yellow.Rmin != 170) || (Ranges.Yellow.Rmax!=255) || (Ranges.Yellow.Gmin!=176) || (Ranges.Yellow.Gmax!=255) || (Ranges.Yellow.Bmin!=0) || (Ranges.Yellow.Bmax!=175)){
        fprintf(stderr,"Range de la Boule Jaune errone");
        return -1;
    }

    if ((Ranges.White.Rmin != 100) || (Ranges.White.Rmax!=255) || (Ranges.White.Gmin!=150) || (Ranges.White.Gmax!=255) || (Ranges.White.Bmin!=150) || (Ranges.White.Bmax!=255)){
        fprintf(stderr,"Range de la Boule Blanche errone");
        return -1;
    }

    if ((Ranges.Background.Rmin != 39) || (Ranges.Background.Rmax!=62) || (Ranges.Background.Gmin!=91) || (Ranges.Background.Gmax!=116) || (Ranges.Background.Bmin!=202) || (Ranges.Background.Bmax!=255)){
        fprintf(stderr,"Range du tapis du billard errone");
        return -1;
    }



    
    FILE *pixmap = fopen("Pixmap(218).bin", "rb");

    if (pixmap == NULL) {
        fprintf(stderr, "Le fichier n'a pas pu etre ouvert!\n");
        return -1;
    }

    fseek(pixmap, 0, SEEK_END);
    long taille_pixmap = ftell(pixmap);
    fseek(pixmap, 0, SEEK_SET);

    printf("taille pixmap : %d\n", taille_pixmap);

    unsigned int *Hauteur = (unsigned int*)malloc(sizeof(unsigned int));
    unsigned int *Largeur = (unsigned int*)malloc(sizeof(unsigned int));


    if ((Hauteur == NULL) || (Largeur == NULL)){
        fprintf(stderr, "Echec de la Sauvegarde des dimensions!\n");
        free(Hauteur);
        free(Largeur);
        fclose(pixmap);
        return -1;
    }

    fread(Largeur, sizeof(unsigned int), sizeof(unsigned int), pixmap);
    fseek(pixmap, sizeof(unsigned int), SEEK_SET);
    fread(Hauteur, sizeof(unsigned int), sizeof(unsigned int), pixmap);
    fseek(pixmap, 0, SEEK_SET);

    printf("Hauteur : %d\n", *Hauteur);
    printf("Largeur : %d\n", *Largeur);

    if (((*Hauteur) < 100) || ((*Hauteur) > 500)){
        fprintf(stderr, "dimensions du billard incorrectes (Hauteur)\n");
        free(Hauteur);
        free(Largeur);
        fclose(pixmap);
        return -1;
    }

    if (((*Largeur) < 500) || ((*Largeur) > 1000)){
        fprintf(stderr, "dimensions du billard incorrectes (Largeur)\n");
        free(Hauteur);
        free(Largeur);
        fclose(pixmap);
        return -1;
    }

    if ((Billardbox.Lmax > (*Hauteur)) || (Billardbox.Lmin < 100) || (Billardbox.Lmax > 500)){
        fprintf(stderr, "dimensions du billard incorrectes (Hauteur)\n");
        free(Hauteur);
        free(Largeur);
        fclose(pixmap);
        return -1;
    }

    if ((Billardbox.Cmax > (*Largeur)) || (Billardbox.Cmin < 100) || (Billardbox.Cmax > 1000)){
        fprintf(stderr, "dimensions du billard incorrectes (Largeur)\n");
        free(Hauteur);
        free(Largeur);
        fclose(pixmap);
        return -1;
    }

    if ((Billardbox.Lmax - Billardbox.Lmin) < 250) {
        fprintf(stderr, "dimensions du billard incorrectes (Hauteur)\n");
        free(Hauteur);
        free(Largeur);
        fclose(pixmap);
    }

    if ((Billardbox.Cmax - Billardbox.Cmin) < 550) {
        fprintf(stderr, "dimensions du billard incorrectes (Hauteur)\n");
        free(Hauteur);
        free(Largeur);
        fclose(pixmap);
    }


    unsigned int *Pix = (unsigned int*)malloc(taille_pixmap - (2*sizeof(unsigned int)));

    if (Pix == NULL){
        fprintf(stderr,"Echec de l'allocation de memoire pour Pixmap\n");
        free(Hauteur);
        free(Largeur);
        free(Pix);
        fclose(pixmap);
        return -1;
    }


    unsigned int Nb_Pixels = (*Hauteur)*(*Largeur);

    printf("Nb Pixels : %d\n", Nb_Pixels);

    fseek(pixmap, 2*sizeof(unsigned int), SEEK_SET);
    
    size_t taille_lue = fread(Pix, sizeof(unsigned int), Nb_Pixels, pixmap);

    printf("taille lue : %zu\n", taille_lue);



    if (taille_lue != Nb_Pixels) {
        if (feof(pixmap)) {
            fprintf(stderr,"End of file reached prematurely.\n");
            free(Hauteur);
            free(Largeur);
            free(Pix);
            fclose(pixmap);
            return -1;
        }
         else if (ferror(pixmap)) {
            fprintf(stderr,"Error occurred while reading the file.\n");
            free(Hauteur);
            free(Largeur);
            free(Pix);
            fclose(pixmap);
            return -1;
        }
         else {
            fprintf(stderr,"Unexpected number of elements read from the file.\n");
            free(Hauteur);
            free(Largeur);
            free(Pix);
            fclose(pixmap);
            return -1;
        }
    }



    if (taille_lue != Nb_Pixels){
        fprintf(stderr,"Lecture erronee de pixmap!\n");
        free(Hauteur);
        free(Largeur);
        free(Pix);
        fclose(pixmap);
        return -1;
    }

    if (((*Hauteur)*(*Largeur) * sizeof(unsigned int)) > (taille_lue * sizeof(unsigned int))){

        fprintf(stderr,"Pixels Manquants!\n");
        free(Hauteur);
        free(Largeur);
        free(Pix);
        fclose(pixmap);
        return -1;
    }
    if (((*Hauteur)*(*Largeur) * sizeof(unsigned int)) < (taille_lue * sizeof(unsigned int))){

        printf("Presence de Pixels supplementaires\n");
    }
    if (((*Hauteur)*(*Largeur) * sizeof(unsigned int)) == (taille_lue * sizeof(unsigned int))){

        printf("nombre de pixels correct\n");
    }

    

    Pix_tab = (unsigned int**)malloc(*Largeur * sizeof(unsigned int*));

    for (unsigned int i = 0; i < *Largeur; i++) {
        Pix_tab[i] = (unsigned int*)malloc(*Hauteur * sizeof(unsigned int));
    }

    Pix_tab = pixnbtocoord(Pix, Nb_Pixels, *Largeur, *Hauteur);

    Ball_coord Ballcoord = GetBallcoord();

    printf(" Red : %ld  %ld  %d\n", Ballcoord.RedBall.X_coord, Ballcoord.RedBall.Y_cood, Ballcoord.RedBall.Highscore);
    printf(" Yellow : %ld  %ld  %d\n", Ballcoord.YellowBall.X_coord, Ballcoord.YellowBall.Y_cood, Ballcoord.YellowBall.Highscore);
    printf(" White : %ld  %ld  %d\n", Ballcoord.WhiteBall.X_coord, Ballcoord.WhiteBall.Y_cood, Ballcoord.WhiteBall.Highscore);


    free(Hauteur);
    free(Largeur);
    free(Pix);
    free(Pix_tab);
    fclose(pixmap);

    return 0;





}

