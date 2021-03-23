#include <main.h>
#include <SD.h>
#include <SPI.h>
int i=0,j=0,nbByte=0;
int etatComme=0,chifreComme=0,chifreCommeP=0,envoieNbTrame,numTrameEnvoie=0;
String motRenvoie="12345678",dataReception;
int numTrame=0;
String str="11111111";
//byte receptionComme[9];
byte receptionComme[8];
//int receptionComme[8];
byte receptionCommeP[8],commeValide=0,tabEvoie[8],trameEnvoie[8];
boolean robotEnMatche= false,passe=false,boleEnvoie=false,dernierTrame=false;
boolean verificationByte1=true,passe1=false,passe2=false,passeS=false;
String chaineTeste="la programation c'est bien";
String mot255="00000000";
char dataRecue[500];
char* fichier;
int endroiChaine=0;


float emplacementRelleRobotX,emplacementRelleRobotY,emplacementRelleRobotA;
int affichage=0,conteurOdometrie=0;
coordoneCartesient emplacementRelleRobot;
int uneSeconde=0,etatStrategie=0,etatDeplacement=0;

Stepper motorDroit(STEP_MOTEUR_GAUCHE, DIR_MOTUER_GAUCHE);   //STEP pin =  2, DIR pin = 3
Stepper motorGauch(STEP_MOTEUR_DROIT,DIR_MOTEUR_DROIT);   //STEP pin =  9, DIR pin = 10

StepControl controller;
File myFile;
const int chipSelect = BUILTIN_SDCARD;

float impultionEncodeurDroit,impultionEncodeurGauche;

void setup() {
  for(i=1;i<8;i++){
    mot255[i]=255;
  }
  SD.begin(chipSelect);
  Serial2.begin(9600);
  Serial.begin(9600);
  MsTimer2::set(1,IntrerrupTimer);
  MsTimer2::start();
  pinMode(ENCODEUR_DROIT_A,INPUT_PULLUP);
  pinMode(ENCODEUR_DROIT_B,INPUT_PULLUP);
  pinMode(ENCODEUR_GAUCHE_A,INPUT_PULLUP);
  pinMode(ENCODEUR_GAUCHE_B,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODEUR_DROIT_A),contageEncodeurDroit,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODEUR_GAUCHE_A),contageEncodeurGauche,RISING);

  // pinMode(RESETE_MOTEUR_GAUCHE, OUTPUT);    //le resete se fait a l'aita bas
  // digitalWrite(RESETE_MOTEUR_GAUCHE, HIGH);
  // pinMode(SLEEP_MOTEUR_GAUCHE, OUTPUT);    //le sleep se met a l'aita bas poru une carte fonctionelle
  // digitalWrite(SLEEP_MOTEUR_GAUCHE, HIGH);
  //
  // pinMode(RESETE_MOTEUR_DROIT, OUTPUT);    //le resete se fait a l'aita bas
  // digitalWrite(RESETE_MOTEUR_DROIT, HIGH);
  // pinMode(SLEEP_MOTEUR_DROIT, OUTPUT);    //le sleep se met a l'aita bas poru une carte fonctionelle
  // digitalWrite(SLEEP_MOTEUR_DROIT, HIGH);

  emplacementRelleRobot.x=0;
  emplacementRelleRobot.y=0;
  emplacementRelleRobot.a=0;
}

void loop(){
  machineEtatStrategie();
  machineEtatDeplacement();
}

void machineEtatStrategie(){
  switch (etatStrategie) {
    case 0://recherche strategie
    break;
    case 1:
    break;
    case 2:
    break;
  }
}

void machineEtatDeplacement(){
  switch (etatDeplacement) {
    case 0://interpretation de la trame strategie
    break;
    case 1://Go
    lancemntDesMoteur(100,-100,50,50,10,10);
    etatDeplacement=0;
    Serial2.println(etatDeplacement);
    break;
    case 2://turn
    break;
  }
}

void lancemntDesMoteur(int nbPasMoteurGauche,int nbPasMoteurDroit,int vitesseMotueurGauche,
  int vitesseMotueurDroit,int axelereationMotueurGauche,int axelereationMotueurDroit){
    motorDroit.setPosition(0);
    motorDroit
     .setMaxSpeed(vitesseMotueurDroit)       // steps/s
     .setAcceleration(axelereationMotueurDroit); // steps/s^2
    motorGauch.setPosition(0);
    motorGauch
     .setMaxSpeed(vitesseMotueurGauche)       // steps/s
     .setAcceleration(axelereationMotueurGauche); // steps/s^2
    motorDroit
      .setTargetRel(nbPasMoteurDroit);
    motorGauch
       .setTargetRel(nbPasMoteurGauche);
   controller.moveAsync(motorDroit,motorGauch);
}

