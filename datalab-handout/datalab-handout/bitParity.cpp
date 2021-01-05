
#include <cstdint>
#include <cstdio>

void bitParity4bit(int8_t x) {
    int8_t mask = (x&0b01)^((x&(0b010))>>1);
    int8_t mask2 = ((x&0b01000)>>3)^((x&0b0100)>>2);
    int8_t ret = mask ^ mask2;
    printf("x=%d, res=%d\n", x,ret);
    return;
}

int test4bit(){
    int8_t i = (7)&0b001111;
    while (i>=-8) {
        bitParity4bit(i);
        i--;
    }
    return 0;
}

int main(void){
    test4bit();
    return 0;
}
