/*
 * @Description:
 * @Author: chenzedeng
 * @Date: 2023-06-09 21:55:39
 * @LastEditTime: 2023-06-09 22:11:08
 */
#include <Arduino.h>

#include "tm1637.h"

unsigned int counter = 0;
int point_s = 1;

void setup() {
    Serial.begin(115200);
    Serial.println("Start...");
    tm_init();
    tm_clear();
    delay(100);
    tm_set_point(point_s);
}

void loop() {
    // point_s = !point_s;
    // tm_set_point(point_s);
    tm_display(0, (counter / 1000) % 10);
    tm_display(1, (counter / 100) % 10);
    tm_display(2, (counter / 10) % 10);
    tm_display(3, counter % 10);

    counter++;
    if (counter == 10000) {
        counter = 0;
    }

    delay(100);
}