void IntrerrupTimer(){
  conteurOdometrie++;
  uneSeconde++;
  if(conteurOdometrie>=50){
   calculePosition();
    conteurOdometrie=0;

  }
}

void calculePosition(){
  float impD=0,impG=0;
  coordonePolair sortieEncodeur;
  impD=(float)impultionEncodeurDroit;
  impG=(float)impultionEncodeurGauche;
  impultionEncodeurDroit=0;
  impultionEncodeurGauche=0;
  sortieEncodeur.r=impD*COEF_RD+impG*COEF_RG;
  sortieEncodeur.a=(float)atan((impG*COEF_RG*2-impD*COEF_RD*2)/200)*180/PI;
  //passage en coordoneCartesient et incrementation
  if(emplacementRelleRobot.a<0)emplacementRelleRobot.a+=360;
  emplacementRelleRobot.x+=(sortieEncodeur.r*((float)cos(PI*emplacementRelleRobot.a/180)));//(emplacementRelleRobot.a+sortieEcodeur.a*PI+emplacementRelleRobot.a)/2  possibiliter d'amelioration
  emplacementRelleRobot.y+=(sortieEncodeur.r*((float)sin(PI*emplacementRelleRobot.a/180)));
  emplacementRelleRobot.a-=(sortieEncodeur.a);
  //emplacementRelleRobot.a=emplacementRelleRobot.a+PI;
  while((emplacementRelleRobot.a>360||emplacementRelleRobot.a<-360)){
    if(emplacementRelleRobot.a>0)emplacementRelleRobot.a-=360;
    else emplacementRelleRobot.a+=360;
  }
  emplacementRelleRobot.a=emplacementRelleRobot.a;
  // msg.id=0x200;
  // msg.buf[0]='/';
  // msg.buf[5]=0;
  // aEnvoie=(int)emplacementRelleRobot.a;
  // passe3=aEnvoie;
  // msg.buf[4]=((passe3&=(1<<8)   )>>1);
  // if (emplacementRelleRobot.x < 0){
  //   msg.buf[1]=(int)emplacementRelleRobot.x*-1;
  //   xEnvoie=(int)emplacementRelleRobot.x*-1;
  //   msg.buf[5]|=(0x02);
  // }else{
  //   msg.buf[1]=(int)emplacementRelleRobot.x;
  //   xEnvoie=(int)emplacementRelleRobot.x;
  // }
  // passe1=xEnvoie;
  // msg.buf[4]|= ((  passe1&=(0xF00)  )>>8);
  //
  // if (emplacementRelleRobot.y < 0){
  //   msg.buf[2]=(int)emplacementRelleRobot.y*-1;
  //   yEnvoie=(int)emplacementRelleRobot.y*-1;
  //   msg.buf[5]|=(0x04);
  // }else{
  //   msg.buf[2]=(int)emplacementRelleRobot.y;
  //   yEnvoie=(int)emplacementRelleRobot.y;
  // }
  // passe2=yEnvoie;
  // msg.buf[4]|= ((  passe2&=(0x700)  )>>4);
  //
  // msg.buf[3]=(int)emplacementRelleRobot.a;
  // if(distancePas>0)msg.buf[5]|=1;
  // else msg.buf[5]|=0;
  // msg.len=6;
  // Can0.write(msg);
}

void contageEncodeurDroit(){
  if(digitalRead(ENCODEUR_DROIT_B))impultionEncodeurDroit++;
  else impultionEncodeurDroit--;
}

void contageEncodeurGauche(){
  if(digitalRead(ENCODEUR_GAUCHE_B))impultionEncodeurGauche--;
  else impultionEncodeurGauche++;
}
//partie etatStrategie
int comparChen(String chaine1,String chaine2){
  int i=0;
  while(chaine1[i]!='\0'||chaine2[i]!='\0'){
    if(chaine1[i]!=chaine2[i])return 0;
    i++;
  }
  return 1;
}
//Partie relatife a lecriture et a l'anvoie dans la cate sd
void retireLesZeroInutile(){
  i=0;
  while(dataRecue[i]!='\0'){
    i++;
  }
  i--;
  while(dataRecue[i]==0){
    dataRecue[i]='\0';
    i--;
  }
}

