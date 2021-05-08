/*
 * mmp4.c
 *
 * Created: 5/4/2021 5:43:55 PM
 * Author : KALI
 */ 
#include <avr/io.h>
#define F_CPU 1000000ul
#include <util/delay.h>

void delay_ms(int d) {
_delay_ms(1);
}
void segShow(int p1, int p2, int p3, int p4) {
	PORTK = (1<<4);
	PORTL = p1/60;
	_delay_ms(1);
	PORTK = (1<<5);
	PORTL = p2/60;
	_delay_ms(1);
	PORTK = (1<<6);
	PORTL = p3/60;
	_delay_ms(1);
	PORTK = (1<<7);
	PORTL = p4/60;
}
void soundVBuzzer(){
	PORTK |= (1<<0);

}
void stopvBuzzer(){
	PORTK &= (0<<0);
}
int main(void)
{
	
	DDRF = 0b00000111; //keypad
    /* Replace with your application code */
	DDRK = 0xff;
	DDRL = 0xff;
	DDRB = 0xFF;
	
	//SET to 0 if paddock closed and resume when cows feeding
	// int p1_counter = 0;
	// int p2_counter = 0;
	// int p3_counter = 0;
	// int p4_counter = 0;
	
	//set door on/off 
	int d1G = 0;
	int d1W = 0;
	int d2G = 0;
	int d2W = 0;
	int d3G = 0;
	int d3W = 0;
	int d4G = 0;
	int d4W = 0;

	//check is first iteration if yes SET 1 if not set 0
	int d1Gi = 1, d1Wi = 1, d2Gi = 1, d2Wi =1, d3Gi = 1, d3Wi =1, d4Wi = 1, d4Gi =1;
	
	//set door counter
	int d1G_counter = 0;

	int d1W_counter = 0;
	int d2G_counter = 0;
	int d2W_counter = 0;
	int d3G_counter = 0;
	int d3W_counter = 0;
	int d4G_counter = 0;
	int d4W_counter = 0;
	
	//SET 0 for off and 1 for on
	int p1R_LED = 0;
	int p1G_LED = 0;
	int p2R_LED = 0;
	int p2G_LED = 0;
	int p3R_LED = 0;
	int p3G_LED = 0;
	int p4R_LED = 0;
	int p4G_LED = 0;

	//red LED counters
	int p1R_LED_counter = 0;
	int p2R_LED_counter = 0;
	int p3R_LED_counter = 0;
	int p4R_LED_counter = 0;

	//maximum red_led light duration
	int MAX_LIGHT_TIME = 60; //we remove 5 seconds to handle execution time 60 => 55

	
	//set 0 for counter off and 1 for start counter
	int p1Gr = 0;
	int p2Gr = 0;
	int p3Gr = 0;
	int p4Gr = 0;
	int p1Wr = 0;
	int p2Wr = 0;
	int p3Wr = 0;
	int p4Wr = 0;
	
	//grazing and water counters for the paddocks
	int p1Gr_counter = 0;
	int p2Gr_counter = 0;
	int p3Gr_counter = 0;
	int p4Gr_counter = 0;
	int p1Wr_counter = 0;
	int p2Wr_counter = 0;
	int p3Wr_counter = 0;
	int p4Wr_counter = 0;

	//total grazing and water counters for the paddocks
	int p1Gr_total_counter = 0;
	int p2Gr_total_counter = 0;
	int p3Gr_total_counter = 0;
	int p4Gr_total_counter = 0;
	int p1Wr_total_counter = 0;
	int p2Wr_total_counter = 0;
	int p3Wr_total_counter = 0;
	int p4Wr_total_counter = 0;
	
	//set maximum grazing and drinking time
	int p1_max_grazing = 240;  //default grazing of 4 min
	int p2_max_grazing = 240;  //default grazing of 4 min
	int p3_max_grazing = 240;  //default grazing of 4 min
	int p4_max_grazing = 240;  //default grazing of 4 min
	int MAX_DRINKING = 120;  //default water time of 2 min
	
	//SETUP timer for check every second
	int timerOverflowCount = 0;
	TCNT0 = 0x00;
	TCCR0B = (1<<CS10);

    while (1)
    {
		
		//display 7 segment display
		segShow(p1Gr_total_counter, p2Gr_total_counter,p3Gr_total_counter,p4Gr_total_counter);
		
		PORTF = 0b11111110;
		if( (PINF & (1<<3)) ==0 ) {
			d3G = 1;
			p3R_LED = 1;
			soundVBuzzer();
		}
		
		PORTF = 0b11111101;
		if( (PINF & (1<<3)) ==0 ) {
			d2G = 1;
			p2R_LED = 1;
			soundVBuzzer();
		}
		else if((PINF & (1<<6)) == 0){
			d2G = 1, d3G =1, d1G =1, d4G =1;
			p2R_LED = 1, p3R_LED = 1, p1R_LED = 1, p4R_LED = 1; 
			soundVBuzzer();
		}
	
		PORTF = 0b11111011;
		if( (PINF & (1<<3)) ==0 ) {
			d1G = 1;
			p1R_LED = 1; 
			soundVBuzzer();
			
		}
		else if( (PINF & (1<<4)) ==0 ) {
			d4G = 1;
			p4R_LED = 1;
			soundVBuzzer();
		}
		while((TIFR0 & 0x01) == 0);
		TCNT0 = 0x00;
		TIFR0 = 0x01;
		timerOverflowCount++;
		if(timerOverflowCount >= 3921){
			timerOverflowCount = 0;
			//CONTROLL EVERY SECOND DELAY
			stopvBuzzer(); //STOP BUZZER

			//check if red light is lighting to increment counter
			if(p1R_LED){
				PORTB ^= (1 << PB4);
				p1R_LED_counter++;
				if(p1R_LED_counter == MAX_LIGHT_TIME){
					p1R_LED_counter = 0;
					p1R_LED = 0; //set red light off
					PORTB &= (0<<4);

				}
			}
			if(p2R_LED){
				PORTB ^= (1 << PB5);
				p2R_LED_counter++;
				if(p2R_LED_counter == MAX_LIGHT_TIME){
					p2R_LED_counter = 0;
					p2R_LED = 0; //set red light off
					PORTB &= (0<<5);

				}
			}
			if(p3R_LED){
				PORTB ^= (1 << PB6);
				p3R_LED_counter++;
				if(p3R_LED_counter == MAX_LIGHT_TIME){
					p3R_LED_counter = 0;
					p3R_LED = 0; //set red light off
					PORTB &= (0<<6);
				}
			}
			if(p4R_LED){
				PORTB ^= (1 << PB7);
				p4R_LED_counter++;
				if(p4R_LED_counter == MAX_LIGHT_TIME){
					p4R_LED_counter = 0;
					p4R_LED = 0; //set red light off
					PORTB &= (0<<7);

				}
			}
			
			//check if grazing or water counters are set to start counting
			if(p1Gr){
				if(!p1G_LED)
					p1G_LED = 1; //set green led back on
					PORTB |= (1<<0);
				p1Gr_counter++;
				p1Gr_total_counter++;
				if(p1Gr_counter == p1_max_grazing){
					p1Gr_counter = 0;
					p1Gr = 0;
					if(p1_max_grazing == 180){
						d1G = 1;
					}else{
						d1W = 1; //open water door
						p1_max_grazing = 240; //reset to default time
					}
					p1R_LED = 1; 
					soundVBuzzer();
				}
			}
			if(p2Gr){
				if(!p2G_LED)
					p2G_LED = 1; //set green led back on
					PORTB |= (1<<1);
				p2Gr_counter++;
				p2Gr_total_counter++;
				if(p2Gr_counter == p2_max_grazing){
					p2Gr_counter = 0;
					p2Gr = 0;
					if(p2_max_grazing == 180){
						d2G = 1;
					}else{
						d2W = 1; //open water door
						p2_max_grazing = 240; //reset to default time
					}
					p2R_LED = 1; 
					soundVBuzzer();
				}
			}
			if(p3Gr){
				if(!p3G_LED)
					p3G_LED = 1; //set green led back on
					PORTB |= (1<<2);
				p3Gr_counter++;
				p3Gr_total_counter++;
				if(p3Gr_counter == p3_max_grazing){
					p3Gr_counter = 0;
					p3Gr = 0;
					if(p3_max_grazing == 180){
						d3G = 1;
					}else{
						d3W = 1; //open water door
						p3_max_grazing = 240; //reset to default time
					}
					p3R_LED = 1; 
					soundVBuzzer();
				}
			}
			if(p4Gr){
				if(!p4G_LED)
					p4G_LED = 1; //set green led back on
					PORTB |= (1<<3);
				p4Gr_counter++;
				p4Gr_total_counter++;
				if(p4Gr_counter == p4_max_grazing){
					p4Gr_counter = 0;
					p4Gr = 0;
					if(p4_max_grazing == 180){
						d4G = 1;
					}else{
						d4W = 1; //open water door
						p4_max_grazing = 240; //reset to default time
					}
					p4R_LED = 1; 
					soundVBuzzer();
				}
			}
			if(p1Wr){
				p1Wr_counter++;
				if(p1Wr_counter == MAX_DRINKING){
					p1Wr_total_counter += p1Wr_counter;
					p1Wr_counter = 0;
					p1Wr = 0;
					d1W = 1; //open WATER door
					p1_max_grazing = 180; //set grazing time to 3 min
					p1R_LED = 1; 
					soundVBuzzer();
				}
			}
			if(p2Wr){
				p2Wr_counter++;
				if(p2Wr_counter == MAX_DRINKING){
					p2Wr_total_counter += p2Wr_counter;
					p2Wr_counter = 0;
					p2Wr = 0;
					d2W = 1; //open WATER door
					p2_max_grazing = 180; //set grazing time to 3 min
					p2R_LED = 1; 
					soundVBuzzer();
				}
			}
			if(p3Wr){
				p3Wr_counter++;
				if(p3Wr_counter == MAX_DRINKING){
					p3Wr_total_counter += p3Wr_counter;
					p3Wr_counter = 0;
					p3Wr = 0;
					d3W = 1; //open WATER door
					p3_max_grazing = 180; //set grazing time to 3 min
					p3R_LED = 1; 
					soundVBuzzer();
				}
			}
			if(p4Wr){
				p4Wr_counter++;
				if(p4Wr_counter == MAX_DRINKING){
					p4Wr_total_counter += p4Wr_counter;
					p4Wr_counter = 0;
					p4Wr = 0;
					d4W = 1; //open WATER door
					p4_max_grazing = 180; //set grazing time to 3 min
					p4R_LED = 1; 
					soundVBuzzer();
				}
			}
			
			
			//check if door is opened
			if(d1G){
				d1G_counter++;
				if(d1G_counter == 40){
					d1G_counter = 0;
					d1G = 0;
					d1Gi = 1;
				}else if(d1G_counter == 4){
					if(d1Gi && (p1Gr_total_counter == 0)){
						d1Gi = 0;
						//set grazing counter
						p1Gr = 1;
					}
					else{
						//stop grazing timer
						p1Gr = 0;
						p1G_LED = 0; //turn off green light
						PORTB &= (0<<0);
					}
				}
		
			}
			if(d2G){
				d2G_counter++;
				if(d2G_counter == 40){
					d2G_counter = 0;
					d2G = 0;
					d2Gi = 1;
				}else if(d2G_counter == 4){
					if(d2Gi && (p2Gr_total_counter == 0)){
						d2Gi = 0;
						//set grazing counter
						p2Gr = 1;
					}
					else{
						//stop grazing timer
						p2Gr = 0;
						p2G_LED = 0; //turn off green light
						PORTB &= (0<<1);
					}
				}
			}
			if(d3G){
				d3G_counter++;
				if(d3G_counter == 40){
					d3G_counter = 0;
					d3G = 0;
					d3Gi = 1;
				}else if(d3G_counter == 4){
					if(d3Gi && (p3Gr_total_counter == 0)){
						d3Gi = 0;
						//set grazing counter
						p3Gr = 1;
					}
					else{
						//stop grazing timer
						p3Gr = 0;
						p3G_LED = 0; //turn off green light
						PORTB &= (0<<2);
					}
				}
			}
			if(d4G){
				d4G_counter++;
				if(d4G_counter == 40){
					d4G_counter = 0;
					d4G = 0;
					d4Gi = 1;
				}else if(d4G_counter == 4){
					if(d4Gi && (p4Gr_total_counter == 0)){
						d4Gi = 0;
						//set grazing counter
						p4Gr = 1;
					}
					else{
						//stop grazing timer
						p4Gr = 0;
						p4G_LED = 0; //turn off green light
						PORTB &= (0<<3);
					}
				}
			}
			if(d1W){
				d1W_counter++;
				if(d1W_counter == 40){
					d1W_counter = 0;
					d1W = 0;
					d1Wi = 1;
				}else if(d1W_counter == 4){
					if(d1Wi && (p1Wr_total_counter == 0)){
						d1Wi = 0;
						//start water counter
						p1Wr = 1;
					}else{
						//stop water timer and start grazing
						p1Gr = 1;
						p1Wr = 0;
					}
				}
			}
			if(d2W){
				d2W_counter++;
				if(d2W_counter == 40){
					d2W_counter = 0;
					d2W = 0;
					d2Wi = 1;
				}else if(d2W_counter == 4){
					if(d2Wi && (p2Wr_total_counter == 0)){
						d2Wi = 0;
						//start water counter
						p2Wr = 1;
					}else{
						//stop water timer and start grazing
						p2Gr = 1;
						p2Wr = 0;
					}
				}
			}
			if(d3W){
				d3W_counter++;
				if(d3W_counter == 40){
					d3W_counter = 0;
					d3W = 0;
					d3Wi = 1;
				}else if(d3W_counter == 4){
					if(d3Wi && (p3Wr_total_counter == 0)){
						d3Wi = 0;
						//start water counter
						p3Wr = 1;
					}else{
						//stop water timer and start grazing
						p3Gr = 1;
						p3Wr = 0;
					}
				}
			}
			if(d4W){
				d4W_counter++;
				if(d4W_counter == 40){
					d4W_counter = 0;
					d4W = 0;
					d4Wi = 1;
				}else if(d4W_counter == 4){
					if(d4Wi && (p4Wr_total_counter == 0)){
						d4Wi = 0;
						//start water counter
						p4Wr = 1;
					}else{
						//stop water timer and start grazing
						p4Gr = 1;
						p4Wr = 0;
					}
				}
			}
		}
    }
}

