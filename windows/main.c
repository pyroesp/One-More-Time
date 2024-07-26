#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "../include/clock.h"


void printRow(uint8_t v, uint8_t row);
void printTime(Clock *c);
void cursor_reset(void);


int main(int argc, char* argv[]){
    int16_t i = 0;
    Clock c;
    clock_init(&c);

    uint8_t button_in = 0xFF;

    // assign buttons
    button_set_io(&c.buttons[BUTTON_ALARM].btn, (uint8_t*)&button_in, BUTTON_ALARM); // PC0
    button_set_io(&c.buttons[BUTTON_HOUR].btn, (uint8_t*)&button_in, BUTTON_HOUR); // PC1
    button_set_io(&c.buttons[BUTTON_MINUTE].btn, (uint8_t*)&button_in, BUTTON_MINUTE); // PC2
    button_set_io(&c.buttons[BUTTON_SNOOZE].btn, (uint8_t*)&button_in, BUTTON_SNOOZE); // PC3

    button_copy_state(c.buttons, c.prev_buttons, BUTTON_SIZE);

    uint8_t segment_out = 0;
    uint8_t grid_out = 0;

    // assign VFD io
    vfd_set_io(&c.vfd.grid, (uint8_t*)&segment_out, 0xFC, 2); // PA2~PA7
    vfd_set_io(&c.vfd.segment, (uint8_t*)&grid_out, 0xFF, 0); // PD0~PD7

    // time stuff
    time_t t;
    time((time_t*)&t);
    struct tm *lt = (struct tm*)localtime((const time_t*)&t);

    c.time.hour = lt->tm_hour;
    c.time.minute = lt->tm_min;
    c.time.second = lt->tm_sec;
    c.date.month = lt->tm_mon;
    c.date.day = lt->tm_mday;

    clock_update(&c, 0);

    #define SLEEP_TIME 10
    #define SLEEP_TIME_IN_1S (1000/SLEEP_TIME)

    for (;;){
        if (GetAsyncKeyState('Q') & 0x8000) // press q to exit
            break;

        printTime(&c);
        Sleep(SLEEP_TIME);
        clock_update(&c, SLEEP_TIME);
        i++;
        if (i >= SLEEP_TIME_IN_1S){
            time_update(&c.time);
            i = 0;
        }

        printf("time: %02i:%02i:%02i\n", c.time.hour, c.time.minute, c.time.second);
        printf("date: 20%02i/%02i/%02i\n", c.date.year, c.date.month, c.date.day);
        printf("mode: %s\n",  c.mode == CLOCK_MODE_TIME                 ? "CLOCK_MODE_TIME               " :
                                c.mode == CLOCK_MODE_CHANGE_HOUR        ? "CLOCK_MODE_CHANGE_HOUR        " :
                                c.mode == CLOCK_MODE_CHANGE_MINUTE      ? "CLOCK_MODE_CHANGE_MINUTE      " :
                                c.mode == CLOCK_MODE_CHANGE_ALARM       ? "CLOCK_MODE_CHANGE_ALARM       " :
                                c.mode == CLOCK_MODE_CHANGE_DAY_OF_WEEK ? "CLOCK_MODE_CHANGE_DAY_OF_WEEK " :
                                c.mode == CLOCK_MODE_DATE               ? "CLOCK_MODE_DATE               " :
                                                                          "CLOCK_MODE_UNKNOWN            "
        );
        printf("button: %1i%1i%1i%1i %1i%1i%1i%1i\n"
               , !!(button_in & 0x80), !!(button_in & 0x40), !!(button_in & 0x20), !!(button_in & 0x10)
               , !!(button_in & 0x8), !!(button_in & 0x4), !!(button_in & 0x2), !!(button_in & 0x1));

        printf("mode timeout: %4i\n", c.change_mode_time);
        printf("blink timeout: %4i\n", c.blink_time);


        if (GetAsyncKeyState('A') & 0x8000){ // press a for alarm
            button_in &= ~(1 << BUTTON_ALARM);
            printf("A - %1i - %3i\n", io_read_pin(&c.buttons[BUTTON_ALARM].btn), c.buttons[BUTTON_ALARM].pressed_time);
        } else {
            button_in |= (1 << BUTTON_ALARM);
            printf("               \n");
        }

        if (GetAsyncKeyState('Z') & 0x8000){ // press z for hour
            button_in &= ~(1 << BUTTON_HOUR);
            printf("Z - %1i - %3i\n", io_read_pin(&c.buttons[BUTTON_HOUR].btn), c.buttons[BUTTON_HOUR].pressed_time);
        } else {
            button_in |= (1 << BUTTON_HOUR);
            printf("               \n");
        }

        if (GetAsyncKeyState('E') & 0x8000){ // press e for minute
            button_in &= ~(1 << BUTTON_MINUTE);
            printf("E - %1i - %3i\n", io_read_pin(&c.buttons[BUTTON_MINUTE].btn), c.buttons[BUTTON_MINUTE].pressed_time);
        } else {
            button_in |= (1 << BUTTON_MINUTE);
            printf("               \n");
        }

        if (GetAsyncKeyState('R') & 0x8000){ // press r for snooze
            button_in &= ~(1 << BUTTON_SNOOZE);
            printf("R - %1i - %3i\n", io_read_pin(&c.buttons[BUTTON_SNOOZE].btn), c.buttons[BUTTON_SNOOZE].pressed_time);
        } else {
            button_in |= (1 << BUTTON_SNOOZE);
            printf("               \n");
        }

        cursor_reset();
    }

    return 0;
}


