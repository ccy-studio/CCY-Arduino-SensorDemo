/*
 * @Description:
 * @Author: chenzedeng
 * @Date: 2023-06-09 10:58:44
 * @LastEditTime: 2023-06-09 15:09:51
 */
#ifndef TM1637_H__
#define TM1637_H__

#include <Arduino.h>

#define CLK_PIN 2
#define DIO_PIN 3

#define CLK_0 digitalWrite(CLK_PIN, LOW)
#define CLK_1 digitalWrite(CLK_PIN, HIGH)
#define DIO_0 digitalWrite(DIO_PIN, LOW)
#define DIO_1 digitalWrite(DIO_PIN, HIGH)
#define DIO_R digitalRead(DIO_PIN)

#define CLK_OUT pinMode(CLK_PIN, OUTPUT)
#define DIO_OUT pinMode(DIO_PIN, OUTPUT)
#define DIO_IN pinMode(DIO_PIN, INPUT)

// 微秒延迟
#define tm_delay_us(us_num) delayMicroseconds(us_num)

void tm_init(void);
void tm_display(uint8_t index, uint8_t numer);
void tm_clear(void);
void tm_set_point(uint8_t PointFlag);

#endif