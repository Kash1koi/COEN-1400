int buttonPins[] = {2, 4, 6, 8};   //red,yellow, green, blue But
int ledPins[] = {3, 5, 7, 9};      //red,yellow, green, blue LEds
int buzzerPin = 10;               

const int GAME_MAX_LENGTH = 4;    //length of game
int gameSequence[GAME_MAX_LENGTH]; 
int playerSequence[GAME_MAX_LENGTH]; 

int currentRound = 0;              

void setup() { //this gets all of the pins ready to go
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // The start
  }
  
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  
  pinMode(buzzerPin, OUTPUT);
  randomSeed(analogRead(0));
}

void loop() { //this loop is what scans for if the game is being played
  if (currentRound == 0) {
    delay(1000);
    generateNextStep();
    currentRound = 1;
  }

  playSequence();

  if (getPlayerInput()) {
    currentRound++;
    if (currentRound >= GAME_MAX_LENGTH) {
      gameWon();
    } else {
      generateNextStep();
      delay(1000); 
    }
  } else {
    gameOver();
  }
}

void generateNextStep() {
  gameSequence[currentRound - 1] = random(0, 4);
}

void playSequence() {
  for (int i = 0; i < currentRound; i++) {
    flashLedAndBuzz(gameSequence[i], 300);
    delay(150); 
  }
}

void flashLedAndBuzz(int ledIndex, int duration) {
  int tones[] = {260, 320, 380, 480}; 
  
  digitalWrite(ledPins[ledIndex], HIGH);
  tone(buzzerPin, tones[ledIndex]);
  delay(duration);
  
  digitalWrite(ledPins[ledIndex], LOW);
  noTone(buzzerPin);
}

bool getPlayerInput() { //this ensures the correct player input
  for (int i = 0; i < currentRound; i++) {
    int pressedButton = -1;
    
    while (pressedButton == -1) {
      for (int b = 0; b < 4; b++) {
        if (digitalRead(buttonPins[b]) == LOW) {
          pressedButton = b;
          flashLedAndBuzz(pressedButton, 250); 
          
          while(digitalRead(buttonPins[b]) == LOW); 
          delay(50); 
        }
      }
    }
    
    if (pressedButton != gameSequence[i]) {
      return false; 
    }
  }
  return true; 
}

void gameOver() { //this fuction plays the game over sound and then also starts the game over
  tone(buzzerPin, 130, 900); 
  for (int i = 0; i < 3; i++) {
    for (int l = 0; l < 4; l++) digitalWrite(ledPins[l], HIGH);
    delay(500);
    for (int l = 0; l < 4; l++) digitalWrite(ledPins[l], LOW);
    delay(100);
  }
  currentRound = 0; 
}

void gameWon() { //this fuction plays the game won sound and oother things
  for (int i = 0; i < 3; i++) {
    flashLedAndBuzz(0, 500);
    flashLedAndBuzz(1, 500);
    flashLedAndBuzz(2, 500);
    flashLedAndBuzz(3, 500);
  }
  currentRound = 0; 
}
