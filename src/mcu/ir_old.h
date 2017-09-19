#ifndef IR_H_
#define IR_H_

void init_carrier(void);
void enable_carrier(void);
void IR_carrier(uint16_t IRTimeMicroSeconds);
void send_code(uint32_t IRcode);
void send_1(void);
void send_0(void);

#endif
