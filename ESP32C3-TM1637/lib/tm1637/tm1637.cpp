/*
 * @Description:
 * @Author: chenzedeng
 * @Date: 2023-06-09 11:08:46
 * @LastEditTime: 2023-06-09 15:08:14
 */

//  --0x01--
// |        |
// 0x20     0x02
// |        |
//  --0x40- -
// |        |
// 0x10     0x04
// |        |
//  --0x08--

#include "tm1637.h"

uint8_t _PointFlag = 0;

uint8_t char2segments(char c) {
    switch (c) {
        case '_':
            return 0x08;
        case '^':
            return 0x01;  // ¯
        case '-':
            return 0x40;
        case '*':
            return 0x63;  // °
        case ' ':
            return 0x00;  // space
        case 'A':
            return 0x77;  // upper case A
        case 'a':
            return 0x5f;  // lower case a
        case 'B':         // lower case b
        case 'b':
            return 0x7c;  // lower case b
        case 'C':
            return 0x39;  // upper case C
        case 'c':
            return 0x58;  // lower case c
        case 'D':         // lower case d
        case 'd':
            return 0x5e;  // lower case d
        case 'E':         // upper case E
        case 'e':
            return 0x79;  // upper case E
        case 'F':         // upper case F
        case 'f':
            return 0x71;  // upper case F
        case 'G':         // upper case G
        case 'g':
            return 0x35;  // upper case G
        case 'H':
            return 0x76;  // upper case H
        case 'h':
            return 0x74;  // lower case h
        case 'I':
            return 0x06;  // 1
        case 'i':
            return 0x04;  // lower case i
        case 'J':
            return 0x1e;  // upper case J
        case 'j':
            return 0x16;  // lower case j
        case 'K':         // upper case K
        case 'k':
            return 0x75;  // upper case K
        case 'L':         // upper case L
        case 'l':
            return 0x38;  // upper case L
        case 'M':         // twice tall n
        case 'm':
            return 0x37;  // twice tall ∩
        case 'N':         // lower case n
        case 'n':
            return 0x54;  // lower case n
        case 'O':         // lower case o
        case 'o':
            return 0x5c;  // lower case o
        case 'P':         // upper case P
        case 'p':
            return 0x73;  // upper case P
        case 'Q':
            return 0x7b;  // upper case Q
        case 'q':
            return 0x67;  // lower case q
        case 'R':         // lower case r
        case 'r':
            return 0x50;  // lower case r
        case 'S':         // 5
        case 's':
            return 0x6d;  // 5
        case 'T':         // lower case t
        case 't':
            return 0x78;  // lower case t
        case 'U':         // lower case u
        case 'u':
            return 0x1c;  // lower case u
        case 'V':         // twice tall u
        case 'v':
            return 0x3e;  // twice tall u
        case 'W':
            return 0x7e;  // upside down A
        case 'w':
            return 0x2a;  // separated w
        case 'X':         // upper case H
        case 'x':
            return 0x76;  // upper case H
        case 'Y':         // lower case y
        case 'y':
            return 0x6e;  // lower case y
        case 'Z':         // separated Z
        case 'z':
            return 0x1b;  // separated Z
    }
    return 0;
}

static int8_t tube_tab[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d,
                            0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c,
                            0x39, 0x5e, 0x79, 0x71};  // 0~9,A,b,C,d,E,F

/**
 * 数据传输开始
 * 数据输入的开始条件是 CLK
为高电平时，DIO 由高变低；
*/
void dat_start() {
    DIO_OUT;
    tm_delay_us(2);
    CLK_1;
    DIO_1;
    tm_delay_us(2);
    DIO_0;
    CLK_0;
}

/**
 * 传输结束
 * 结束条件是 CLK 为高时，DIO 由低电平变为高电平
 */
void dat_stop() {
    CLK_0;
    DIO_0;
    tm_delay_us(2);
    CLK_1;
    DIO_1;
}

int dat_ack_check() {
    uint8_t ack = 0, errorRetry = 5;
    DIO_1;
    tm_delay_us(5);
    DIO_IN;
    tm_delay_us(5);
    CLK_0;
    tm_delay_us(5);
    while (DIO_R) {
        if (errorRetry <= 0) {
            break;
        }
        tm_delay_us(1);
        errorRetry--;
    }
    if (errorRetry) {
        ack = 1;
        CLK_1;
        tm_delay_us(5);
    }
    CLK_0;
    DIO_OUT;
    return ack;
}

int dat_send8(uint8_t data) {
    // 这个发送时序是LSB 从低位到高位
    for (int i = 0; i < 8; i++) {
        CLK_0;
        if (data & 0x01) {
            DIO_1;
        } else {
            DIO_0;
        }
        tm_delay_us(3);
        CLK_1;
        tm_delay_us(3);
        data >>= 1;
    }
    return dat_ack_check();
}

/**
 * 固定地址模式
 */
void tm_sram_lock() {
    dat_start();
    dat_send8(0x44);  // 固定地址模式
    dat_stop();
}

void tm_set_point(uint8_t PointFlag) {
    _PointFlag = PointFlag;
}

// void coding(int8_t disp_data[]) {
//     for (uint8_t i = 0; i < DIGITS; i++) {
//         disp_data[i] = coding(disp_data[i]);
//     }
// }

uint8_t coding(uint8_t disp_data) {
    if (disp_data == 0x7f) {
        disp_data = 0x00;  // Clear digit
    } else if (disp_data >= 0 &&
               disp_data < int(sizeof(tube_tab) / sizeof(*tube_tab))) {
        disp_data = tube_tab[disp_data];
    } else if (disp_data >= '0' && disp_data <= '9') {
        disp_data =
            tube_tab[int(disp_data) - 48];  // char to int (char "0" = ASCII 48)
    } else {
        disp_data = char2segments(disp_data);
    }
    disp_data += _PointFlag == 1 ? 0x80 : 0;

    return disp_data;
}

void tm_lock_show(uint8_t index, uint8_t number) {
    uint8_t zero = 0xc0;
    dat_start();
    dat_send8(zero + index);  // 发送地址
    dat_send8(coding(number));
    dat_stop();
}

void tm_lock_light(uint8_t commant) {
    dat_start();
    dat_send8(commant);
    dat_stop();
}

void tm_init(void) {
    DIO_OUT;
    CLK_OUT;
    tm_sram_lock();
}
void tm_display(uint8_t index, uint8_t number) {
    tm_lock_show(index, number);
    tm_lock_light(0x8f);
}

void tm_clear(void) {
    tm_display(0x00, 0x7f);
    tm_display(0x01, 0x7f);
    tm_display(0x02, 0x7f);
    tm_display(0x03, 0x7f);
}
