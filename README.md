# Fancy Light Switch Using PWM and a Keypad

The program scans for a keypad button press every 3ms and then controls the brightness of the LED using a PWM signal depending on which button was pressed. 
When a keypad button press is detected, depending on which button is pressed, the program updates the timer to change the duty cycle for
the desired brightness level. The PWM signal has a fixed period of 2ms and is always enabled with only the duty cycle changing depending on the pressed button.
