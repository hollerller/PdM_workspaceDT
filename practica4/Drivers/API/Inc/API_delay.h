/* Public function prototypes
 *
 *  */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

#include <stdint.h>		// Include library for integer data type with specific width
#include <stdbool.h>	// Include library for bool datatypes


typedef uint32_t tick_t;	// Define unsigned integer data type
typedef bool bool_t;		// Define bool data type

typedef struct{
   tick_t startTime;	// Time when the delay starts
   tick_t duration;		// Duration of the delay
   bool_t running;		// Show if the delay is running
} delay_t;				// Structure definition for delays


// Declaration of functions to be used for implementing non-blocking delays

void delayInit( delay_t *delay, tick_t duration );
bool_t delayRead( delay_t *delay );
void delayWrite( delay_t *delay, tick_t duration );


#endif /* API_INC_API_DEBOUNCE_H_ */
