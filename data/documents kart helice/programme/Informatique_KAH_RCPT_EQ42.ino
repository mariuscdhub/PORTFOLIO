/*********************************************************************/
// Sujet :   Programme de reception NEC pour téléviseur              //
// Auteur :  AUGEREAU F.                                             //
// Date :    04/01/2016                                              //
// Version : 1.0                                                     //
/*********************************************************************/

// inclusion des fichiers header des bibliothèques de fonctions
#include <stdint.h>        // bibliotheque de type de variable
#include <arduino.h>       // bibliotheque de fonctions arduino
#include <Servo.h>         // bibliotheque servomoteur/moteur brushless
#include "NEC.h"           // bibliotheque de fonctions NEC

// definition des constantes du programme
#define RECEPTEUR_Pin  8
#define BUZZER_Pin     5
#define SERVO_Pin      9
#define MOTEUR_Pin     10
#define LEDBLEUE_Pin   1
#define AdresseNEC     0x20

Servo servomoteur;
Servo moteurbrushless;

// declaration des fonctions du programme
uint8_t CalculerNum(uint8_t Adresse);                   // fonction de traitement
uint8_t CalculerPuissance(uint8_t Donnee);
uint8_t CalculerAngle(uint8_t Donnee);
uint8_t CalculerBuzzer(uint8_t Adresse);    // fonction de traitement
void pilotermot(uint8_t Puissance);
void piloterroue(uint8_t Angle);
void piloterbuzz(uint8_t Etat) ;
void piloterled(uint8_t Etat) ;

// definition des fonctions du programme
uint8_t CalculerPuissance(uint8_t Donnee)                    // Calculer Puissance: fonction de traitement
{
  return (Donnee >> 5);
}

uint8_t CalculerAngle(uint8_t Donnee)                    // CalculerAngle : fonction de traitement
{
  return (Donnee & 0b00011111);
}

uint8_t CalculerNum(uint8_t Adresse)
{
  return (Adresse & 0b01111111);
}

uint8_t CalculerBuzzer(uint8_t Adresse)
{
  return (Adresse >> 7);
}


void setup()
{
  // Initialisation du sens de transfert de l'information des broches de type GPIO
  Serial.begin(9600);
  pinMode(RECEPTEUR_Pin, INPUT);
  pinMode(LEDBLEUE_Pin,  OUTPUT);
  pinMode(BUZZER_Pin,    OUTPUT);
  pinMode(SERVO_Pin,     OUTPUT);
  pinMode(MOTEUR_Pin ,   OUTPUT);
  servomoteur.attach(SERVO_Pin, 1000, 2000);
  moteurbrushless.attach(MOTEUR_Pin, 1000, 2000);
}


void loop()
{
  uint8_t Adresse;
  uint8_t Donnee;
  int8_t Erreur;
  uint8_t Numequipe;
  uint8_t Puissance;
  uint8_t Angle;
  uint8_t Buzzer;
  Erreur = AcquerirTrameNEC(RECEPTEUR_Pin, &Adresse, &Donnee); // Acquisition Trame
  Numequipe = CalculerNum(Adresse);
  if ((Erreur == 0) && (Numequipe == 0x42)) {
    piloterled(1);
    Puissance = CalculerPuissance(Donnee);
    pilotermot(Puissance);
    Angle = CalculerAngle(Donnee);
    piloterroue(Angle);
    Buzzer = CalculerBuzzer(Adresse);
    piloterbuzz(Buzzer);
  }
  else {
    piloterled(0);
    pilotermot(0);
  }
}
// Action Chaine

void pilotermot(uint8_t Puissance) {
  uint8_t Valeur = map(Puissance, 0, 7, 0, 180);
  moteurbrushless.write(Valeur);
}

void piloterroue(uint8_t Angle) {
  uint8_t Valeur = map(Angle, 0, 31, 0, 180);
  servomoteur.write(Valeur);
}

void piloterbuzz(uint8_t Etat) {
  if (Etat == 1) {
    tone(BUZZER_Pin, 4000);
  } else {
    noTone(BUZZER_Pin);
  }
}

void piloterled(uint8_t Etat) {
  digitalWrite(LEDBLEUE_Pin, Etat);
}
