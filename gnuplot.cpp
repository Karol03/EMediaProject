#include "gnuplot.hpp"
#include <cstdio>
#include <iostream>
#include <fstream>
#include "gnuplot_i.hpp"

namespace detail
{

std::string getFileName()
{
    static unsigned i = 0;
    std::string fileName{"array"};
    if (i!=0)
        fileName += std::to_string(i);
    fileName += ".asc";
    ++i;
    return fileName;
}

}   // namespace detail

void GNUPlot::draw(const std::vector<double>& columnX,
                   const std::vector<double>& array)
{
    saveArray(columnX, array);
    openAndDraw(columnX, array);
}

void GNUPlot::openAndDraw(const std::vector<double>& columnX,
                          const std::vector<double>& array)
{
    std::cout << __func__ << std::endl;
    // Gnuplot(columnX, array, "MyPlot", "lines");
    system("pause");
}

void GNUPlot::saveArray(const std::vector<double>& columnX,
                        const std::vector<double>& array)
{
    auto fileName = detail::getFileName().c_str();
    std::cout << __func__ << ":" << fileName << std::endl;
    std::ofstream file(fileName);
    for (size_t i=0; i<array.size(); i++)
        file << columnX[i] << " " << array[i] << std::endl;
    file.close();
}
