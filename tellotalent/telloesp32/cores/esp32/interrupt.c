#include <interrupt.h>

portMUX_TYPE port_mux = portMUX_INITIALIZER_UNLOCKED;

void __enable_irq(){
    portEXIT_CRITICAL(&port_mux);
}
void __disable_irq(){
    portENTER_CRITICAL(&port_mux);
}