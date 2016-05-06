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

#ifndef DRIVERS_USART_H_
#define DRIVERS_USART_H_

#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>

#define UBRR_VALUE(baud) ((F_CPU/(16*baud))-1)

#define PARITY_DISABLED ((0<<UPM01) + (0<<UPM00))
#define PARITY_EVEN     ((1<<UPM01) + (0<<UPM00))
#define PARTIY_ODD      ((1<<UPM01) + (1<<UPM00))

#define STOP_1BIT       (0<<USBS0)
#define STOP_2BIT       (1<<USBS0)

#define CHAR_5BIT       ((0<<UCSZ02)|(0<<UCSZ01)|(0<<UCSZ00))
#define CHAR_6BIT       ((0<<UCSZ02)|(0<<UCSZ01)|(1<<UCSZ00))
#define CHAR_7BIT       ((0<<UCSZ02)|(1<<UCSZ01)|(0<<UCSZ00))
#define CHAR_8BIT       ((0<<UCSZ02)|(1<<UCSZ01)|(1<<UCSZ00))
#define CHAR_9BIT       ((1<<UCSZ02)|(1<<UCSZ01)|(1<<UCSZ00))

#define usart_enable_rx_int() UCSR0B |= _BV(RXCIE0)
#define usart_disable_rx_int() UCSR0B &= ~_BV(RXCIE0)

void          usart_init(unsigned long baudrate, char parity, char stopbit, char characters);
void          usart_sendbyte(char c);
unsigned char usart_receivebyte();
int           usart_putchar(char c, FILE *stream);

#endif /* DRIVERS_USART_H_ */
