#include <Keyboard.h>
#include <Mouse.h>

/* * OH GOD, WHAT HAVE I DONE? 
 * I am but a lowly peasant, a fool who dared to play with the forbidden copper.
 * May the Heavens avert their eyes from this mechanical sin. 
 */

#define NUM_INPUTS 11
#define SENS 90 //max 127

// The ritual thresholds. If the voltage trembles, the beast awakens.
const int clickThreshold = 800;
const int releaseThreshold = 700;

// I have tried to soothe the static screams with these numbers...
float alpha = 0.2; 
int deadzone = 30; 
int maxSpeed = 10; 
int center = 512;

static bool leftPressed = false;
static bool rightPressed = false;

// The iron pins I have desecrated with my clumsy solder.
int pinNumbers[NUM_INPUTS] = {
  5, 6, 7, 8, 12, 13,
  0, 1, 2, 3, 4
};

// The ancient runes that shall be cast into the machine's mind.
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
#define DEBOUNCE_THRESHOLD 3 // A meager wall against the Void's flickering whispers

void setup() {
  // TEN SECONDS TO PRAY. Run while you still have your soul!
  delay(10000);
  
  for (int i = 0; i < NUM_INPUTS; i++) {
    pinMode(pinNumbers[i], INPUT);
    pressed[i] = false;
    prevPressed[i] = false;
    stableCount[i] = 0;
  }
  
  Keyboard.begin();
  Mouse.begin();
  Serial.begin(9600);
}

void loop() {
  /* THE DIGITAL TORTURE CHAMBER */
  for (int i = 0; i < NUM_INPUTS; i++) 
  {
    bool rawState = (digitalRead(pinNumbers[i]) == LOW);

    // Trying to distinguish a true touch from the twitching of a corpse.
    if (rawState == pressed[i]) 
    {
      stableCount[i] = 0;
    } else 
    {
      stableCount[i]++;
      if (stableCount[i] >= DEBOUNCE_THRESHOLD) 
      {
        pressed[i] = rawState;
        stableCount[i] = 0;
      }
    }

    // Forcing the silicon to mimic a human finger. Mercy, O Lord!
    if (pressed[i] && !prevPressed[i]) 
    {
      Keyboard.press(keyCodes[i]);
    } 
    else if (!pressed[i] && prevPressed[i]) 
    {
      Keyboard.release(keyCodes[i]);
    }

    prevPressed[i] = pressed[i];
  }

  bool A_r[6] = {0};
  A_r[0] = !digitalRead(A0);
  A_r[1] = !digitalRead(A1);
  A_r[2] = !digitalRead(A2);
  A_r[3] = !digitalRead(A3);
  A_r[4] = !digitalRead(A4);
  A_r[5] = !digitalRead(A5);

  //DEBUG
  for(int i = 0; i<6; i++)
  {
    Serial.print(A_r[i]);
  }
  Serial.println("");

  int sens = 90;
  //MORE UNHOLY MESS
  if ( A_r[0] && !A_r[1] ) 
  {
    Mouse.move(SENS, 0, 0);
  }
  else if ( !A_r[0] && A_r[1] ) 
  {
    Mouse.move(-SENS, 0, 0);
  }
  else if(A_r[4] && !A_r[5] )
  {
    Mouse.move(0, SENS, 0);
  }
  else if(!A_r[4] && A_r[5] )
  {
    Mouse.move(0, -SENS, 0);
  }
  
  /* THE CLICKING OF THE TEETH */
  // When the voltage rises like a fever, the mouse bites!
  
  if (A_r[2] && !leftPressed) 
  {
    Mouse.press(MOUSE_LEFT);
    leftPressed = true;
  } 
  else if (!A_r[2] && leftPressed) 
  {
    Mouse.release(MOUSE_LEFT);
    leftPressed = false;
  }
  
  if (A_r[3] && !rightPressed) 
  {
    Mouse.press(MOUSE_RIGHT);
    rightPressed = true;
  } else if (!A_r[3] && rightPressed) 
  {
    Mouse.release(MOUSE_RIGHT);
    rightPressed = false;
  }
  

  // A brief moment of silence before the madness repeats.
  delay(10);
}