boolean pretraitement(){
  if(verificationByte1){
    if(receptionComme[0]<=8){
      //if(receptionCommeP[0]==receptionComme[0]||receptionComme[0]==receptionCommeP[0]+1||(receptionCommeP[0]==8&&receptionComme[0]==0)){
      if(receptionCommeP[0]==receptionComme[0]||receptionComme[0]==receptionCommeP[0]+1||(receptionCommeP[0]==8&&receptionComme[0]==1)){
        chifreCommeP=chifreComme;
        chifreComme=receptionComme[0];
        return true;
      }
    }
    return false;
  }
  return true;
}

boolean conparaison8Byte(byte ch1[8],String ch2){
  for(i=1;i<8;i++){
    if(ch1[i]!=ch2[i])return false;
  }
  return true;
}

boolean conparaison8ByteByte(byte ch1[8],byte ch2[8]){
  for(i=1;i<8;i++){
    if(ch1[i]!=ch2[i])return false;
  }
  return true;
}

boolean envoieData(String chaine){
  if(dernierTrame&&conparaison8ByteByte(receptionComme,trameEnvoie)){
    envoieNbTrame++;
    numTrameEnvoie++;
    Serial2.print((char)envoieNbTrame);
    trameEnvoie[0]=envoieNbTrame;
    for(i=1;i<8;i++){
      Serial2.print((char)255);
    }
    etatComme=0;
    verificationByte1=true;
  }
  if(!dernierTrame){
    if(conparaison8ByteByte(receptionComme,trameEnvoie)){
      numTrameEnvoie=0;
      envoieNbTrame++;
      numTrameEnvoie++;
      Serial2.print((char)envoieNbTrame);
      trameEnvoie[0]=envoieNbTrame;
      for(i=((envoieNbTrame-1)*7);i<envoieNbTrame*7;i++){
        if(chaine[i]=='\0'){
          break;
        }
        trameEnvoie[i-((envoieNbTrame-1)*7)+1]=chaine[i];
        Serial2.print(chaine[i]);
      }
      for(;i<envoieNbTrame*7;i++){
        trameEnvoie[i-((envoieNbTrame-1)*7)+1]='\0';
        Serial2.print('\0');
        dernierTrame=true;
      }
    }else{
      numTrameEnvoie++;
      if(numTrameEnvoie==11)etatComme=0;
      Serial2.print((char)envoieNbTrame);
      trameEnvoie[0]=envoieNbTrame;
      for(i=(envoieNbTrame-1)*7;i<envoieNbTrame*7;i++){
        if(chaine[i]=='\0'){
          break;
        }
        trameEnvoie[i-((envoieNbTrame-1)*7)+1]=chaine[i];
        Serial2.print(chaine[i]);
      }
      for(;i<envoieNbTrame*7;i++){
        trameEnvoie[i-((envoieNbTrame-1)*7)+1]='\0';
        Serial2.print('\0');
      }
    }
  }
  return false;
}

void chaineRenvoie(int nb,String motRenvoie,byte receptionComme2[8]){
  String chaine="12345678";
  for(i=0;i<nb;i++){
    chaine[i]=(char)receptionComme2[i];
  }
  for(i=nb;i<8;i++){
    chaine[i]=motRenvoie[i-nb];
  }
  for(i=0;i<8;i++){
    Serial.print(chaine[i]);
    Serial.print( " ");
    Serial2.print(chaine[i]);
  }
  Serial.println("  ");
}

boolean receptionData(){
  mot255[0]=receptionComme[0];
  chaineRenvoie(8,"",receptionComme);
  if(conparaison8Byte(receptionComme,mot255)){
    return true;
  }
  for(i=1;i<8;i++){
    dataRecue[endroiChaine]=(char)receptionComme[i];
    endroiChaine++;
  }
  return false;
}

