/*
 *  MP3 lib
 *  pyroesp
 *
 *  https://github.com/DFRobot/DFRobotDFPlayerMini/blob/master/doc/FN-M16P%2BEmbedded%2BMP3%2BAudio%2BModule%2BDatasheet.pdf
 *
 */

#ifndef __MP3_H
#define	__MP3_H

#include <stdint.h>
#include "usart.h"


#define MP3_BAUDRATE 9600
#define MP3_FRAME_SIZE 10

// Frame bytes
#define MP3_FRAME_START 0x7E
#define MP3_FRAME_VERSION 0xFF
#define MP3_FRAME_STOP 0xEF
#define MP3_FRAME_LENGTH 0x06
#define MP3_FRAME_FEEDBACK_ON 0x01
#define MP3_FRAME_FEEDBACK_OFF 0x00

// Commands
#define MP3_CMD_SLEEP 0x0A
#define MP3_CMD_RESET 0x0C
#define MP3_CMD_PLAY 0x0D
#define MP3_CMD_PAUSE 0x0E
#define MP3_CMD_STOP 0x16


// State machine
typedef enum{
    MP3_OFF = 0,
    MP3_WAITING,
    MP3_RESET,
    MP3_SLEEP,
    MP3_PLAY,
    MP3_PAUSE,
    MP3_STOP,
    MP3_CHECKSUM,
    MP3_SEND_FRAME
}Mp3_State;


// MP3 struct
typedef struct{
    Mp3_State status;                   // MP3 status
    Usart com;                          // USART coms

    uint8_t frame_idx;
    union{                              // #TODO: Check endianness
        uint8_t frame[MP3_FRAME_SIZE];  // full frame of mp3 player
        struct{
            uint8_t start;
            uint8_t version;
            uint8_t length;
            uint8_t cmd;
            uint8_t feedback;
            union{
                uint16_t param;
                struct{
                    uint8_t param_hi;
                    uint8_t param_lo;
                };
            };
            union{
                uint16_t chksm;
                struct{
                    uint8_t chksm_hi;
                    uint8_t chksm_lo;
                };
            };
            uint8_t stop;
        };
    };
}Mp3;

// MP3 init
void mp3_init(Mp3 *m);
// MP3 update
void mp3_update(Mp3 *m);
// MP3 send frame
void mp3_send_frame(Mp3 *m);

// MP3 set status
void mp3_set_status_play(Mp3 *m);
void mp3_set_status_pause(Mp3 *m);
void mp3_set_status_stop(Mp3 *m);

// #TODO select song command
// #TODO select random song command

// MP3 play cmd
void mp3_play_cmd(Mp3 *m);
// MP3 pause cmd
void mp3_pause_cmd(Mp3 *m);
// MP3 stop cmd
void mp3_stop_cmd(Mp3 *m);

// MP3 Checksum
void mp3_checksum(Mp3 *m);

#endif

