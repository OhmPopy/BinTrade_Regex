#pragma once

#include <cstdint>

//https://referencesource.microsoft.com/#mscorlib/system/char.cs,73e61bf2c5d6ecd4
//https://github.com/dotnetGame/natsu-clr/blob/1334aa1b8ff4af2819750417c54f76fe4784e6f8/src/Native/natsu.unicode.cpp
//https://github.com/dotnetGame/natsu-clr/tree/1334aa1b8ff4af2819750417c54f76fe4784e6f8


uint8_t _s_InternalGetCategoryValue(int32_t ch, int32_t offset);

uint16_t _s_InternalGetDigitValues(int32_t ch);

double _s_InternalGetNumericValue(int32_t ch);
