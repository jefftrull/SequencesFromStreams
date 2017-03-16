// Demonstrating the use of ranges built using a qi::spirit parser adapted to be a range directly through
// the range-v3 facade
#include <string>
#include <iostream>
#include <sstream>

#include <boost/spirit/include/qi.hpp>

#include <range/v3/all.hpp>

#include "nmea.h"

// create a Range that (underneath) uses Spirit to parse the "next" value
class gga_range_t : public ranges::view_facade<gga_range_t>
{
    friend ranges::range_access;
    struct cursor;

 public:

    typedef boost::spirit::istream_iterator iter_t;

    gga_range_t(std::istream& is) : m_stream(&is), m_it(*m_stream), m_parse_good(true) {
        using namespace boost::spirit;
        m_parser =
            qi::lit("$GPGGA") >> ',' >> qi::int_ >> ',' >>
            qi::double_ >> ',' >> qi::char_("NS") >> ',' >>
            qi::double_ >> ',' >> qi::char_("EW") >> ',' >>
            qi::int_ >> ',' >> qi::int_ >> ',' >>
            qi::double_ >> ',' >>
            qi::double_ >> ',' >> qi::char_ >> ',' >>
            qi::double_ >> ',' >> qi::char_ >> ',' >>
            -qi::int_ >> ',' >> -qi::int_ >> ",*" >> qi::int_ >> -qi::eol;
    }

private:
    cursor begin_cursor() {
        return cursor(*this);
    }

    gga_t cached() const {
        return m_g;
    }

    bool done() const {
        // EOF or something I don't recognize
        return !m_parse_good || !m_stream;
    }

    std::istream* m_stream;
    gga_t m_g;
    iter_t m_it;
    bool m_parse_good;
    boost::spirit::qi::rule<iter_t, gga_t()> m_parser;

    void next() {
        m_parse_good = boost::spirit::qi::parse(m_it, iter_t(), m_parser, m_g);
    }

    struct cursor {
        cursor(void) = default;
        explicit cursor(gga_range_t& rng) : m_rng(&rng) {
            next();    // to load the first value
        }

        void next() {
            m_rng->next();
        }

        gga_t read() const {
            return m_rng->cached();
        }

        bool equal(ranges::v3::default_sentinel) const {
            return m_rng->done();
        }

        gga_range_t* m_rng;
    };
};



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
    ranges::for_each(gga_range_t(ss),
                     [](gga_t const& g) {
                         std::cout << g << "\n";
                     });
}
