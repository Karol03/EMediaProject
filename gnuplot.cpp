#include "gnuplot.hpp"
#include <cstdio>
#include <iostream>
#include <fstream>

namespace detail
{
const char* getFileName()
{
    static int i = 0;
    std::string fileName("array");
    if (i!=0)
        fileName += static_cast<char>(i);
    fileName += ".asc";
    ++i;
    return fileName.c_str();
}
}   // detail

void GNUPlot::draw(double duration, std::vector<double> array)
{
    saveArray(duration, array);
    openAndDraw();
}

void GNUPlot::openAndDraw()
{
    if (FILE* pipe = _popen("C:/gnuplot/bin/wgnuplot.exe", "w"))
    {
        std::cout << "Read data from file" << std::endl;
        fprintf(pipe, "plot array.asc’ with lines\n");
        fflush(pipe);
        std::cout << "Close pipe" << std::endl;
        _pclose(pipe);
    }
    else
        std::cerr << "Could not open the file\n";
}

void GNUPlot::saveArray(double duration, std::vector<double> array)
{
    std::ofstream file(detail::getFileName());
    for (size_t i=0; i<array.size(); i++)
        file << static_cast<double>(i)*duration << " " << array[i] << std::endl;
    file.close();
}
