#pragma once

#include <vector>
#include <iostream>

class GNUPlot final
{
public:
    static void draw(const std::vector<double>& columnX,
                     const std::vector<double>& array);

private:
    static void saveArray(const std::vector<double>& columnX,
                          const std::vector<double>& array);
    static void openAndDraw(const std::vector<double>& columnX,
                            const std::vector<double>& array);
};
