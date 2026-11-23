
#include <Servo.h>
#include <IRremote.hpp>

// =================================
// DEFINICJE PINOW
// =================================

#define SERWO1_PIN 9
#define SERWO2_PIN 10

#define IR_PIN 2
#define LED_PIN 13
#define PIR_PIN 3

// =================================
// DEFINICJE STANOW
// =================================

#define ST_ZAMKNIETA       1
#define ST_OTWARTA         2
#define ST_OTWIERANIE      3
#define ST_ZAMYKANIE       4
#define ST_POLOTWARTA      5
#define ST_POLZAMKNIETA    6

// =================================
// USTAWIENIA
// =================================

#define CZAS_RUCHU 10000UL

#define KAT_OTWARTY 90
#define KAT_ZAMKNIETY 0

#define SERWO2_ODWROTNIE true

// =================================
// OBIEKTY
// =================================

Servo serwo1;
Servo serwo2;

// =================================
// ZMIENNE
// =================================

uint8_t state = ST_ZAMKNIETA;

float aktualnyKat = 0;
float katStart = 0;
float katCel = 0;

unsigned long czasStartu = 0;
unsigned long czasRuchu = 0;

unsigned long czasLED = 0;
unsigned long ostatnieKlikniecie = 0;

bool ledStan = false;
bool kierunekOtwierania = true;

// =================================
// USTAWIANIE SERW
// =================================

void ustawSerwa(float kat) {

  int kat1 = map((int)kat, 0, 90, 0, 90);

  int kat2 = map((int)kat, 0, 90, 180, 90);

  serwo1.write(kat1);
  serwo2.write(kat2);
}
// =================================
// ROZPOCZECIE RUCHU
// =================================

void rozpocznijRuch(bool otwieranie) {

  kierunekOtwierania = otwieranie;

  katStart = aktualnyKat;

  if (otwieranie) {

    katCel = KAT_OTWARTY;
    state = ST_OTWIERANIE;

  }
  else {

    katCel = KAT_ZAMKNIETY;
    state = ST_ZAMYKANIE;

  }

  czasStartu = millis();

  czasRuchu = (unsigned long)(
    CZAS_RUCHU * abs(katCel - katStart) / 90.0
  );

  if (czasRuchu == 0) {
    czasRuchu = 1;
  }
}

// =================================
// AKTUALIZACJA RUCHU
// =================================

void aktualizujRuch() {

  if (state != ST_OTWIERANIE &&
      state != ST_ZAMYKANIE) {
    return;
  }

  unsigned long teraz = millis();

  float postep =
    (float)(teraz - czasStartu) / czasRuchu;

  if (postep >= 1.0) {

    aktualnyKat = katCel;

    ustawSerwa(aktualnyKat);

    if (kierunekOtwierania) {
      state = ST_OTWARTA;
    }
    else {
      state = ST_ZAMKNIETA;
    }

  }
  else {

    aktualnyKat =
      katStart + (katCel - katStart) * postep;

    ustawSerwa(aktualnyKat);

  }
}

// =================================
// ZATRZYMANIE BRAMY
// =================================

void zatrzymajBrame() {

  aktualizujRuch();

  if (state == ST_OTWIERANIE) {
    state = ST_POLOTWARTA;
  }
  else if (state == ST_ZAMYKANIE) {
    state = ST_POLZAMKNIETA;
  }

  ustawSerwa(aktualnyKat);
}

// =================================
// OBSLUGA PILOTA
// =================================

void obsluzPilota() {

  if (IrReceiver.decode()) {

    bool powtorzenie =
      IrReceiver.decodedIRData.flags &
      IRDATA_FLAGS_IS_REPEAT;

    if (!powtorzenie &&
        millis() - ostatnieKlikniecie > 250) {

      ostatnieKlikniecie = millis();

      switch (state) {

        case ST_ZAMKNIETA:
          rozpocznijRuch(true);
          break;

        case ST_OTWARTA:
          rozpocznijRuch(false);
          break;

        case ST_OTWIERANIE:
          zatrzymajBrame();
          break;

        case ST_ZAMYKANIE:
          zatrzymajBrame();
          break;

        case ST_POLOTWARTA:
          rozpocznijRuch(false);
          break;

        case ST_POLZAMKNIETA:
          rozpocznijRuch(true);
          break;
      }
    }

    IrReceiver.resume();
  }
}

// =================================
// MIGANIE LED
// =================================

void miganieLED() {

  if (state == ST_OTWIERANIE ||
      state == ST_ZAMYKANIE) {

    if (millis() - czasLED >= 500) {

      czasLED = millis();

      ledStan = !ledStan;

      digitalWrite(LED_PIN, ledStan);
    }

  }
  else {

    ledStan = false;
    digitalWrite(LED_PIN, LOW);
  }
}

// =================================
// OBSLUGA CZUJNIKA PIR
// =================================

void obsluzPIR() {

  if (state == ST_ZAMYKANIE) {

    if (digitalRead(PIR_PIN) == HIGH) {

      rozpocznijRuch(true);

    }

  }

}

// =================================
// SETUP
// =================================

void setup() {

  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  pinMode(PIR_PIN, INPUT);
  // Podlaczenie serw
  serwo1.attach(SERWO1_PIN);
  serwo2.attach(SERWO2_PIN);

  // Pozycja poczatkowa:
  // Serwo 1 = 0 stopni
  // Serwo 2 = 90 stopni

  aktualnyKat = 0;

  serwo1.write(0);
  serwo2.write(90);

  // Odbiornik IR
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  state = ST_ZAMKNIETA;
  aktualnyKat = 0;

ustawSerwa(aktualnyKat);
}

// =================================
// LOOP
// =================================

void loop() {

  obsluzPilota();

  obsluzPIR();

  aktualizujRuch();

  miganieLED();

}