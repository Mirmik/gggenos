#include "genos/debug/debug.h"
#include "kernel/diag.h"

#include "hal/arch.h"
#include "util/bits.h" 

#include <iostream>

int main(){
arch_init();

uint8_t a = 0b00000000;
uint8_t bias = 2;

uint8_t mask = 0b00000111;
uint8_t c = 0b00000101;
uint8_t d = 0b00010100;

bits_mask_assign_bias(a, c, mask, bias);

std::cout << std::hex << (int)a << std::endl << (int)d << std::endl;

}