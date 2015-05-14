// Demonstrating the use of ranges built using istream_iterator
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>

template<typename T>
void separated_printer(std::string const& s) {
    typedef std::istream_iterator<T> iter_t;
    std::istringstream ss(s);
    for (auto it = iter_t(ss); it != iter_t(); ++it) {
        std::cout << "|" << *it << "| ";
    }
    std::cout << "\n";
}
    

int main() {
    std::string test("123 ABC");
    separated_printer<char>(test);        // |1| |2| |3| |A| |B| |C|
    separated_printer<std::string>(test); // |123| |ABC|
    separated_printer<int>(test);         // |123|
    
}
