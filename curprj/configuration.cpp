
#include <avr/eeprom.h>
#include "configuration.h"
#include "syscontext/syscontext.h"
#include "genos/debug/debug.h"

#define CONFIGURATION_BLOCK_ADDRESS 0x5
#define VALID_MAGIC (sizeof(configuration_t) + 0x28)

configuration_t default_conf = 
{
	VALID_MAGIC,
	115200,
	usartParityDisabled,
	usartOneStopBit,
	usartEightDataBit,
	115200,
	usartParityDisabled,
	usartOneStopBit,
	usartEightDataBit
};
configuration_t conf;

void _print_conf(configuration_t& _conf)
{
	stdout.print("magic:"); stdout.printhexln(_conf.magic); 
	stdout.print("usart0speed:"); stdout.printhexln(_conf.usart0_speed);
	stdout.print("usart0parity:"); stdout.printhexln(_conf.usart0_parity);
	stdout.print("usart0stopbits:"); stdout.printhexln(_conf.usart0_stopbits);
	stdout.print("usart0databits:"); stdout.printhexln(_conf.usart0_databits);
	stdout.print("usart1speed:"); stdout.printhexln(_conf.usart1_speed);
	stdout.print("usart1parity:"); stdout.printhexln(_conf.usart1_parity);
	stdout.print("usart1stopbits:"); stdout.printhexln(_conf.usart1_stopbits);
	stdout.print("usart1databits:"); stdout.printhexln(_conf.usart1_databits);
};

void load_configuration()
{
	configuration_t temp;
	eeprom_read_block ((void*)&(temp), (void*)CONFIGURATION_BLOCK_ADDRESS, sizeof(configuration_t));
	if (temp.magic == VALID_MAGIC) 
		{
			debug_print("Configuration was successfualy load.\n\r");
			conf = temp; 
			//_print_conf(conf);
		}
	else
		{
			debug_print("Conf load error. Default configuration.\n\r");
			conf = default_conf;
			//_print_conf(conf);
		};
};

void update_configuration()
{
	conf.magic = VALID_MAGIC;
	eeprom_write_block ((void*)&(conf), (void*)CONFIGURATION_BLOCK_ADDRESS, sizeof(configuration_t));
};


void print_conf()
{
	_print_conf(conf);
};

void print_writed_conf()
{
	configuration_t temp;
	eeprom_read_block ((void*)&(temp), (void*)CONFIGURATION_BLOCK_ADDRESS, sizeof(configuration_t));
	_print_conf(temp);
};

void write_conf()
{
	update_configuration();
};