#include "object.h"


int Object_flag_compare(uint32_t data, uint32_t mask) {
    data |= 0xff000000;
    mask |= 0xff000000;
    return (data & mask) == mask;

}


void Object_flag_set(uint32_t* data, uint32_t mask) {
    *data |= mask;
}


void Object_flag_unset(uint32_t* data, uint32_t mask) {
    mask &= 0x00ffffff;
    *data &= ~mask;
}


