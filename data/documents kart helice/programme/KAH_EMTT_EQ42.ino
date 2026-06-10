/*************************************************************************************************************************************/
// Sujet :   Programme de l'émetteur du projet KAH                                                                                   //
// Equipe :  42                                                                                                                      //
// Date :    10/03/2026                                                                                                              //
// Version : 1                                                                                                                       //
/*************************************************************************************************************************************/

#include <stdint.h>
#include <arduino.h>
#include "NEC.h"

#define PotentiometreVitesse_Pin     A5
#define PotentiometreDirection_Pin   A4
#define BoutonPoussoir_Pin            2
#define LEDInfrarouge_Pin            11
#define NumeroEquipe               0x42

uint8_t AcquisitionGaz(void) {           // retourne une valeur : [0 ; 7]
  return map(analogRead(PotentiometreVitesse_Pin), 0, 1023, 0, 7);
}

uint8_t AcquisitionDirection(void) {     // retourne une valeur : [0 ; 31]
  return map(analogRead(PotentiometreDirection_Pin), 0, 1023, 0, 31);
}

uint8_t AcquisitionBouton(void) {        // retourne : 0 (BP relâché), 1 (BP enfoncé)
  return digitalRead(BoutonPoussoir_Pin);
}

uint8_t CalculerDonneeNEC(uint8_t Gaz, uint8_t Direction) {
  return 0x00;
}

uint8_t CalculerAdresseNEC(uint8_t Bouton) {
  return NumeroEquipe;
}

void PiloterInfrarouge(uint8_t Adresse, uint8_t Donnee) {
  GenererTrameNEC(LEDInfrarouge_Pin, Adresse, Donnee);
}

void setup(void) {
  pinMode(BoutonPoussoir_Pin, INPUT);
  pinMode(LEDInfrarouge_Pin,  OUTPUT);
  Serial.begin(9600);
}

void loop(void) {
  static uint8_t TrameDonnee_precedente;
  static uint8_t TrameAdresse_precedente;
  static uint32_t t_envoi_precedent = 0;
  uint8_t Gaz       = AcquisitionGaz();
  uint8_t Direction = AcquisitionDirection();
  uint8_t Bouton    = AcquisitionBouton();

  //Serial.print("Gaz       : "); Serial.println(Gaz);
  //Serial.print("Direction : "); Serial.println(Direction);
  //Serial.print("Bouton    : "); Serial.println(Bouton);
  //Serial.println("---");

  uint8_t TrameDonnee  = CalculerDonneeNEC(Gaz, Direction);
  uint8_t TrameAdresse = CalculerAdresseNEC(Bouton);

  if ( (TrameDonnee != TrameDonnee_precedente) || (TrameAdresse != TrameAdresse_precedente) || (millis() > (t_envoi_precedent + 333))) {
    TrameDonnee_precedente = TrameDonnee;
    TrameAdresse_precedente = TrameAdresse;
    t_envoi_precedent = millis();
    PiloterInfrarouge(TrameAdresse, TrameDonnee);
  } else {}
}
