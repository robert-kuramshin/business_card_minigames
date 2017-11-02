#include <Tiny85_TLC5940.h>
#define TICK 1000
#define WIDTH 3
#define HEIGHT 5

int lives = 3;
int pos = 1;

unsigned long last_tick;

int key_stroke = 0; //0 = nothing, 1 = left, 2 = up, 3 = right
int last_stroke = 0;
unsigned long last_press = 0;
int debounce_delay = 40;

bool last_row[3];

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
//    if (keyPressed()) {
//      //move
//      last_tick = millis();
//      refreshBuffer();
//    }
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
  long rand = random(0, 5);
  bool row[3] = {false, false, false};
  switch (rand) {
    case 0:
      row[0] = true;
      row[1] = false;
      row[2] = false;
      break;
    case 1:
      row[0] = true;
      row[1] = true;
      row[2] = false;
      break;
    case 2:
      row[0] = false;
      row[1] = true;
      row[2] = false;
      break;
    case 3:
      row[0] = false;
      row[1] = true;
      row[2] = true;
      break;
    case 4:
      row[0] = false;
      row[1] = false;
      row[2] = true;
      break;
  }
  bool dead_end = true;
  for(byte i =0;i<3;i++){
    if(last_row[i] == false && row[i]==false){
        dead_end = false;
    }
  }

  if(dead_end){
      row[0] = false;
      row[1] = false;
      row[2] = false;
  }

  for(byte i =0;i<3;i++){
    last_row[i] = row[i];
  }
  for (int y = HEIGHT - 2; y >= 0; y--) {
    for (int x = 0; x < 3; x++) {
      if (valid(x, y + 1) && valid(x, y)) {
        frame_buffer[y + 1][x] = frame_buffer[y][x];
      }
    }
  }
  for (int i = 0; i < 3; i++) {
    frame_buffer[0][i] = row[i];
  }
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
    tlc5940.set(index, 2000);
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

