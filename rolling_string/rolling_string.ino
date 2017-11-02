#include <Tiny85_TLC5940.h>
#include <EEPROM.h>
#define TICK 400
#define WIDTH 3
#define HEIGHT 5
#define BRIGHTNESS 200

char str[] = "ROBERT";
int lives = 3;
int pos = 1;
int count = 0;

unsigned long last_tick;

int key_stroke = 0; //0 = nothing, 1 = left, 2 = up, 3 = right
int last_stroke = 0;
unsigned long last_press = 0;
int debounce_delay = 40;

short mapping[] = {15, 13, 12,
                   11, 10, 9, 
                   8, 7, 6,
                   5, 4, 3,
                   2, 1, 14};
                  
bool frame[HEIGHT][WIDTH];

bool frame_buffer[HEIGHT][WIDTH];

bool buffer_ready = false;

void setup() {
  tlc5940.init();
  pinMode(PB3, INPUT);

  last_tick = millis();
}


void loop () {
  if (millis() - last_tick < TICK) {
    if (!buffer_ready) {
      update();
    }
    paint();
  } else {
    if (buffer_ready) {
      refreshBuffer();
      buffer_ready = false;
    }
    last_tick = millis();
    paint();
  }
}
void update() {
  count = count%26;
  
  static bool letter_array[5][3];
  byte letter[2];
  byte address = 2 * count;

  letter[0] = EEPROM.read(address);
  letter[1] = EEPROM.read(address+1);

  for (byte i = 0;i<15;i++){
    byte current = letter[i/8];
    current = current >> (7-(i%8)-(i/8));
    letter_array[i/3][i%3] = !((current%2)==0);
  }
  

    for (int i = 0 ; i < HEIGHT; i++) {
      for (int j = 0 ; j < WIDTH; j++) {
        frame_buffer[i][j] = letter_array[i][j];
      }
    }
  count++;
  
  buffer_ready = true;
}

void paint() {
  for (int i = 0 ; i < HEIGHT; i++) {
    for (int j = 0 ; j < WIDTH; j++) {
      setLED(j, i, frame[i][j]);
    }
  }
  tlc5940.update();
}
void clearFrameBuffer() {
  for (int i = 0 ; i < HEIGHT; i++) {
    for (int j = 0 ; j < WIDTH; j++) {
      frame_buffer[i][j] = false;
    }
  }
}
void refreshBuffer() {
  for (int i = 0 ; i < HEIGHT; i++) {
    for (int j = 0 ; j < WIDTH; j++) {
      frame[i][j] = frame_buffer[i][j];
    }
  }
}
void setLED(int x, int y, bool value) {
  int index = y * WIDTH + x;
  index = mapping[index];
  if (value) {
    tlc5940.set(index, BRIGHTNESS);
  } else {
    tlc5940.set(index, 0);
  }
}
bool valid(int x, int y) {
  if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH) {
    return false;
  }
  return true;
}
int buttonStatus() {
  int value = analogRead(PB3);
  value = value / (1024 / 15);
  int key = 0;
  switch (value) {
    case 11:
      key = 2;
      break;
    case 13:
      key = 1;
      break;
    case 12:
      key = 3;
  }
  return key;
}
bool keyPressed() {
  int reading = buttonStatus();
  if (reading != last_stroke) {
    last_press = millis();
  }
  if ((millis() - last_press) > debounce_delay) {
    if (reading != key_stroke) {
      key_stroke = reading;
      return true;
    }
  }
  return false;
}

