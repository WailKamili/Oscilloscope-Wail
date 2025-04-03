//**************************************************************
//Prénom, nom, année, classe, école : Wail, Kamili, 2025, 6A, INRACI
//Titre : Oscilloscope portable
//**************************************************************

//Définire les pattes.
#define CLK 3
#define RST 4
#define DQ 2

//Définir des variables.
int N;  //Position du curseur.
int i;


void setup() {

  //Mettre les pattes en sorties ou entrées.
  pinMode(CLK, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(DQ, OUTPUT);
}

void loop() {

  N = 200;
  // Changer la valeur de l'amplification du Signal.

  //Commencer à mettre toutes les entrées à 0.
  digitalWrite(RST, 0);
  digitalWrite(CLK, 0);
  digitalWrite(DQ, 0);

  //Les DS peut commencer à prendre en compte les variations.
  //de la CLK et du DQ.
  delay(10);
  digitalWrite(RST, 1);  // début de la transmission.

  //Les 9 bits de poids faibles ne sont pas utiliser donc.
  //ils sont juste mis à 0.
  for (i = 0; i < 9; i++) {  // envoi du stack bit + POT1 non utilisé.
    digitalWrite(CLK, 1);    // Flanc d'horloge.
    digitalWrite(CLK, 0);
  }

  //Envoie la valeur du niveau d'amplification en binaire au
  //DS1267B.
  for (i = 0; i < 8; i++) {     // envoi du POT0 (Position_curseur) utilisé.
    if (N & (0b10000000 >> i))  // MSB first.
    {
      digitalWrite(DQ, 1);
    } else {
      digitalWrite(DQ, 0);
      digitalWrite(CLK, 1);  // Flanc d'horloge.
      digitalWrite(CLK, 0);
    }
    digitalWrite(DQ, 0);   // non nécessaire test.
    digitalWrite(RST, 0);  // Fin de transmission.
    delay(100);
  }
}
