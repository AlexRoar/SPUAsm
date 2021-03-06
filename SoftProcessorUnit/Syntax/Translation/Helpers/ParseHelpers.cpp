//
//  ParseHelpers.cpp
//  SoftProcessorUnit
//
//  Created by Александр Дремов on 22.10.2020.
//

#include "ParseHelpers.hpp"
#include "CommandsParser.hpp"
#include "SPUDtypes.hpp"
#include "ComplexValueTools.hpp"
#include <cstring>

double getDoubleFromBuffer(char* start) {
    double value = 0;
    memcpy(&value, start, sizeof(double));
    return value;
}

int getIntFromBuffer(char* start) {
    int value = 0;
    memcpy(&value, start, sizeof(int));
    return value;
}
