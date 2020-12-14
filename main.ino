//
// Zoom Buttons
//

#include "HID-Project.h"

//#define VOLUME
//#define KYBDLEDS
#define ZOOMBTNS


// Define Arduino pin numbers for buttons and LEDs
#define led 9

#ifdef VOLUME
#define ROTARY_A 5
#define ROTARY_B 6
#define ROTARY_C 7
#endif

#ifdef KYBDLEDS
#define CAPSLOCKLED 5 
#define SCRLLOCKLED 6
#define NUMLOCKLED 7
#endif

#ifdef ZOOMBTNS
#define BTN1 2
#define BTN2 3
#define BTN3 10
#define BTN4 A0
#define BTN5 A1
#define BTN6 A2
#define NUMBUTTONS 6
int buttons[NUMBUTTONS] = {BTN1, BTN2, BTN3, BTN4, BTN5, BTN6};
unsigned long btntime[NUMBUTTONS];
boolean btnpress[NUMBUTTONS];
#endif

#ifdef VOLUME
boolean A, a, B, b, C, c;
#endif

char line[80];
unsigned long t;
int n;

void setup() {
//  Serial.begin(38400);
//  Serial.write("Starting...\n");
//  Serial.end();
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
#ifdef VOLUME
  pinMode(ROTARY_A, INPUT_PULLUP);
  pinMode(ROTARY_B, INPUT_PULLUP);
  pinMode(ROTARY_C, INPUT_PULLUP);
#endif
#ifdef KYBDLEDS
  pinMode(CAPSLOCKLED, OUTPUT);
  pinMode(SCRLLOCKLED, OUTPUT);
  pinMode(NUMLOCKLED, OUTPUT);

  // Flash the LEDs just to show we're in business
  digitalWrite(CAPSLOCKLED, HIGH); delay(200);
  digitalWrite(SCRLLOCKLED, HIGH); delay(200);
  digitalWrite(NUMLOCKLED, HIGH); delay(200);
  digitalWrite(CAPSLOCKLED, LOW); delay(200);
  digitalWrite(SCRLLOCKLED, LOW); delay(200);
  digitalWrite(NUMLOCKLED, LOW); delay(200);
#else
  for (int i=0; i < 3; i++) {
  digitalWrite(led, HIGH); delay(20);
  digitalWrite(led, LOW); delay(200);
  digitalWrite(led, HIGH); delay(20);
  digitalWrite(led, LOW); delay(200);
  digitalWrite(led, HIGH); delay(20);
  digitalWrite(led, LOW); delay(500);
  }
#endif

#ifdef ZOOMBTNS
  for (int i = 0; i < NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    btntime[i] = 0;
    btnpress[i] = false;
  }
#endif
  
#ifdef VOLUME
  a = b = c = false;
#endif

#ifdef KYBDLEDS
  t = 0;
  n = 0;
#endif
#ifdef defined(KYBDLEDS) || defined(ZOOMBTNS)
  BootKeyboard.begin();
#endif

#ifdef VOLUME
  Consumer.begin();
#endif

//  System.begin(); // For System functions
//  Gamepad.begin(); // For gamepad functions
//  Mouse.begin(); // For mouse functions
//  AbsoluteMouse.begin(); // For the Absolute Mouse

}

void loop() {
  int leds;
#ifdef VOLUME
  A = digitalRead(ROTARY_A) == LOW;
  B = digitalRead(ROTARY_B) == LOW;
  C = digitalRead(ROTARY_C) == LOW;
  if (A && !a) {
    if (B) {
      n++; if (n > 100) n = 100;
      Consumer.write(MEDIA_VOL_UP);
    }
    else {
      n--; if (n < 0) n = 0;
      Consumer.write(MEDIA_VOL_DOWN);
    }
//      sprintf(line, "%d\n", n);
//      Serial.write(line);
  }
  a = A;
  
  if (C && ! c) {
    Consumer.write(MEDIA_VOL_MUTE);
//      sprintf(line, "!\n");
//      Serial.write(line);
  }
  c = C;
#endif

#ifdef KYBDLEDS
  if (millis() > t + 10) {
    t = millis();
    leds = BootKeyboard.getLeds();
    if (leds & LED_CAPS_LOCK)
      digitalWrite(CAPSLOCKLED, HIGH);
    else
      digitalWrite(CAPSLOCKLED, LOW);
    if (leds & LED_SCROLL_LOCK)
      digitalWrite(SCRLLOCKLED, HIGH);
    else
      digitalWrite(SCRLLOCKLED, LOW);
    if (leds & LED_NUM_LOCK)
      digitalWrite(NUMLOCKLED, HIGH);
    else
      digitalWrite(NUMLOCKLED, LOW); 
  }
#endif

#ifdef ZOOMBTNS
  for (int i = 0; i < NUMBUTTONS; i++) {
    if (!digitalRead(buttons[i])) {
      // Button pressed (negative logic)
      if (btntime[i] == 0) {
        // Button has just been pressed
        btntime[i] = millis();
      }
      else {
        // Button is still pressed
        if (millis() - btntime[i] > 20 && !btnpress[i]) {
          // This is not just a glitch          
          btnpress[i] = true;
          // Now do your stuff!
          digitalWrite(led, HIGH);
          switch (i) {
#if 1
          case 4 : // Alt-Y: Raise Hand
            BootKeyboard.press(KEY_LEFT_ALT);
            BootKeyboard.write(KEY_Y);
            BootKeyboard.release(KEY_LEFT_ALT);
            break;
          case 5: // Space: Speak
           do{ BootKeyboard.press(KEY_SPACE);         
            //delay(100);
            
            } while (!digitalRead(buttons[5]));
            BootKeyboard.releaseAll();
            break;
 
#else
          case 0:
            BootKeyboard.write(KEY_0);
            break;
          case 1:
            BootKeyboard.write(KEY_1);
            break;
          case 2:
            BootKeyboard.write(KEY_2);
            break;
          case 3:
            BootKeyboard.write(KEY_3);
            break;
          case 4:
            BootKeyboard.write(KEY_4);
            break;
          case 5:
            BootKeyboard.write(KEY_5);
            break;
#endif
          }  
        }
      }
    }
    else {
      // Button not pressed
      if (btntime[i] != 0) {
        // Looks like it's just been released
        digitalWrite(led, LOW);
        btntime[i] = 0;
        btnpress[i] = false;
      }
    }
  }
#endif

}
