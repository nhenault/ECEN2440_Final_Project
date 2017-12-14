/*
 * functions.h
 *
 *  Created on: Nov 19, 2017
 *      Author: Nathan
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

enum Melee {
    SLOW_FORWARD = 0,
    FAST_FORWARD,
    SLOW_BACKWARD,
    FAST_BACKWARD,
    SLOW_LEFT,
    FAST_LEFT,
    SLOW_RIGHT,
    FAST_RIGHT,
    STOP,
    NO_ATTACK = 16,
    SLASH = 32,
    FIRE = 64,
    SPIN = 128,
    SWITCH = 255
};

enum Ranged {
    BACKWARD = 98,
    FORWARD = 102,
    GO = 103,
    LEFT = 108,
    MELEE = 109,
    RIGHT = 114,
    SHOOT = 115,
};

/* A function used in the itoa
 * function.
 */
void reverse(char s[]);

/* A function which converts an
 * int value to a string.
 */
void itoa(int n, char s[]);

/* A function used in the intToStr
 * function.
 */
void rev(char *str, int len);

/* A function used in the ftoa
 * function.
 */
int intToStr(int x, char str[], int d);

/* A function which converts a
 * float value to a string.
 */
void ftoa(float n, char *res, int afterpoint);

#endif /* FUNCTIONS_H_ */