void fonctionDeTraitement(){
  if(pretraitement()){
    switch (etatComme) {

      case 0:
        if(conparaison8Byte(receptionComme," dc00000")){
          dernierTrame=false;
          passe=false;
          passe1=true;
          boleEnvoie=false;
          passe2=false;
          passeS=false;
          for(i=0;i<8;i++)trameEnvoie[i]=0;
          envoieNbTrame=0;
          if(!robotEnMatche){
            chaineRenvoie(4,"0val",receptionComme);
            etatComme=1;
            break;
          }else if (robotEnMatche){
            chaineRenvoie(4,"refm",receptionComme);
            break;
          }
        }else{
          chaineRenvoie(4,"0inc",receptionComme);
        }
      break;
      case 1:
        if(conparaison8Byte(receptionComme," dte0000")){//demande ecriture
          chaineRenvoie(4,"0deb",receptionComme);
          etatComme=2;
        }else if(conparaison8Byte(receptionComme," dtr0000")){//demande l'ecture
          chaineRenvoie(4,"0deb",receptionComme);
          etatComme=3;
        }else if(conparaison8Byte(receptionComme," dtd0000")){//damande supresion fichier
          chaineRenvoie(4,"0deb",receptionComme);
          etatComme=4;
        }else {
          chaineRenvoie(4,"reC0",receptionComme);
          etatComme=0;
        }
      break;
      case 2://demande ecriture + ecriture
      if(!passe1) if(receptionData())passe1=true;
      if(conparaison8Byte(receptionComme," 2345678")&&passe1&&passe){
        Serial.println("testes");
        myFile=SD.open("fichier.txt", FILE_WRITE);
        myFile.println("dataRecue");
        myFile.close();
        for(i=0;i<500;i++){
          dataRecue[i]=0;
        }
        endroiChaine=0;
        chaineRenvoie(1,"dte0fin",receptionComme);
        for(i=0;i<500;i++){
          dataRecue[i]=0;
        }
        endroiChaine=0;
        etatComme=0;
      }
        if(!passe2) if(receptionData())passe2=true;
        if(conparaison8Byte(receptionComme," 2345678")&&passe2&&etatComme==2){
          retireLesZeroInutile();
          fichier=dataRecue;

          if(comparChen(dataRecue,"chemin.txt"))chaineRenvoie(1,"dte0int",receptionComme);//chemin interdie
          else if(SD.exists(fichier)){
            SD.remove(fichier);

            chaineRenvoie(1,"dte00ok",receptionComme);
          }else{

            chaineRenvoie(1,"dte00ok",receptionComme);
          }
          passe=true;
          passe1=false;
          passe2=true;
        }
      break;
      case 3://demande de l'ecture
        if(!passe) if(receptionData()){
          passe=true;
          Serial.println("TEST 3");
        }
        if(conparaison8Byte(receptionComme," 2345678")&&passe){//condition sur chemint acsecible
          retireLesZeroInutile();
          fichier=dataRecue;
          if(SD.exists(fichier)){
            SD.open(fichier);
            chaineRenvoie(1,"dtr00tr",receptionComme);
          }else{

            chaineRenvoie(1,"dtr0ntr",receptionComme);
          }
          for(i=0;i<500;i++){
            dataRecue[i]=0;
          }
        }
        if(receptionCommeP[0]!=receptionComme[0]&&conparaison8Byte(receptionComme," dtr1000")){
          //condition sur chemint acsecible
          //chaineRenvoie(1,"dtr0ntr",receptionComme);
          chaineRenvoie(1,"dtr00tr",receptionComme);
          Serial.print("TEST 2");
        }
        if(conparaison8Byte(receptionComme," dtrpret")){
          boleEnvoie=true;
          for(i=0;i<8;i++)trameEnvoie[i]=receptionComme[i];
        }
        if(boleEnvoie){//metre le fichie dans une chaine
          verificationByte1=false;
          envoieData(chaineTeste);
        }
      break;
      case 4://Supressin dun fichier
        if(!passeS) if(receptionData())passeS=true;
        if(conparaison8Byte(receptionComme," 2345678")&&passeS){
          retireLesZeroInutile();
          fichier=dataRecue;
          if(comparChen(dataRecue,"chemin.txt"))chaineRenvoie(1,"dtd0int",receptionComme);
          else if(SD.exists(fichier)){
            SD.remove(fichier);
            chaineRenvoie(1,"dtd0fin",receptionComme);
          }
          else chaineRenvoie(1,"dtd00nt",receptionComme);
          etatComme=0;
          for(i=0;i<500;i++){
            dataRecue[i]=0;
          }
          endroiChaine=0;
        }
      break;
    }
  }else{
    chaineRenvoie(0,"err byte",receptionComme);
  }
}

void serialEvent2() {
  for(i=0;i<0;i++)receptionComme[i]=0;
  Serial2.readBytes(receptionComme,8);
  for(i=0;i<8;i++){
    receptionComme[i]=receptionComme[i];
    mot255[0]=receptionComme[0];
  }
  Serial.println("");
  Serial.print("---");
  Serial.print(receptionComme[0]);
  for(i=1;i<8;i++){
    Serial.print((char)receptionComme[i]);
  }
  Serial.println("");
  fonctionDeTraitement();
  for(i=0;i<8;i++)receptionCommeP[i]=receptionComme[i];
}
