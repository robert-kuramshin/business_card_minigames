#include <EEPROM.h>
#include <ascii_matrix.h>

bool test = true;
void setup() {

}

void loop() {
    test = false;
    byte bit_count = 0;
    byte b[] = {B00000000,B00000000};
    short count = 0;
    for(byte letter = 0; letter < 27;letter++){
        count = 0;
        for(byte row = 0; row < 5;row++){
            for (byte element = 0; element < 3;element++){
                b[count/8] = b[count/8]<<1;
                if (capitals[letter][row][element]){
                    b[count/8] += 1;
                }else {
                     b[count/8] += 0;
                }
                count+=1;

            }
        }
        EEPROM.put(bit_count, b[0]);
        EEPROM.put(bit_count+1, b[1]);

        bit_count +=2;
        delay(10);
    }

    
}
