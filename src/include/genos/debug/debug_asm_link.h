#ifndef _Mirm_Assembler_     //Защита от 
#define _Mirm_Assembler_     //двойного подключения 

//Общие определения 

#ifdef __ASSEMBLER__

#endif    //#ifdef __ASSEMBLER__

#ifndef __ASSEMBLER__

//Определения не для ассемблера.

#ifdef __cplusplus   
extern "C"{             //extern "C" декларирует, что  
                        //передача параметров 
                        //в функцию и возвращение результата
                        //должны вестись по соглашениям
                        //языка Си, а не C++.
#endif    //#ifdef __cplusplus

 void debug_asmlink_test();
 void debug_asmlink_uint16_dpr(uint16_t a);
 void debug_asmlink_uint16_dpr2(uint16_t a, uint16_t b);
 void debug_asmlink_uint16_dpr4(uint16_t a, uint16_t b, uint16_t c, uint16_t d);
 uint16_t debug_asmlink_uint16_ret();
 uint64_t debug_asmlink_uint64_ret();


#ifdef __cplusplus
} // extern "C"
#endif    //#ifdef __cplusplus
        
#endif    //#if not def __ASSEMBLER__

#endif    //#ifndef _Mirm_Assembler_
