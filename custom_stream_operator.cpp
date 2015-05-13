// Demonstrating the use of ranges built using istream_iterator with a custom stream operator
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>

#include <range/v3/all.hpp>

#include "nmea.h"

int main() {
    std::string test("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,,*47");
    test +=        "\n$GPGGA,170834,4124.8963,N,08151.6838,W,1,05,1.5,280.2,M,-34.0,M,,,*59";

    using namespace ranges::v3;
    std::istringstream ss(test);
    ranges::for_each(istream_range<gga>(ss),
                     [](gga const& g) {
                         std::cout << g << "\n";
                     });
}
