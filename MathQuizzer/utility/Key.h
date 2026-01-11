#ifndef KEY_H
#define KEY_H

#include <Arduino.h>

#define NO_KEY '\0'

typedef enum { IDLE, PRESSED, HOLD, RELEASED } KeyState;

typedef struct {
    char kchar;
    int kcode;
    KeyState kstate;
    boolean stateChanged;
} Key;

#endif
