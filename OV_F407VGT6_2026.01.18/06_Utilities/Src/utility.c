#include "utility.h"
#include "math.h"

float ADC_To_Voltage(uint16_t adc_value) { return (adc_value / 4095.0f) * 3.3f; }
