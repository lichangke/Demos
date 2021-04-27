//
// Created by leacock on 2021/4/27.
//

#ifndef VARIABLEPARAMMACRO_MACRODEMO_H
#define VARIABLEPARAMMACRO_MACRODEMO_H

#include <iostream>

#define GET_MACRO(_1, _2, _3, NAME, ...) NAME
#define PRINT(...) GET_MACRO(__VA_ARGS__,  OUTPUT3, OUTPUT2, OUTPUT1, ...) (__VA_ARGS__)
#define OUTPUT1(v1)  std::cout << v1 << std::endl;
#define OUTPUT2(v1, v2) OUTPUT1(v1) OUTPUT1(v2)
#define OUTPUT3(v1, v2, v3) OUTPUT1(v1) OUTPUT2(v2,v3)


#endif //VARIABLEPARAMMACRO_MACRODEMO_H
