# Automatyczna brama – Arduino UNO

## 1. Opis projektu

Projekt przedstawia symulację automatycznej bramy sterowanej za pomocą Arduino UNO.

Brama wykorzystuje:
- 2 serwomechanizmy SG90
- Pilot IR
- Odbiornik podczerwieni
- Czujnik ruchu PIR
- 1 diodę LED

Sterowanie zostało zaprogramowane z wykorzystaniem maszyny stanów (State Machine) oraz instrukcji switch-case.

## 2. Wykorzystane elementy

- Arduino UNO
- 2x serwomechanizm SG90
- Pilot IR
- Odbiornik IR
- Czujnik PIR
- Dioda LED
- Rezystor 220 Ω
- Przewody połączeniowe

## 3. Podłączenie elementów

| Element | Pin Arduino UNO |
|---|---|
| Serwo 1 – sygnał | D9 |
| Serwo 2 – sygnał | D10 |
| Odbiornik IR – OUT | D2 |
| Czujnik PIR – OUT | D3 |
| Dioda LED (+) przez rezystor | D13 |
| Dioda LED (-) | GND |
| PIR – VCC | 5V |
| PIR – GND | GND |
| Odbiornik IR – VCC | 5V |
| Odbiornik IR – GND | GND |

## 4. Maszyna stanów

W projekcie wykorzystano sześć stanów:

- `ST_ZAMKNIETA` – brama zamknięta
- `ST_OTWARTA` – brama otwarta
- `ST_OTWIERANIE` – brama otwiera się
- `ST_ZAMYKANIE` – brama zamyka się
- `ST_POLOTWARTA` – brama zatrzymana podczas otwierania
- `ST_POLZAMKNIETA` – brama zatrzymana podczas zamykania

### Przebieg stanów

Podstawowy cykl:

`ZAMKNIĘTA -> OTWIERANIE -> OTWARTA -> ZAMYKANIE -> ZAMKNIĘTA`

Możliwe jest zatrzymanie bramy w dowolnym momencie ruchu.

## 5. Sterowanie pilotem

- W stanie zamkniętym – rozpoczyna otwieranie.
- W stanie otwartym – rozpoczyna zamykanie.
- Podczas otwierania – zatrzymuje bramę.
- Podczas zamykania – zatrzymuje bramę.
- W stanie półotwartym – rozpoczyna zamykanie.
- W stanie półzamkniętym – rozpoczyna otwieranie.

## 6. Czujnik ruchu PIR

Czujnik PIR wykrywa ruch w pobliżu bramy.

Jeżeli podczas zamykania bramy czujnik wykryje ruch (HIGH), program automatycznie zmienia kierunek ruchu i rozpoczyna otwieranie.

Przebieg:

`ZAMYKANIE -> WYKRYCIE RUCHU -> OTWIERANIE -> OTWARTA`

## 7. Dioda LED

Jedna dioda LED informuje o ruchu bramy.

- Podczas otwierania – miga.
- Podczas zamykania – miga.
- Po zatrzymaniu – gaśnie.

Czas migania: 500 ms.

## 8. Parametry projektu

| Parametr | Wartość |
|---|---|
| Mikrokontroler | Arduino UNO |
| Serwomechanizmy | 2x SG90 |
| Czas pełnego ruchu | 10 sekund |
| Zakres ruchu | 90 stopni |
| Sterowanie | Pilot IR |
| Wykrywanie ruchu | PIR |
| Sygnalizacja | 1x LED |
| Język programowania | Arduino C++ |
| Środowisko | Tinkercad |

## 9. Wykorzystane biblioteki

- `Servo.h` – obsługa serwomechanizmów.
- `IRremote.hpp` – obsługa odbiornika podczerwieni.

## 10. Cel projektu

Celem projektu jest praktyczne wykorzystanie:
- Maszyny stanów (switch-case).
- Sterowania serwomechanizmami.
- Obsługi pilota IR.
- Odczytu danych z czujnika PIR.
- Obsługi wejść i wyjść cyfrowych Arduino.
- Funkcji `millis()` do sterowania czasem bez użycia delay().

## 11. Bezpieczeństwo

Projekt jest demonstracją edukacyjną w środowisku Tinkercad.

W prawdziwej bramie wymagane są dodatkowe zabezpieczenia przed przygnieceniem, np. fotokomórki i zabezpieczenia krawędziowe. Czujnik PIR nie zastępuje tych zabezpieczeń.
