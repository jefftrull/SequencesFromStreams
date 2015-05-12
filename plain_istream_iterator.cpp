// Demonstrating the use of ranges built using istream_iterator
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>

template<typename T>
void separated_printer(std::string const& s) {
    typedef std::istream_iterator<T> ist_iter;
    std::istringstream ss(s);
    for (auto it = ist_iter(ss); it != ist_iter(); ++it) {
        std::cout << "|" << *it << "| ";
    }
    std::cout << "\n";
}
    

int main() {
    std::string test("123 ABC");
    separated_printer<char>(test);
    separated_printer<std::string>(test);
    separated_printer<int>(test);
    
}
