// Demonstrating the use of ranges built using istream_iterator
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>

// use at least gcc 4.9 or compile errors will result:
#include <range/v3/all.hpp>

template<typename T>
void separated_printer(std::string const& s) {
    using namespace ranges::v3;
    std::istringstream ss(s);
    ranges::for_each(istream_range<T>(ss),
                     [](T const& v) {
                         std::cout << "|" << v << "| ";
                     });
    std::cout << "\n";
}
    

int main() {
    std::string test("123 ABC");
    separated_printer<char>(test);
    separated_printer<std::string>(test);
    separated_printer<int>(test);
    
}
