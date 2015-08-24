// Demonstrating the use of ranges built using istream_iterator with a custom stream operator
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <cmath>

#include <boost/geometry.hpp>

#include <range/v3/all.hpp>

#include "nmea.h"

int main() {
    using namespace std;

    string test("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,,*47");
    test +=   "\n$GPGGA,170834,4124.8963,N,08151.6838,W,1,05,1.5,280.2,M,-34.0,M,,,*59";

    using namespace ranges::v3;
    istringstream ss(test);
    ranges::for_each(istream_range<gga_t>(ss),
                     [](gga_t const& g) {
                         cout << g << "\n";
                     });

    // produce a filtered range of Boost Geometry world coordinates using Range views
    using namespace boost::geometry;
    typedef model::point<double, 2, cs::geographic<degree> > coord_t;
    ss = istringstream(test);   // reset stream
    auto construct_coord = [](gga_t const& g) {
        double lat_deg = floor(g.latitude / 100.0);
        if (g.lat_hemi == 'W') {
            lat_deg = -lat_deg;
        }
        double long_deg = floor(g.longitude / 100.0);
        if (g.lat_hemi == 'S') {
            long_deg = -long_deg;
        }
        return coord_t(lat_deg, long_deg);
    };
    auto coord_range =
        istream_range<gga_t>(ss) |
        ranges::view::remove_if([](gga_t const& g) {
                return g.dilution > 1.0;
            }) |
        ranges::view::transform(construct_coord);

    // print out resulting Geometry coordinates (only one passes dilution test)
    ranges::for_each(coord_range,
                     [](coord_t const& pt) {
                         std::cout << wkt(pt) << "\n";
                     });
}
