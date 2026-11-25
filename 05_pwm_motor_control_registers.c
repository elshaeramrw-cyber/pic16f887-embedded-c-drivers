void init_ports(void) {
    // Disable analog on PORTB (RB0-RB5 are analog capable)
    ANSELH = 0x00;

    // Set RB0, RB1, RB3 as digital outputs
    TRISB.RB0 = 0;  // 1A
    TRISB.RB1 = 0;  // 2A
    TRISB.RB3 = 0;  // CCP2 PWM output (1,2EN)
    // Set unused RB2, RB4-RB7 as outputs and drive low (optional)
    TRISB.RB2 = 0;
    TRISB.RB4 = 0;
    TRISB.RB5 = 0;
    TRISB.RB6 = 0;
    TRISB.RB7 = 0;
    PORTB = 0x00;    // all outputs initially low

    // Set RC4-RC7 as digital inputs (pull-downs external, no internal pull-ups)
    TRISC.RC4 = 1;
    TRISC.RC5 = 1;
    TRISC.RC6 = 1;
    TRISC.RC7 = 1;
}

void init_pwm(void) {
    // Configure CCP2 as PWM mode, single output
    CCP2CON = 0x0C;          // P2M<1:0>=00 (single), CCP2M<3:0>=1100 (PWM)

    // Set Timer2 for PWM period = 1 kHz at 8 MHz
    // PWM period = (PR2+1) * 4 * Tosc * TMR2_prescaler
    // Prescaler 16, PR2 = 124 -> (125)*4*125ns*16 = 1 ms (1 kHz)
    T2CON = 0x02;            // Timer2 ON, prescaler 1:16
    PR2 = 124;               // Period register

    // Initial duty cycle = 0
    CCPR2L = 0;
    CCP2CONbits.DC2B0 = 0;   // Lower 2 bits of 10-bit duty
    CCP2CONbits.DC2B1 = 0;
}

void set_pwm_duty(unsigned int duty10) {
    // duty10: 0-1023
    if(duty10 > 1023) duty10 = 1023;
    CCPR2L = (duty10 >> 2) & 0xFF;        // Upper 8 bits
    // Lower 2 bits are in CCP2CON<5:4> (DC2B1:DC2B0)
    CCP2CONbits.DC2B1 = (duty10 >> 1) & 0x01;
    CCP2CONbits.DC2B0 = duty10 & 0x01;
}

void set_direction(unsigned char dir) {
    direction = dir;
    switch(dir) {
        case CW:
            RB0 = 1;
            RB1 = 0;
            break;
        case CCW:
            RB0 = 0;
            RB1 = 1;
            break;
        case STOP:
        default:
            RB0 = 0;
            RB1 = 0;
            break;
    }
}

/*
 * Debounced read of the 4 pushbuttons on RC7-RC4.
 * Buttons are active high (pulled low externally, go high when pressed).
 * Returns a byte where bit 0 = RC7, bit1 = RC6, bit2 = RC5, bit3 = RC4
 * (mask 0x0F with 1 = pressed).
 */
unsigned char read_buttons_debounced(void) {
    static unsigned char last_state = 0;
    unsigned char raw = (PORTC >> 4) & 0x0F;   // bits 7..4 ? nibble 0..3

    if(raw == 0x00) {          // no button pressed
        last_state = 0;
        return 0;
    }

    if(raw != last_state) {
        __delay_ms(20);        // debounce delay
        raw = (PORTC >> 4) & 0x0F;
        if(raw == last_state) {
            // Still the same, consider it stable
            return raw;        // 1 in positions of pressed buttons
        }
        last_state = raw;
    }
    return 0;   // not stable or no press
}

void main(void) {
    init_ports();
    init_pwm();
    set_direction(STOP);
    set_pwm_duty(0);

    unsigned int duty = 0;                  // current duty cycle (0-1023)
    const unsigned int step = 10;           // speed step size
    const unsigned int max_duty = 1023;

    while(1) {
        unsigned char btns = read_buttons_debounced();

        // Direction control (bit mapping: btns.0 = RC7 = CW, btns.1 = RC6 = CCW)
        if(btns & 0x01) {       // RC7: CW
            set_direction(CW);
        }
        if(btns & 0x02) {       // RC6: CCW
            set_direction(CCW);
        }
        if((btns & 0x03) == 0x03) {   // Both direction buttons -> STOP
            set_direction(STOP);
        }

        // Speed control
        if(btns & 0x04) {       // RC5: Speed Up
            if(duty < max_duty - step) {
                duty += step;
            } else {
                duty = max_duty;
            }
            set_pwm_duty(duty);
        }
        if(btns & 0x08) {       // RC4: Speed Down
            if(duty >= step) {
                duty -= step;
            } else {
                duty = 0;
            }
            set_pwm_duty(duty);
        }

        __delay_ms(50);   // small loop delay to prevent super-fast key repeats
    }
}