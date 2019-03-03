#pragma once

#include <vector>
#include <iostream>

class GNUPlot final
{
public:
    static void draw(double duration, double* array, long size);
};
