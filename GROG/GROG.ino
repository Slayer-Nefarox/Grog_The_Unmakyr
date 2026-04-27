#include <Keyboard.h>
#include <Mouse.h>

#define NUM_INPUTS 11

int pinNumbers[NUM_INPUTS] = {
  5, 6, 7, 8, 12, 13,
  0, 1, 2, 3, 4
};

int keyCodes[NUM_INPUTS] = {
  ' ',
  KEY_RETURN,
  KEY_RIGHT_ARROW,
  KEY_DOWN_ARROW,
  KEY_UP_ARROW,
  KEY_LEFT_ARROW,
  'w', 'a', 's', 'd', 'f'
};

bool pressed[NUM_INPUTS];
bool prevPressed[NUM_INPUTS];

byte stableCount[NUM_INPUTS];
#define DEBOUNCE_THRESHOLD 3

void setup() {
  delay(10000);
  for (int i = 0; i < NUM_INPUTS; i++) {
    pinMode(pinNumbers[i], INPUT);
    pressed[i] = false;
    prevPressed[i] = false;
    stableCount[i] = 0;
  }
  
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  for (int i = 0; i < NUM_INPUTS; i++) {
    bool rawState = (digitalRead(pinNumbers[i]) == LOW);

    if (rawState == pressed[i]) {
      stableCount[i] = 0;
    } else {
      stableCount[i]++;
      if (stableCount[i] >= DEBOUNCE_THRESHOLD) {
        pressed[i] = rawState;
        stableCount[i] = 0;
      }
    }

    if (pressed[i] && !prevPressed[i]) {
      Keyboard.press(keyCodes[i]);
    } 
    else if (!pressed[i] && prevPressed[i]) {
      Keyboard.release(keyCodes[i]);
    }

    prevPressed[i] = pressed[i];
  }

  float alpha = 0.2; 
  int deadzone = 30; 
  int maxSpeed = 10; 
  int center = 512;

  // Variáveis estáticas para retenção de memória do filtro
  static float A0_f = center;
  static float A1_f = center;
  static float A4_f = center;
  static float A5_f = center;

  // Dupla leitura (descarte da primeira) para evitar crosstalk no ADC
  analogRead(A0); 
  int A0_r = analogRead(A0);
  
  analogRead(A1); 
  int A1_r = analogRead(A1);
  
  analogRead(A4); 
  int A4_r = analogRead(A4);
  
  analogRead(A5); 
  int A5_r = analogRead(A5);

  A0_f = alpha * A0_r + (1 - alpha) * A0_f;
  A1_f = alpha * A1_r + (1 - alpha) * A1_f;
  A4_f = alpha * A4_r + (1 - alpha) * A4_f;
  A5_f = alpha * A5_r + (1 - alpha) * A5_f;

  float mov_x = (A1_f - center) - (A0_f - center);
  float mov_y = (A5_f - center) - (A4_f - center);

  if (abs(mov_x) < deadzone) mov_x = 0;
  if (abs(mov_y) < deadzone) mov_y = 0;

  mov_x = constrain(mov_x / 50, -maxSpeed, maxSpeed);
  mov_y = constrain(mov_y / 50, -maxSpeed, maxSpeed);
  
  if (mov_x != 0 || mov_y != 0) {
    Mouse.move((int)mov_x, (int)mov_y, 0);
  }

  static bool leftPressed = false;
  static bool rightPressed = false;

  int clickThreshold = 600;
  int releaseThreshold = 400;

  // Dupla leitura para isolamento do multiplexador
  analogRead(A2); 
  int A2_r = analogRead(A2);
  
  analogRead(A3); 
  int A3_r = analogRead(A3);

  if (A2_r > clickThreshold && !leftPressed) {
    Mouse.press(MOUSE_LEFT);
    leftPressed = true;
  } else if (A2_r < releaseThreshold && leftPressed) {
    Mouse.release(MOUSE_LEFT);
    leftPressed = false;
  }

  if (A3_r > clickThreshold && !rightPressed) {
    Mouse.press(MOUSE_RIGHT);
    rightPressed = true;
  } else if (A3_r < releaseThreshold && rightPressed) {
    Mouse.release(MOUSE_RIGHT);
    rightPressed = false;
  }

  delay(5);
}