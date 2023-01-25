#include "bitTypes.h"
#include <iostream>

using std::cout;
using std::endl;

#define line_print(x)                                                          \
  { cout << #x << "\t= " << x << endl; }

int main() {
  line_print(sizeof(_bitTypes::u_char));
  line_print(sizeof(_bitTypes::u_short));
  line_print(sizeof(_bitTypes::u_int));
  line_print(sizeof(_bitTypes::u_long));
  line_print(sizeof(_bitTypes::int8_t));
  line_print(sizeof(_bitTypes::uint8_t));
  line_print(sizeof(_bitTypes::int16_t));
  line_print(sizeof(_bitTypes::uint16_t));
  line_print(sizeof(_bitTypes::int32_t));
  line_print(sizeof(_bitTypes::uint32_t));
#ifdef ENV64BIT
  line_print(sizeof(_bitTypes::int64_t));
  line_print(sizeof(_bitTypes::uint64_t));
#endif
};
