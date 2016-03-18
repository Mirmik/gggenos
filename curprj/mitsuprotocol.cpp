#include "mitsuprotocol.h"
#include "util/ascii_codes.h"

Driver driver;
Updater updater;

MR_4J_A servo0('2');


void lo_byte_to_symbol(uint8_t num, char* symb)
{
	uint8_t l = num % 0x10;
	uint8_t h = num >> 4; 
	*(symb + 1) = byte_to_hexascii(l);
	*(symb)     = byte_to_hexascii(h);
};

void checksum_declare(char* message_buf, char term)
{
	char* ptr = message_buf;
	int sum = 0;
	assert(*ptr == 1);
	do {
		ptr++;
		sum += *ptr;
	} while(*ptr != 3);
	lo_byte_to_symbol(sum, ++ptr);
};

void driver_info32_element::update_start(char stantion_no)
{
	char mes[20];
	char* ptr = mes; 
	*ptr++ = 1;
	*ptr++ = stantion_no;
	*ptr++ = command[0];
	*ptr++ = command[1];
	*ptr++ = 2;
	*ptr++ = datano[0];
	*ptr++ = datano[1];
	*ptr++ = 3;
	checksum_declare(mes, 3);
	//stdout.print("n");
	SerialHD1.configure_session(mes,10,3);
	SerialHD1.start_session();
};

int driver_info32_element::update_end()
{
	//stdout.println('e');
	//stdout.printhex(SerialHD1.flag);
	//stdout.println();
	stdout.println(SerialHD1.answer);
	if (SerialHD1.flag == 0xFF) 
		{
			stdout.println("errhereFF");
			return -1;
		};
	
	if (SerialHD1.flag == 0xFE) 
		{
			stdout.println("errhereFE");
			return -1;
		};
	
	if (SerialHD1.flag == 0xFD) 
		{
			stdout.println("errhereFD");
			return -1;
		};
	

	char code_error = SerialHD1.answer[1];
	if (code_error != 'A')
		{
			stdout.println("errherenA");
			stdout.print(SerialHD1.answer);
			 return -2;
		};

			//stdout.println("here3");
	data = parse(SerialHD1.answer + 2);
	//stdout.println(SerialHD1.answer);
//	stdout.println(data);
};


void driver_info32_element::wait_operation()
{
	//stdout.println("HeRe");
	wait_autom(&SerialHD1.flag);
};

void Driver::exec()
{
char error_code;
		switch (state)
		{
			case 0:
				state = 1;
				mes[0] = 1;
				mes[4] = 2;
				mes[7] = 3;
				break;
			case 1:
				checksum_declare(mes, 3);
				stdout.write(mes, 10);
				SerialHD1.configure_session(mes , 10, 3);
				SerialHD1.start_session();
				//stdout.print("fasdfasf");
				wait_autom(&rs485flag);
				state = 2;
				break;
			case 2:
				stdout.printhexln(rs485flag);
				stdout.printhexln(SerialHD1.answer_len);
				stdout.write(SerialHD1.answer, SerialHD1.answer_len);
				
				error_code = SerialHD1.answer[2];
				stdout.print("\n\rError code:");
				stdout.print(error_code);
				stdout.println();
				

				stdout.println();
				state = 3;
				exit_autom();
				break;
			case 3:
				stdout.print("!!!!!!");
				break;
		};
};



int32_t parse_int32_bias7(char* c)
{
	int32_t ret = 0;
	uint8_t* ptr = (uint8_t*) &ret;
	*ptr++ = (hexascii_to_byte(c[6]) << 4) | (hexascii_to_byte(c[7]));
	*ptr++ = (hexascii_to_byte(c[4]) << 4) | (hexascii_to_byte(c[5]));
	*ptr++ = (hexascii_to_byte(c[2]) << 4) | (hexascii_to_byte(c[3]));
	*ptr++ = (hexascii_to_byte(c[0]) << 4) | (hexascii_to_byte(c[1]));
	
//	stdout.print(c); stdout.println(ret);

	return ret;
};



extern uint8_t trap_flag;
extern uint8_t trap_counter;
extern int32_t trap[4];
 

void Updater::exec()
{
	switch (state)
	{
		case 0:
			state = 1;
			//stdout.print("d");
			break;
		case 1:
			servo0.ABS.update_start(servo0.stantion_num);
			servo0.ABS.wait_operation();
			state = 2;
			break;
		case 2:
			servo0.ABS.update_end();
			if (trap_flag == 1 && trap_counter == 0) 
				{trap[0] = servo0.ABS.data; trap_counter++;};
			if (trap_flag == 1 && trap_counter == 2) 
				{trap[2] = servo0.ABS.data; trap_counter++;};
			state = 3;
			//msleep_autom(1);
			break;
		case 3:
			servo0.CYC.update_start(servo0.stantion_num);
			servo0.CYC.wait_operation();
			state=4;
			break;
		case 4:
		//stdout.println(servo0.CYC.data);
			servo0.CYC.update_end();
			if (trap_flag == 1 && trap_counter == 1) 
				{trap[1] = servo0.CYC.data; trap_counter++;};
			if (trap_flag == 1 && trap_counter == 3) 
				{trap[3] = servo0.CYC.data; trap_flag = 0;};
			state = 5;
			//msleep_autom(1);
			break;
		case 5:
			servo0.ERR.update_start(servo0.stantion_num);
			servo0.ERR.wait_operation();
			state=6;
			break;
		case 6:
		//stdout.println(servo0.CYC.data);
			servo0.ERR.update_end();
			state = 1;
			msleep_autom(1);
			break;
	
	};
};
