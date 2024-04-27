# Arduino-Operations
The aim of this project is to gain familiarity with programming a microcontroller, its timers and analog to digital converter using interrupts.

Write a single interrupt-driven program to do the following tasks currently:
1.	Use PORTD of an ATMega328P microcontroller to implement a travelling single LED pattern (cylon eyes) on the LED shield provided with your Arduino board. The LED array is connected to the microcontroller’s PORTD output. The on/off time of the LED must be set using a timer interrupt, where the on/off time is based on the input voltage applied to ADC input 0 of the microcontroller. You must not use a delay loop for on/off timing.
2.	Use PORTD to display an output on the LED array to provide a graphical display of the input voltage applied to the ADC input 0 of the microcontroller in ‘thermometer’ format.
3.	Read the input switches attached to PORTB bits 5 and 4, and use the values read to select between the use of PORTD for a cylon eyes type display or ADC value display or a combined display using 4 bits for cylon eyes and 4 bits for ADC indication.
