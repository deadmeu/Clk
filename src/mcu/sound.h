/*
 * sound.h
 *
 * Written by Alex Subaric.
 */

#ifndef SOUND_H_
#define SOUND_H_

void play_note(int note);
// void delay_note(int duration);
void setup_sound(void);
void sound_on(void);
void sound_off(void);
// void play_scale(void);
// void play_octaves(void);
// void play_big_ben(void);
void play_alarm(void);
void update_sound(void);
// void play_sheet(uint8_t *sheet, uint8_t size);    
// void silence(int duration);

#endif /* SOUND_H_ */