void printRow(uint8_t v, uint8_t row){
    switch(row){
        case 0: // A
            if (v & 0x01)
                printf(" ---   ");
            else
                printf("       ");
            break;
        case 1: // B-C
            if (v & 0x02) //B
                printf("|   ");
            else
                printf("    ");

            if (v & 0x04) //C
                printf("|  ");
            else
                printf("   ");
            break;
        case 2: // D
            if (v & 0x08)
                printf(" ---   ");
            else
                printf("       ");
            break;
        case 3: // E-F
            if (v & 0x10) //E
                printf("|   ");
            else
                printf("    ");

            if (v & 0x20) //F
                printf("|  ");
            else
                printf("   ");
            break;
        case 4: // G
            if (v & 0x40)
                printf(" ---   ");
            else
                printf("       ");
            break;
        case 5: // h
            if (v & 0x80)
                printf("    *  ");
            else
                printf("       ");
            break;
    }
}

void printTime(Clock *c){
    uint8_t i;
    for (i = 0; i < 6; i++){
        if (c->mode == CLOCK_MODE_CHANGE_HOUR && c->blink == CLOCK_BLINK_OFF){
            printRow(0, i);
            printRow(0, i);
        } else {
            printRow(c->vfd.display[VFD_DISPLAY_HOUR_TENS], i);
            printRow(c->vfd.display[VFD_DISPLAY_HOUR_UNIT], i);
        }
        if (i & 0x1 && i < 5)
            printf(" o ");
        else
            printf("   ");
        if (c->mode == CLOCK_MODE_CHANGE_MINUTE && c->blink == CLOCK_BLINK_OFF){
            printRow(0, i);
            printRow(0, i);
        } else {
            printRow(c->vfd.display[VFD_DISPLAY_MINUTE_TENS], i);
            printRow(c->vfd.display[VFD_DISPLAY_MINUTE_UNIT], i);
        }
        if (i & 0x1 && i < 5)
            printf(" o ");
        else
            printf("   ");
        printRow(c->vfd.display[VFD_DISPLAY_SECOND_TENS], i);
        printRow(c->vfd.display[VFD_DISPLAY_SECOND_UNIT], i);
        printf("\n");
    }

    return;
}

void cursor_reset(void){
    HANDLE hStdOut;
    COORD homeCoords = { 0, 0 };

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
        return;
    SetConsoleCursorPosition(hStdOut, homeCoords);
    return;
}
