//  bounce_digital
//
//  bounce back and forth across a defined set of outputs using digital I/O
//
//
// Built using the 1.6.7 Arduino IDE
//
//
// Copyright (c) 2016, Christian Herzog
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other materials
//    provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be used
//    to endorse or promote products derived from this software without specific prior written
//    permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
// AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.


#define LED_OVERLAP     2   // max number active at once (first going up, last going down, in between on)
                            // must be greater than 1!
                            
#define LED_ON_TIME     100 // milliseconds


#define LED_ON          1
#define LED_OFF         0


typedef struct  {
  int port;
  int state;
} LEDS;

LEDS  leds[] = {
  3,  0,           // port #, initial state (initialized later, just 0 for now)
  5,  0,
  6,  0,
  9,  0,
  10, 0,
  11, 0
};


// void incLeds( *LEDS, unsigned int);



void setup() {

unsigned int i;

  for (i = 0; i < sizeof(leds) / sizeof(leds[0]) ; i++) {

    if (i < LED_OVERLAP) {
      leds[i].state = LED_ON;         // start with LED_OVERLAP # of LEDs on and the rest off
    } else {
      leds[i].state = LED_OFF;
    }
    
    pinMode(leds[i].port, OUTPUT);
    digitalWrite(leds[i].port, 0);    // set the LEDs all off at the start
  }
}




void loop() {

unsigned int i;

  while (1) {

    for (i = 0; i < sizeof(leds) / sizeof(leds[0]) ; i++) {
      digitalWrite(leds[i].port, leds[i].state);          // on or off according to the current state
    }

    delay(LED_ON_TIME);                                   // just leave things alone for the ON time

    incLeds(leds, sizeof(leds) / sizeof(leds[0]));        // "bounce" to the next sequence of LEDS
  }
}


#define SHIFT_UP    1
#define SHIFT_DOWN  0

int direction = SHIFT_UP;                                 // the direction changes as we bounce from
                                                          // one end of the LED port to the other
                                                          // needs to be a global

void incLeds(LEDS ledList[], int total)

{
int i;

  if (direction == SHIFT_UP) {                  // shifting up
    
    for (i = total - 1 ; i >= 0 ; i--) {        // run through the LEDs from the top except for the first one
      ledList[i + 1].state = ledList[i].state;  // shift up one position
    }
  
    ledList[0].state = LED_OFF;                 // first LED is off

    if (ledList[total - 1].state == LED_ON) {
      direction = SHIFT_DOWN;                   // change direction if last is on
    }
    
  } else {                                      // shifting down
    
    for (i = 0 ; i < total - 1 ; i++) {         // run through the number of LEDs configured except for the last one
      ledList[i].state = ledList[i + 1].state;  // shift down one position
    }
  
    ledList[total - 1].state = LED_OFF;         // last LED is off

    if (ledList[0].state == LED_ON) {
      direction = SHIFT_UP;                     // change direction if first is on
    }
  }
}
