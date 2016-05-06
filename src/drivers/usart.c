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

#include "usart.h"
#include <avr/io.h>

void usart_init(unsigned long baudrate, char parity, char stopbit, char character)
{
	UBRR0H = (unsigned char)(UBRR_VALUE(baudrate) >> 8);
	UBRR0L = (unsigned char)(UBRR_VALUE(baudrate));
	UCSR0A = 0; // U2X is enabled by the bootloader - disable it
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = parity | stopbit | character;
}


void usart_sendbyte(char c)
{
	while((UCSR0A&(1<<UDRE0)) == 0);
	UDR0 = c;
}


unsigned char usart_receivebyte()
{
	while((UCSR0A & (1<<RXC0)) == 0);
	return UDR0;
}

int usart_putchar(char c, FILE *stream)
{
	if (c == '\n')
    	usart_putchar('\r', stream);
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}
