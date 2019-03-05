#pragma once

#include <vector>
#include <iostream>

class GNUPlot final
{
public:
    static void draw(double duration, std::vector<double> array);

private:
    static void saveArray(double duration, std::vector<double> array);
    static void openAndDraw();
};
