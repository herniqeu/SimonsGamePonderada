int iSequence[4];
int iPlayerInput[4];
int iSequenceIndex = 0;
int iPlayerCount = 0;
bool bWaitingInput = false;

const int LED_PINS[] = {14, 12, 13, 2};
const int BUTTON_PINS[] = {34, 35, 32, 33};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT); 
    pinMode(BUTTON_PINS[i], INPUT);
  }
}

void loop() {
  if (iPlayerCount == 0) {
    generateRandomSequence();
    Serial.println("comecar");
    delay(1000);
    startPrint();
    }

  if(digitalRead(2) == HIGH){ // verifica se o botão na porta D2 foi pressionado
    iSequenceIndex = 0;
    iPlayerCount = 0;
    bWaitingInput = false;
    Serial.println("resetado");
    delay(1000);
  }

  Serial.println(iSequence[0]);

  playSequence(iSequenceIndex);

  digitalWrite(LED_PINS[iSequence[iSequenceIndex]], HIGH);
  delay(1000);
  digitalWrite(LED_PINS[iSequence[iSequenceIndex]], LOW);

  if (waitForInput(iSequenceIndex + 1)) {
    if (iSequenceIndex == 2) {
      nextRound();
    }
    else if (iSequenceIndex == 3) {
      Serial.println("ganhou");
      iSequenceIndex = 0;
      iPlayerCount = 0;
      delay(1000);
      startPrint();
    } 
    else {
      iSequenceIndex++;
    }
  } 
  else {
  gameOver();
  }
}

int buttonLedConverter(int Button) {
  if (Button == 34) {
    return 14;
  }
  if (Button == 35) {
    return 12;
  }
  if (Button == 32) {
    return 13;
  }
  if (Button == 33) {
    return 2;
  }
  return -1;
}

void generateRandomSequence() {
  for (int i = 0; i < 4; i++) {
    int iRandomSingle = random(0, 3);
    iSequence[i] = iRandomSingle;
  }
}

void startPrint() {
  digitalWrite(LED_PINS[0], HIGH);
  digitalWrite(LED_PINS[1], HIGH);
  digitalWrite(LED_PINS[2], HIGH);
  digitalWrite(LED_PINS[3], HIGH);
  delay(3000);

  digitalWrite(LED_PINS[0], LOW);
  digitalWrite(LED_PINS[1], LOW);
  digitalWrite(LED_PINS[2], LOW);
  digitalWrite(LED_PINS[3], LOW);
  delay(500);
}

void rightPrint() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PINS[i], HIGH);
    delay(100);
    digitalWrite(LED_PINS[i], LOW);
    delay(100);
  }
}

void errorPrint() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PINS[i], HIGH);
    delay(100);
    digitalWrite(LED_PINS[i], LOW);
    delay(100);
  }
}

void light(int n) {
  for (int j = 0; j < n; j++) {
    digitalWrite(LED_PINS[iSequence[j]], HIGH);
    delay(500);
    digitalWrite(LED_PINS[iSequence[j]], LOW);
    delay(500);
  }
}

void playSequence(int iIndex) {
  for (int i = 0; i < iIndex; i++) {
    digitalWrite(LED_PINS[iSequence[i]], HIGH);
    delay(500);
    digitalWrite(LED_PINS[iSequence[i]], LOW);
    delay(500);
  }
}

bool waitForInput(int iIndex) {
  int iInputIndex = 0;
  while (iInputIndex < iIndex) {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(BUTTON_PINS[i]) == HIGH) {
      iPlayerInput[iInputIndex] = buttonLedConverter(BUTTON_PINS[i]);
      iInputIndex++;
      digitalWrite(LED_PINS[i], HIGH);
      delay(250);
      digitalWrite(LED_PINS[i], LOW);
      delay(250);
    }
  }
  if (iInputIndex == iIndex) {
    break;  // Exit the while loop if the expected number of inputs has been received
    }
  }

  for (int i = 0; i < iIndex; i++) {
    if (iPlayerInput[i] != iSequence[i]) {
      errorPrint();
      return false;
    }
  }

  rightPrint();
  return true;
}

void nextRound() {
  iSequenceIndex = 0;
  iPlayerCount++;
  delay(1000);
}

void gameOver() {
  Serial.println("errou");
  errorPrint();
  iSequenceIndex = 0;
  iPlayerCount = 0;
  delay(1000);
}