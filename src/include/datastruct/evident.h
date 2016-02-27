#pragma once
#ifndef GENOS_EVIDENT
	#define GENOS_EVIDENT
	
	#include "genos/types.h"
	#include "genos/gstl/hassign.h"
	#include "genos/defs.h"
	#include "gstd/string.h"
	//#include "gstd/typename.h"
	#include "genos/base_struct/evident_table.h"
	#include "genos/memory.h"
	//#include "genos/debug/debug.h"
		#include "genos/base_struct/list.h"
	
	
	#ifdef PROCESS_MODE
		#include "genos/schedproc/process_base.h"
		#include "genos/schedproc/scheduler_base.h"
		#include "genos/syscontext/syscontext.h"
	#endif 
	
	#ifdef DELEGATE_MODE
		#include "genos/sigslot/delegate2.h"
		#include "genos/sigslot/delegate.h"
	#endif 
	
	namespace evd{
		static constexpr uint8_t UNKNOW				=0x00;
		static constexpr uint8_t INT8				=0x01;
		static constexpr uint8_t INT16				=0x02;
		static constexpr uint8_t INT32				=0x03;
		static constexpr uint8_t INT64				=0x04;
		static constexpr uint8_t UINT8				=0x05;
		static constexpr uint8_t UINT16				=0x06;
		static constexpr uint8_t UINT32				=0x07;
		static constexpr uint8_t UINT64				=0x08;
		static constexpr uint8_t CHAR				=0x09;
		static constexpr uint8_t CHAR_PTR			=0x0A;
		static constexpr uint8_t CONST_CHAR_PTR		=0x0B;
		static constexpr uint8_t PROCESS_PTR			=0x0C; 
		static constexpr uint8_t SCHEDULER_PTR		=0x0D; 
		static constexpr uint8_t RTDELEGATE			=0x0E; 
		static constexpr uint8_t CDELEGATE			=0x0F; 
		static constexpr uint8_t THREAD_PTR			=0x10; 
		static constexpr uint8_t STATIC_STR_PTR			=0x11;
		static constexpr uint8_t STATIC_REVERSE_STR_PTR	=0x12;
		static constexpr uint8_t STATIC_STR			=0x13;
		static constexpr uint8_t STATIC_REVERSE_STR	=0x14;
		static constexpr uint8_t UNSIGNED_CHAR_PTR	=0x15;
		static constexpr uint8_t LIST_HEAD			=0x16;
		static constexpr uint8_t LIST_HEAD_PTR		=0x17;
		static constexpr uint8_t VOID_PTR		=0x18;
		
		
		
		static constexpr uint8_t TYPE	=0x7F;
		static constexpr uint8_t PACKED	=0x80;
	};
	
	class evident{
		public:
		uintmax_t data;
		uint8_t possition;
		
		
		
		template<typename T>
		uint8_t get_possition(T obj);//{return evd::UNKNOW;};
			
			
			#define evident_get_possition(type, cnt) uint8_t get_possition(type v) {return evd::cnt;}
			
				template<size_t N> uint8_t get_possition(gstd::static_string<N>* obj) {return evd::STATIC_STR_PTR;}
				template<size_t N> uint8_t get_possition(gstd::static_revstring<N>* obj) {return evd::STATIC_REVERSE_STR_PTR;}
				template<size_t N> uint8_t get_possition(gstd::static_string<N> obj) {return evd::STATIC_STR;}
				template<size_t N> uint8_t get_possition(gstd::static_revstring<N> obj) {return evd::STATIC_REVERSE_STR;}
			
			evident_get_possition(int8_t, INT8);
			evident_get_possition(int16_t, INT16);
			evident_get_possition(int32_t, INT32);
			evident_get_possition(int64_t, INT64);
			evident_get_possition(uint8_t, UINT8);
			evident_get_possition(uint16_t, UINT16);
			evident_get_possition(uint32_t, UINT32);
			evident_get_possition(uint64_t, UINT64);
			evident_get_possition(char, CHAR);
			evident_get_possition(char*, CHAR_PTR);
			evident_get_possition(void*, VOID_PTR);
			evident_get_possition(unsigned char*, UNSIGNED_CHAR_PTR);
			evident_get_possition(const char*, CONST_CHAR_PTR);
			evident_get_possition(thread*, THREAD_PTR);
			#ifdef PROCESS_MODE
				evident_get_possition(process_base*, PROCESS_PTR);
				evident_get_possition(scheduler_base*, SCHEDULER_PTR);
			#endif
			#ifdef DELEGATE_MODE_MODE
				evident_get_possition(rtdelegate , RTDELEGATE);
				evident_get_possition(cdelegate, CDELEGATE);
			#endif*/
			
			evident_get_possition(list_head, LIST_HEAD);
			evident_get_possition(list_head*, LIST_HEAD_PTR);
				
			
			template<typename T> evident(T var)
			{
				using vartype = typename std::remove_cv<T>::type;
				possition = get_possition(vartype());
				if (sizeof(uintmax_t) >= sizeof(vartype))
				hassign(data, var);
				else 
				{
					htreat<T*>(data) = genos::sys_allocate<T>(1);
					hassign(*htreat<T*>(data), var);
					bits_set(possition, evd::PACKED);
				};
			};
			
			template<typename T>
			T& get_ref() {
				if (bits_mask(possition, evd::PACKED))
				return *htreat<T*>(data);	
				else
			return htreat<T>(data);}; 
			
			
			const char* type(){
				return evident_table.rec[bits_mask(possition,evd::TYPE)].type;
			};
			
			~evident() {if (bits_mask(possition, evd::PACKED)) delete htreat<void*>(data);};
			
			
			gstd::static_revstring<21> to_string(){
				return evident_table.rec[bits_mask(possition,evd::TYPE)].to_string((void*)data);
				};
			
			
		};
		
		
		
		
	#endif
	
