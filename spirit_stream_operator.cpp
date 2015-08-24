// Demonstrating the use of ranges built using a qi::spirit parser adapted to be a stream operator
#include <string>
#include <iostream>
#include <sstream>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_match.hpp>

#include <range/v3/all.hpp>

#include "nmea.h"

// supply our own stream operators here that match the declarations
std::istream&
operator>>(std::istream& is, gga_t& g) {
    using namespace boost::spirit;
    using namespace boost::spirit::qi;

    rule<istream_iterator, gga_t()> gga_parser =
        lit("$GPGGA") > ',' > int_ > ',' >
        double_ > ',' > char_("NS") > ',' >
        double_ > ',' > char_("EW") > ',' >
        int_ > ',' > int_ > ',' >
        double_ > ',' >
        double_ > ',' > char_ > ',' >
        double_ > ',' > char_ > ',' >
        -int_ > ',' > -int_ > ",*" > int_;
        
    // construct a stream manipulator from this parser
    auto manip = match(gga_parser, g);
    is >> manip;

    return is;
}

std::ostream& operator<<(std::ostream& os, gga_t const& g) {
    os << '(' << g.latitude << g.lat_hemi << ',' << g.longitude << g.long_hemi << ')';
    return os;
}

int main() {
    std::string test("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,,*47");
    test +=        "\n$GPGGA,170834,4124.8963,N,08151.6838,W,1,05,1.5,280.2,M,-34.0,M,,,*59";

    std::istringstream ss(test);
    ss.unsetf(std::ios::skipws);
    using namespace ranges::v3;
    ranges::for_each(istream_range<gga_t>(ss),
                     [](gga_t const& g) {
                         std::cout << g << "\n";
                     });

}
