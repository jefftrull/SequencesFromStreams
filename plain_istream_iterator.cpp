// Demonstrating the use of ranges built using istream_iterator
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>

template<typename T> using ist_iter = std::istream_iterator<T>;

template<typename T>
void separated_printer(std::string const& s) {
    std::istringstream ss(s);
    for (auto it = ist_iter<T>(ss); it != ist_iter<T>(); ++it) {
        std::cout << "|" << *it << "|\n";
    }
}
    

int main() {
    std::string test("123 ABC");
    separated_printer<char>(test);
    separated_printer<std::string>(test);
    separated_printer<int>(test);
    
}
