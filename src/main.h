#include <arduino.h>
#include <TeensyStep.h>
#include <MsTimer2.h>

// #include <stdarg.h>



#define COEF_RD 0.103085738//coeficient de la roue droit * son coretctifre / 2
#define COEF_RG 0.105383985//coeficient de la roue gauche * sont corectiffe / 2

#define COEF_ROT 0.0568810239//coeficient pour le calcuele de langle 2 / (pi * l'entraxe)

#define COEF_GO -7.520210565
#define COEF_TURN 1.483529864

#define V_MAX_MOTEUR 8400//1500
#define A_MAX_MOTEUR 2250//150

#define V_BASSE_MOTEUR 4200//1500
#define A_BASSE_MOTEUR 1125//150

#define DIR_MOTUER_GAUCHE     0
#define CAN_TX                3
#define CAN_RX                4
#define STEP_MOTEUR_GAUCHE    2
#define SLEEP_MOTEUR_GAUCHE   1
#define RESETE_MOTEUR_GAUCHE  5
#define DIR_MOTEUR_DROIT      6
#define STEP_MOTEUR_DROIT     7
#define SLEEP_MOTEUR_DROIT    8
#define XBEE_RX               9
#define XBER_TX               10
#define RESETE_MOTEUR_DROIT   11
#define M2                    12

#define SPI_SDA               18
#define SPI_SDL               19

#define M1                    24
#define M0                    25
#define ENABLE                26
#define ENCODEUR_DROIT_A      28
#define ENCODEUR_DROIT_B      27
#define ENCODEUR_DROIT_T      29
#define ENCODEUR_GAUCHE_A     31
#define ENCODEUR_GAUCHE_B     30
#define ENCODEUR_GAUCHE_T     32


//define des machine etatp
#define ATTANTE_STRATEGIE 0
#define ATTENTE_FIN_DEPLACEMENT 1
#define GO                      2
#define TURN                    3




typedef struct{
  float x;
  float y;
  float a;
}coordoneCartesient;
typedef struct{
  float r;
  float a;
}coordonePolair;


void machineEtatStrategie();
//getion de la strategie et du robot
void machineEtatDeplacement();
//getion des deplacement et paticulierment des moteur

void lancemntDesMoteur(int nbPasMoteurGauche,int nbPasMoteurDroit,int vitesseMotueurGauche,
  int vitesseMotueurDroit,int axelereationMotueurGauche,int axelereationMotueurDroit);

void IntrerrupTimer();
//                           fonction appeler tout les temp
//elle permet d'appeler fonction d'odometrie qi vas calculer la position du robot
void calculePosition();
// calcule la position a chaque foit qu'on l'appelle
void contageEncodeurDroit();
//fonction de contage d'impultion encodeur droit
void contageEncodeurGauche();
//fonction de contage d'impultion encodeur gauche
int comparChen(String chaine1,String chaine2);
//compart 2 chaine
void retireLesZeroInutile();
//retiler les zero de la chaine dataRecue a la fin
//truc0000
//devien
//truc
boolean pretraitement();
//fonction permetant de verfier le byte de comme si il est bon
boolean conparaison8Byte(byte ch1[8],String ch2);
//permet de comparée une chainde de 8 byte et un string
boolean conparaison8ByteByte(byte ch1[8],byte ch2[8]);
//peremt de fare la comparaison entre chaien de 8 byte
boolean envoieData(String chaine);
//renvoie de la chaine a au pc
void chaineRenvoie(int nb,String motRenvoie,byte receptionComme2[8]);
//renvoie que les pour les 8 byte
boolean receptionData();
//donne la fin d'une transimition de data
void fonctionDeTraitement();
//foncition qui ger toute l'interface serie 2
void serialEvent2();
// fonction qui fait la reciton des donnai bute
