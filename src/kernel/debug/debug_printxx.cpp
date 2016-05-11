#include "asm/to_uint.h"
#include "genos/debug/debug.h"
#include "genos/gstl/horrible_cast.h"
#include "utilxx/string.h"


void dprln() {debug_print("\r\n");};

template<> void dpr(char obj) { debug_putchar(obj); };
template<> void dpr(char* obj) { debug_print(obj); };
template<> void dpr(const char* obj) { debug_print(obj); };

template<> void dpr(genos::charptr obj) 
{ 
	debug_write(obj.to_buf(),obj.length()); 
};

template<> void dpr(uint8_t obj) { debug_printdec_uint8(obj); };
template<> void dpr(uint16_t obj) { debug_printdec_uint16(obj); };
template<> void dpr(uint32_t obj) { debug_printdec_uint32(obj); };
template<> void dpr(uint64_t obj) { debug_printdec_uint64(obj); };

template<> void dpr(int8_t obj) { debug_printdec_int8(obj); };
template<> void dpr(int16_t obj) { debug_printdec_int16(obj); };
template<> void dpr(int32_t obj) { debug_printdec_int32(obj); };
template<> void dpr(int64_t obj) { debug_printdec_int64(obj); };

template<> void dpr(bool obj) { debug_print_bool(obj); };

template<> void dprbin(uint8_t obj) { debug_printbin_uint8(obj);  };
template<> void dprbin(uint16_t obj) { debug_printbin_uint16(obj);  };
template<> void dprbin(uint32_t obj) { debug_printbin_uint32(obj);  };
template<> void dprbin(uint64_t obj) { debug_printbin_uint64(obj);   };

template<> void dprbin(int8_t obj) { debug_printbin_uint8(obj);   };
template<> void dprbin(int16_t obj) { debug_printbin_uint16(obj);   };
template<> void dprbin(int32_t obj) { debug_printbin_uint32(obj);   };
template<> void dprbin(int64_t obj) { debug_printbin_uint64(obj);   };


template<> void dprhex(const char* obj) { debug_print_dump_simple((void*)obj, debug_strlen(obj));   };
template<> void dprhex(char obj) { debug_printhex_uint8((uint8_t) obj); };

template<> void dprhex(uint8_t obj) { debug_printhex_uint8(obj);   };
template<> void dprhex(uint16_t obj) { debug_printhex_uint16(obj);   };
template<> void dprhex(uint32_t obj) { debug_printhex_uint32(obj);   };
template<> void dprhex(uint64_t obj) { debug_printhex_uint64(obj);   };

template<> void dprhex(int8_t obj) { debug_printhex_uint8(obj);   };
template<> void dprhex(int16_t obj) { debug_printhex_uint16(obj);   };
template<> void dprhex(int32_t obj) { debug_printhex_uint32(obj);   };
template<> void dprhex(int64_t obj) { debug_printhex_uint64(obj);   };

template<> void dprhex(float obj) { dprhex(horrible_cast<float_uint, float>(obj));   };
template<> void dprhex(double obj) { dprhex(horrible_cast<double_uint, double>(obj));   };

template<> void dpr(genos::string obj) {dpr(obj.c_str());};
