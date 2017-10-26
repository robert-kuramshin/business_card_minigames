#include <EEPROM.h>
#include <ascii_matrix.h>

void setup() {
    byte bit_count = 0;
    byte b[] = {B00000000,B00000000};
    short count = 0;
    for(byte letter = 0; letter < sizeof(capitals) / sizeof(capitals[0]);letter++){
        count = 0;
        for(byte row = 0; row < 5;row++){
            for (byte element = 0; element < 3;element++){
                b[bit_count+count/8] = b[bit_count+count/8]<<1;
                if (capitals[letter][row][element]){
                    b[bit_count+count/8] += 1;
                }else {
                     b[bit_count+count/8] += 0;
                }
                count+=1;
            }
        }
        EEPROM.put(bit_count, b[bit_count]);
        EEPROM.put(bit_count+1, b[bit_count+1]);
        bit_count +=2;
    }
}

void loop() {

}
