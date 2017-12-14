/*
 * action.h
 *
 *  Created on: Nov 22, 2017
 *      Author: Nathan
 */

#ifndef FUNCTIONS_FINAL_H_
#define FUNCTIONS_FINAL_H_

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

/* The function which dictates robot
 * motion based on inputs received
 * through uart in melee mode.
 * @param attack = type of attack.
 * @param motion = direction/speed of motion.
 */
void Robot_Action_Melee(uint8_t attack, uint8_t motion);

/* The function which dictates robot
 * motion based on inputs received
 * through uart in ranged mode.
 * @param direction = direction of motion.
 * @param distance = distance or degrees of motion.
 */
void Robot_Action_Ranged(uint8_t direction, float distance);

/* The function which empties the cached
 * ranged instructions and calls the
 * Robot_Action_Ranged function for
 * movement control.
 */
void Ranged_Commands(void);

/* Function called when a UART command
 * is received.
 */
void Receive_Function(void);

#endif /* FUNCTIONS_FINAL_H_ */
