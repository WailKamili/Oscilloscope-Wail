//*****************************************************
//Programme_de_l'Oscilloscope
//Wail Kamili 6 ème Eo Inraci 2024-25
// Projet : afficher un signal sur un écran
//Hardware : Arduino Nano + DS1267BS-010+ + MAX118CP1+
//Date : 21/05/2025
//Librairie : Arduino Nano
//*****************************************************
 
// Définir des constantes
#define FLANC_MONTANT 0;
#define FLANC_DESCENDANt 1;
#define DQ 2
#define CLK 3
#define RST 4
#define RD 5
#define INT 9
#define WR 10
 
//Définir les variables à virgule
float Echantillon[250];
float SynchroLevel = 2.5;  // Synchro à 50%
 
//Les variables
int p;
int i;
int Curseur = 50;
int Tempo = 0;
int caractere_recu;
int x;
 
byte Pente = FLANC_MONTANT;
 
//Enregistrer la valeur de la tension pour le convertir en valeur binaire
byte Get_sample() {
  byte Sample;
  // En utilisant les registres internes ==> rapide
  PORTB &= 0b11111011;  // WR à 0  Début de conversion
  PORTB |= 0b00000100;  // WR à 1
  while (PINB & 0b00000010)
    ;  // INT : Attendre que la conversion soit finie
  PORTD &= 0b11011111;                                 // RD à 0  Récupération de la donnée
  Sample = (PIND & 0b11000000) + (PINC & 0b00111111);  // Les 6 LSB sont dans le port C et les 2 MSB sont dans le port D
  PORTD |= 0b00100000;                                 // RD à 1
  return Sample;
}
 
// Changer la position du curseur dans le programme
int mettre_a_jour_le_potentiometre() {
  digitalWrite(RST, 0);
  digitalWrite(CLK, 0);
  digitalWrite(DQ, 0);
  delay(10);
  digitalWrite(RST, 1);      // début de la transmission
  for (i = 0; i < 9; i++) {  // envoi du stack bit + POT1 non utilisé
    digitalWrite(CLK, 1);    // Flanc d'horloge
    digitalWrite(CLK, 0);
  }
  for (i = 0; i < 8; i++) {             // envoi du POT0 (Position_curseur) utilisé
    if (Curseur & (0b10000000 >> i))  // MSB first
      digitalWrite(DQ, 1);
    else
      digitalWrite(DQ, 0);
    digitalWrite(CLK, 1);  // Flanc d'horloge
    digitalWrite(CLK, 0);
  }
  digitalWrite(DQ, 0);   // non nécessaire test
  digitalWrite(RST, 0);  // Fin de transmission
}
 
void setup() {
 
//Initialer toutes les pattes
  Serial.begin(115200);
  while (!Serial)
    ;
  pinMode(INT, INPUT);
  pinMode(CLK, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(DQ, OUTPUT);
  pinMode(RD, OUTPUT);
  pinMode(WR, OUTPUT);
  mettre_a_jour_le_potentiometre();
}
 
void loop() {
 
//Prendre 50 échantillons et les envoyer selon la synchro
  if (Pente == 0) {
    p = 0;  // afficher les 50 premiers points si pas de synchro
    for (int i = 0; i < 250; i++) {
      Echantillon[i] = Get_sample() / 51.0; // diviser par 51 pour le mettre en base 5 et non 255
      delay((int)(pow(Tempo, 2) * 0.4)); // Pour que la période se divise par 2 à chaque fois que l'on augmente la valeur
      // pow(Tempo, 2) correspond à x². J'ai trouvé ce calcul via ChatGPT
    }
    for (int i = 0; i < 200; i++) {
      if ((Echantillon[i] < SynchroLevel) && (Echantillon[i + 1] >= SynchroLevel)) {
        p = i;
        // p est la valeur de l’échantillon correcte pour la synchro
      }
    }
    // Affichage des 50 points à partir de p
    for (int i = p; i < p + 50; i++) {
      Serial.print(Echantillon[i]);
      Serial.print(" ");
      Serial.print(5);
      Serial.print(" ");
      Serial.print(SynchroLevel);
      Serial.print(" ");
      Serial.println(0);
    }
    delay(100);
  }
 
//Les différentes options
  caractere_recu = Serial.read();
  switch (caractere_recu) {
    case 'p':
      Tempo++;//Réduire la fréquence d'échantillonage
      break;
 
    case 'm':
      Tempo--;
      break;
 
    case 'b':
      Curseur = 10 + Curseur;
      mettre_a_jour_le_potentiometre();// Mettre à jour le potentiomètre pour qu'il change la position du curseur
      break;
 
    case 'h':
      Curseur = Curseur - 10;
      mettre_a_jour_le_potentiometre();
      break;
 
    case 's':
      SynchroLevel = SynchroLevel + 0.1;
      break;
 
    case 'x':
      SynchroLevel = SynchroLevel - 0.1;
      break;
 
    default:
      break;
  }
}