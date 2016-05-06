/*
 * (C) Copyright 2016 I/O Blocks Toolkit (https://iobtoolkit.com/) and others.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contributors:
 *     I/O Blocks Toolkit Team
 */

#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <util/delay.h>

#include <drivers/led.h>
#include <drivers/usart.h>

static FILE __stderr = FDEV_SETUP_STREAM(usart_putchar, NULL, _FDEV_SETUP_WRITE);
static FILE __stdout = FDEV_SETUP_STREAM(usart_putchar, NULL, _FDEV_SETUP_WRITE);

int main(void) {

	/* Enable L LED */
	led_init();

	/* 	Initialize UART. */
	usart_init(57600, CHAR_8BIT, PARITY_EVEN, STOP_1BIT);
	usart_enable_rx_int();
	stdout = &__stdout;
	stderr = &__stderr;

	/* Enable interrupts */
	sei();

	while(1)
	{
		_delay_ms(500);
		led_on();
		printf_P(PSTR("Ping... "));
		_delay_ms(500);
		led_off();
		printf_P(PSTR("Pong.\n"));
	}

	return 0;
}

/*
 * Echo received characters back.
 */
ISR(USART_RX_vect) {
	char c = UDR0;
	usart_sendbyte(c);
}
