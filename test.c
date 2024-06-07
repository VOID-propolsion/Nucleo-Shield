#include <json-c/json.h>
#include <json-c/json_object.h>
#include <libpynq.h>
#include <platform.h>
#include <stdint.h>
#include <stepper.h>
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

#include <time.h>


#define MOVE_CONST    63.26656122907641
#define ROTATE_CONST  6.901311249137336

#define LEFT_IR   IO_AR12
#define RIGHT_IR  IO_AR11 
#define FRONT_IR  IO_AR13

json_object *generate_json_object(int x, int y, char *timestamp){
   // {}
   json_object *child = json_object_new_object();
   //  {"robot": ... }
   json_object_object_add(child, "x", json_object_new_int(x));
   // {"loglevel":"...", "msg":"..."}
   json_object_object_add(child, "y", json_object_new_int(y));
   // {"loglevel":"...", "msg":"...", "timestamp":"..."}
   json_object_object_add(child, "color", json_object_new_string(timestamp));
   return child;
}

int x_coor;
int y_coor;
int angle;

void send_data(int x, int y, char* color) {
  char timestamp[10];

  sprintf(timestamp, "%d", x);  // Create a simple timestamp as a string
  json_object *packet = generate_json_object(x, y, color);

  const char *json_str = json_object_to_json_string(packet);
  printf("JSON String: %s\n", json_str);

  int n = strlen(json_str);
  unsigned char size_of_byte[4];

  size_of_byte[3] = (n >> 24) & 0xFF;
  size_of_byte[2] = (n >> 16) & 0xFF;
  size_of_byte[1] = (n >> 8) & 0xFF;
  size_of_byte[0] = n & 0xFF;
  for (int b = 0; b < 4; b++) {
    int send = 1;
    while (send) {
      uart_send (UART0, size_of_byte[b]);
      send = gpio_get_level(IO_AR2);
    }
  }

  for (int c = 0; c < n; c++) {
    int send = 1;
    while (send) {
      uart_send (UART0, json_str[c]);
      send = gpio_get_level(IO_AR2);
    }
  }
  sleep_msec(5000);
}

void init(void) {
  printf("initializing...\n");
  pynq_init();
  gpio_init();
  switchbox_set_pin(IO_AR0, SWB_UART0_RX);
  switchbox_set_pin(IO_AR1, SWB_UART0_TX);
  uart_init(UART0);
  uart_reset_fifos(UART0);

  // Initialize the stepper driver.
  stepper_init();
  // Apply power to the stepper motors.
  stepper_enable();
  // Move one full rotation.
  stepper_set_speed(16000,16000);
  printf("finished.\n");
}

void move(int d) {
  int steps = (int)(d*MOVE_CONST);
  stepper_steps(steps, steps);
}

bool stepper_wait(int irq) {
  while(!stepper_steps_done()) {
    if (gpio_get_level(irq)) {
      printf("line detected\n");
      stepper_reset();
      sleep_msec(100);
      stepper_enable();
      return false;
    }
    sleep_msec(10);
  }
  return true;
}

void rotate(int theta) {
  float left = -ROTATE_CONST * theta;
  float right = ROTATE_CONST * theta;
  stepper_steps((int) left, (int) right);
}

void startup() {
  while(!gpio_get_level(FRONT_IR)) {
    move(1);
    stepper_wait(FRONT_IR);
  }
  
  sleep_msec(500);

  rotate(360);
  stepper_wait(LEFT_IR);
  sleep_msec(500);
  int degree = 0;
  while(!gpio_get_level(RIGHT_IR)) {
    rotate(-1);
    stepper_wait(RIGHT_IR);
    degree++;
    if (degree > 360) {
      break;
    }
  }
  sleep_msec(500);
  rotate((int) (degree / 2));
  stepper_wait(LEFT_IR);
  // MOVE BACK
  move(-2);
  stepper_wait(RIGHT_IR);
  // TURN RIGHT
  rotate(90);
  stepper_wait(RIGHT_IR);

    // readjust if it misaligns the left IR sensor
  while (!gpio_get_level(LEFT_IR)) {
    rotate(-90);
    stepper_wait(RIGHT_IR);
    move(1);
    stepper_wait(FRONT_IR);
    rotate(90);
    stepper_wait(RIGHT_IR)
  }

  while(!gpio_get_level(FRONT_IR)) {
    move(1);
    while(!stepper_steps_done()) {
        if (gpio_get_level(FRONT_IR) || !gpio_get_level(LEFT_IR)) { // keep driving until something in front or detect nothing on the left
        sleep_msec(1000);
        if (gpio_get_level(LEFT_IR)) {
            break; // check if it was a false positive
        }
        printf("line detected\n");
        stepper_reset();
        sleep_msec(100);
        stepper_enable();
        return false;
        }
        sleep_msec(10);
    }
  }

  int x = 0;
  int y = 0;

//   while(!gpio_get_level(FRONT_IR)) {
//     move(1);
//     stepper_wait(FRONT_IR);
//   }
}


int main(void) {
  pynq_init();
  switchbox_set_pin(IO_AR0, SWB_UART0_RX);
  switchbox_set_pin(IO_AR1, SWB_UART0_TX);

  // Initialize the stepper driver.
  stepper_init();
  // Apply power to the stepper motors.
  stepper_enable();
  // Move one full rotation.
  stepper_set_speed(16000,16000);

  uart_init(UART0);

  uart_reset_fifos(UART0);
  printf("\n");
  
  // int left, right;


  while (1) {
    if (uart_has_data(UART0)) {
      printf("\n");
      uint8_t length[4];
      for (int x = 0; x < 4; x++) {
        length[x] = uart_recv(UART0);
      }
      uint8_t s = length[0] + (length[1] << 2) + (length[2] << 4) + (length[3] << 6);
      printf("length: %d\n", s);
      char buffer[s];
      buffer[s] = '\0';

      for (int i = 0; i < s; i++) {
        buffer[i] = (char) uart_recv(UART0);
        //printf("%02X ", buffer[i]);
      }
      printf("%s\n", buffer);
      if (strcmp(buffer, "straight") == 0) {
          move(10);
      } else if (strcmp(buffer, "spin") == 0) {
          stepper_steps(2439, -2439);
      } else if (strcmp(buffer, "left") == 0) {
          stepper_steps(663, -663);
      } else if (strcmp(buffer, "right") == 0) {
          stepper_steps(-663, 663);
      } else if (strcmp(buffer, "back") == 0) {
          stepper_steps(-1325,1325);
      } else if (strcmp(buffer, "start") == 0) {
        
      } else if (strcmp(buffer, "rotate") == 0) {
        rotate(90);
      }
      // sscanf(buffer, "%d %d", &left, &right);

      // stepper_steps(left, right);
      // sleep_msec(100);

    }
  }
}