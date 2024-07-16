/*
 *  MP3 lib
 *  pyroesp
 */

#include "mp3.h"


void mp3_init(Mp3 *m){
    int8_t i;
    m->status = MP3_OFF;
    usart_init(&m->com);
    for (i = 0; i < MP3_FRAME_SIZE; i++){
        m->frame[i] = 0;
    }
    m->frame_idx = 0;
    
    m->start = MP3_FRAME_START;
    m->version = MP3_FRAME_VERSION;
    m->length = MP3_FRAME_LENGTH;
    m->feedback = MP3_FRAME_FEEDBACK_OFF;
    m->stop = MP3_FRAME_STOP;
    return;
}

void mp3_update(Mp3 *m){
    switch (m->status){
        case MP3_PLAY:
            mp3_play_cmd(m);
            m->status = MP3_CHECKSUM:
            break;
        case MP3_PAUSE:
            mp3_pause_cmd(m);
            m->status = MP3_CHECKSUM:
            break;
        case MP3_STOP:
            mp3_stop_cmd(m);
            m->status = MP3_CHECKSUM:
            break;
        case MP3_CHECKSUM:
            mp3_checksum(m);
            m->status = MP3_SEND_FRAME;
            break;
        case MP3_SEND_FRAME:
            // #TODO: Send frame to USART
            mp3_send_frame(m);
            if (m->frame_idx >= MP3_FRAME_SIZE){
                m->frame_idx = 0;
                m->status = MP3_WAITING;
            }
            break;
        case MP3_WAITING:
        case MP3_OFF:
        default:
            m->frame_idx = 0;
            break;
    }
}

void mp3_send_frame(Mp3 *m){
    if (m->com.usart_check_tx_complete()){          // only send a new byte if the previous byte has been transmitted
        m->com.usart_write(m->frame[m->frame_idx]); // write new byte from frame to transmit
        m->frame_idx++;                             // increase frame_idx
    }
    return;
}

// MP3 set play status
void mp3_set_status_play(Mp3 *m){
    if (m->status == MP3_WAITING || m->status == MP3_OFF){
        m->status = MP3_PLAY;
    }
    return;
}

// MP3 set pause status
void mp3_set_status_pause(Mp3 *m){
    if (m->status == MP3_WAITING || m->status == MP3_OFF){
        m->status = MP3_PAUSE;
    }
    return;
}

// MP3 set stop status
void mp3_set_status_stop(Mp3 *m){
    if (m->status == MP3_WAITING || m->status == MP3_OFF){
        m->status = MP3_STOP;
    }
    return;
}

void mp3_play_cmd(Mp3 *m){
    m->cmd = MP3_CMD_PLAY;
    return;
}

void mp3_pause_cmd(Mp3 *m){
    m->cmd = MP3_CMD_PAUSE;
    return;
}

void mp3_stop_cmd(Mp3 *m){
    m->cmd = MP3_CMD_STOP;
    return;
}

void mp3_checksum(Mp3 *m){
    m.chksm = 0xFFFF - (m.version + m.length + m.cmd + m.feedback + m.param_hi + m.param_lo) + 1;
    return;
}