#include "HuskyLensProtocolCore.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include <stdlib.h>

#define FRAME_BUFFER_SIZE 128

#define HEADER_0_INDEX      0
#define HEADER_1_INDEX      1
#define ADDRESS_INDEX       2
#define CONTENT_SIZE_INDEX  3
#define COMMAND_INDEX       4
#define CONTENT_INDEX       5
#define PROTOCOL_SIZE       6

static uint8_t send_buffer[FRAME_BUFFER_SIZE];
static uint8_t receive_buffer[FRAME_BUFFER_SIZE];

static short send_index = 0;
static short receive_index = 0;

static bool send_fail = false;
static bool receive_fail = false;

static short content_current = 0;
static short content_end = 0;
static bool content_read_end = false;

bool validateCheckSum(){
    uint8_t stackSumIndex=receive_buffer[CONTENT_SIZE_INDEX] + CONTENT_INDEX;
    uint8_t sum = 0;
  
    //calculate the checksum
    for (uint8_t i=0; i< stackSumIndex; i++) {
        sum+=receive_buffer[i];
    }

    return (sum == receive_buffer[stackSumIndex]);
}

bool husky_lens_protocol_receive(uint8_t data){
    switch (receive_index)
    {
    case HEADER_0_INDEX:
        if (data!=0x55) {receive_index = 0; return false;}
        receive_buffer[HEADER_0_INDEX] = 0x55;
        break;
    case HEADER_1_INDEX:
        if (data!=0xaa) {receive_index = 0; return false;}
        receive_buffer[HEADER_1_INDEX] = 0xaa;
        break;
    case ADDRESS_INDEX:
        receive_buffer[ADDRESS_INDEX] = data;
        break;
    case CONTENT_SIZE_INDEX:
        if (data >= FRAME_BUFFER_SIZE-PROTOCOL_SIZE) {receive_index = 0; return false;}
        receive_buffer[CONTENT_SIZE_INDEX] = data;
        break;
    default:
        receive_buffer[receive_index]=data;
        if (receive_index==receive_buffer[CONTENT_SIZE_INDEX]+CONTENT_INDEX) {
            content_end = receive_index;
            receive_index=0;
            return validateCheckSum();
        }
        break;
    }
    receive_index++;
    return false;
}

bool husky_lens_protocol_read_begin(uint8_t command){
    if (command == receive_buffer[COMMAND_INDEX])
    {
        content_current = CONTENT_INDEX;
        content_read_end = false;
        receive_fail = false;
        return true;
    }
    return false;
}

uint8_t husky_lens_protocol_read_uint8(){
    if (content_current >= content_end || content_read_end){receive_fail = true; return 0;}
    uint8_t result;
    memcpy(&result, receive_buffer + content_current, sizeof(result));
    content_current += sizeof(result);
    return result;
}

int16_t husky_lens_protocol_read_int16(){
    if (content_current >= content_end || content_read_end){receive_fail = true; return 0;}
    int16_t result;
    memcpy(&result, receive_buffer + content_current, sizeof(result));
    if (IS_BIG_ENDIAN()){__builtin_bswap16(result);}
    content_current += sizeof(result);
    return result;
}

int32_t husky_lens_protocol_read_int32(){
    if (content_current >= content_end || content_read_end){receive_fail = true; return 0;}
    int32_t result;
    memcpy(&result, receive_buffer + content_current, sizeof(result));
    if (IS_BIG_ENDIAN()){__builtin_bswap32(result);}
    content_current += sizeof(result);
    return result;
}

float husky_lens_protocol_read_float(){
    if (content_current >= content_end || content_read_end){receive_fail = true; return 0;}
    float result;
    memcpy(&result, receive_buffer + content_current, sizeof(result));
    if (IS_BIG_ENDIAN()){__builtin_bswap32(result);}
    content_current += sizeof(result);
    return result;
}

bool husky_lens_protocol_read_end(){
    if (receive_fail)
    {
        receive_fail = false;
        return false;
    }
    return content_current == content_end;
}

uint8_t* husky_lens_protocol_write_begin(uint8_t command){
    send_fail = false;
    send_buffer[HEADER_0_INDEX] = 0x55;
    send_buffer[HEADER_1_INDEX] = 0xAA;
    send_buffer[ADDRESS_INDEX] = 0x11;
    send_buffer[COMMAND_INDEX] = command;
    send_index = CONTENT_INDEX;
    return &send_buffer;
}

void husky_lens_protocol_write_uint8(uint8_t content){
    if(send_index + sizeof(content) >= FRAME_BUFFER_SIZE) {send_fail = true; return;}
    memcpy(send_buffer + send_index, &content, sizeof(content));
    send_index += sizeof(content);
}

void husky_lens_protocol_write_int16(int16_t content){
    if(send_index + sizeof(content) >= FRAME_BUFFER_SIZE) {send_fail = true; return;}
    if (IS_BIG_ENDIAN()){__builtin_bswap16(content);}
    memcpy(send_buffer + send_index, &content, sizeof(content));
    send_index += sizeof(content);
}

void husky_lens_protocol_write_int32(int32_t content){
    if(send_index + sizeof(content) >= FRAME_BUFFER_SIZE) {send_fail = true; return;}
    if (IS_BIG_ENDIAN()){__builtin_bswap32(content);}
    memcpy(send_buffer + send_index, &content, sizeof(content));
    send_index += sizeof(content);
}

void husky_lens_protocol_write_float(float content){
    if(send_index + sizeof(content) >= FRAME_BUFFER_SIZE) {send_fail = true; return;}
    if (IS_BIG_ENDIAN()){__builtin_bswap32(content);}
    memcpy(send_buffer + send_index, &content, sizeof(content));
    send_index += sizeof(content);
}

void husky_lens_protocol_write_buffer_uint8(uint8_t *content, uint32_t length)
{
    if(send_index + sizeof(uint8_t) * length >= FRAME_BUFFER_SIZE) {send_fail = true; return;}
    memcpy(send_buffer + send_index, content, sizeof(uint8_t) * length);
    send_index += sizeof(uint8_t) * length;
}

int husky_lens_protocol_write_end(){
    if(send_fail) {return 0;}
    if(send_index + 1 >= FRAME_BUFFER_SIZE) {return 0;}
    send_buffer[CONTENT_SIZE_INDEX] = send_index - CONTENT_INDEX;
    uint8_t sum = 0;
    for (int i = 0; i < send_index; i++)
    {
        sum += send_buffer[i];
    }
    send_buffer[send_index] = sum;
    send_index ++;
    return send_index;
}