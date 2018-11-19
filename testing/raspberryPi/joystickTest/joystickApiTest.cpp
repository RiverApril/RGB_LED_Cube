
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

/*

connect controller

$ sudo sixpair

then disconnect controller

$ sudo sixad -s

then press the PS button on controller

then start this program

*/


#define JS_EVENT_BUTTON 0x01
#define JS_EVENT_AXIS 0x02
#define JS_EVENT_INIT 0x80


int main(){

    int fd = open ("/dev/input/js0", O_RDONLY);

	struct js_event {
		unsigned int time;     /* event timestamp in milliseconds */
		signed short value;    /* value */
		unsigned char type;      /* event type */
		unsigned char number;    /* axis/button number */
	};
    
    js_event e;

    while(true){

        while (read (fd, &e, sizeof(e)) > 0) {

            if ((e.type & ~JS_EVENT_INIT) == JS_EVENT_BUTTON) {
                if(e.value){
                    printf("Down: %d\n", e.number);
                }else{
                    printf("Up: %d\n", e.number);
                }
            }else if((e.type & ~JS_EVENT_INIT) == JS_EVENT_AXIS){
                printf("Axis[%d]: %d\n", e.number, e.value);
            }

            //printf("%u, %d, %u, %u\n", e.time, e.value, e.type, e.number);
		}

        usleep(100000);

    }

    return 0;
}