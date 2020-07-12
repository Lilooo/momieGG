/*
   Auteur : Alice Giordani
   Date de création : 01/08/2019
   Dernière modification : 04/08/2019
*/

#include <Keypad.h>
#include <Adafruit_NeoPixel.h>

// Branchements
#define PIN 12
#define NUMPIXELS 20

int relais = 11;

const byte ROWS = 4;
const byte COLS = 5;
char keys[ROWS][COLS] = {
  {'A', 'B', 'C', 'D', 'E'},
  {'F', 'G', 'H', 'I', 'J'},
  {'K', 'L', 'M', 'N', 'O'},
  {'P', 'Q', 'R', 'S', 'T'}
};
byte colPins[COLS] = {2, 3, 4, 5, 6};
byte rowPins[ROWS] = {7, 8, 9, 10};

int brightness = 50;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
unsigned long previousMillis = 0;
const long interval = 30;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Valeurs mdp
String password = "ABDGHTMNOR";
String tempPassword;
boolean enteredPassword;
char keypressed;
int k = 0;


// Etats
boolean activated = false;
boolean alarmActivated = false;
bool ouvert;

void setup() {
  Serial.begin(9600);
  strip.begin();
  pinMode(relais, OUTPUT);
  alarmActivated = true;
}

void loop() {

  // Alarme activée, attente du bon mdp
  if (alarmActivated) {
    enterPassword();
  }

  if (ouvert) {
    digitalWrite(relais, HIGH); // ouverture
  } else {
    digitalWrite(relais, LOW); // fermeture
  }

}

// Système de désactivation de l'alarme
void enterPassword() {
  activated = true;
  //Serial.println("Pass>");
  while (activated) {
    keypressed = keypad.getKey();
    //Serial.println(keypressed);
    if (keypressed != NO_KEY) {
      if ( k == 0 ) {
        if ( keypressed == 'R' ) {
          goodAnswer(17);
        } else {
          wrongAnswer();
        }
      } else if ( k == 1 ) {
        if ( keypressed == 'Q' ) {
          goodAnswer(18);
        } else if (keypressed == 'R') {
          //nothing
        } else {
          wrongAnswer();
        }
      } else if ( k == 2 ) {
        if ( keypressed == 'E' ) {
          goodAnswer(4);
        } else if (keypressed == 'Q' || keypressed == 'R') {
          //nothing
        } else {
          wrongAnswer();
        }
      } else if ( k == 3 ) {
        if ( keypressed == 'F' ) {
          goodAnswer(9);
        } else if (keypressed == 'Q' || keypressed == 'R' || keypressed == 'E') {
          //nothing
        } else {
          wrongAnswer();
        }
      } else if ( k == 4 ) {
        if ( keypressed == 'H' ) {
          goodAnswer(7);
        } else if (keypressed == 'Q' || keypressed == 'R' || keypressed == 'E' || keypressed == 'F') {
          //nothing
        } else {
          wrongAnswer();
        }
      } else if ( k == 5 ) {
        if ( keypressed == 'T' ) {
          goodAnswer(15);
        } else if (keypressed == 'H' || keypressed == 'Q' || keypressed == 'R' || keypressed == 'E' || keypressed == 'F') {
          //nothing
        } else {
          wrongAnswer();
        }
      } else if ( k == 6 ) {
        if ( keypressed == 'L' ) {
          goodAnswer(11);
        } else if (keypressed == 'T' || keypressed == 'H' || keypressed == 'Q' || keypressed == 'R' || keypressed == 'E' || keypressed == 'F') {
          //nothing
        } else {
          wrongAnswer();
        }
      } else if ( k == 7 ) {
        if ( keypressed == 'O') {
          goodAnswer(14);
        } else if (keypressed == 'L' || keypressed == 'T' || keypressed == 'H' || keypressed == 'Q' || keypressed == 'R' || keypressed == 'E' || keypressed == 'F') {
          //nothing
        } else {
          wrongAnswer();
        }
      } else if ( k == 8 ) {
        if ( keypressed == 'A' ) {
          goodAnswer(0);
        } else if (keypressed == 'O' || keypressed == 'L' || keypressed == 'T' || keypressed == 'H' || keypressed == 'Q' || keypressed == 'R' || keypressed == 'E' || keypressed == 'F') {
          //nothing
        } else {
          wrongAnswer();
        }
      } else if ( k == 9 ) {
        if ( keypressed == 'C' ) {
          activated = false;
          alarmActivated = false;
          ouvert = true;
          goodAnswer(2);
        } else if (keypressed == 'A' || keypressed == 'O' || keypressed == 'L' || keypressed == 'T' || keypressed == 'H' || keypressed == 'Q' || keypressed == 'R' || keypressed == 'E' || keypressed == 'F') {
          //nothing
        } else {
          wrongAnswer();
        }
      }
    } else if ( k == 0 ) {
      waitMode();
    }
  }
}
// Réponses

void goodAnswer(int l) {
  Serial.println(keypressed);
  Serial.println("Good password!");
  strip.setPixelColor(l, 0, 100, 0);
  strip.setBrightness(225);
  strip.show();
  k++;
}
void wrongAnswer() {
  Serial.println(keypressed);
  Serial.println("Wrong! Try Again");
  errorLight(200);
  strip.show();
  k = 0;
}

// Animation des lumières

void waitMode() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 255, 103, 23);
      strip.setBrightness(brightness);
    }
    brightness = brightness + fadeAmount;
    if (brightness <= 50 || brightness >= 255  ) {
      fadeAmount = -fadeAmount;
    }
    strip.show();
  }
}

void errorLight(uint8_t wait) {

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 125, 0, 0);  //turn every third pixel on
  }
  strip.setBrightness(255);
  strip.show();

  delay(wait);

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0, 0, 0);  //turn every third pixel on
  }
  strip.setBrightness(255);
  strip.show();

  delay(wait);

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 125, 0, 0);  //turn every third pixel on
  }
  strip.setBrightness(255);
  strip.show();

  delay(wait);

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0, 0, 0);  //turn every third pixel on
  }
  strip.setBrightness(255);
  strip.show();
}
