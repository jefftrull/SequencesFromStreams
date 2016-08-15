// Demonstrating the use of ranges built by executing a spirit::qi grammar in a coroutine
#include <string>
#include <iostream>
#include <sstream>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/coroutine2/all.hpp>

#include "nmea.h"

typedef boost::spirit::istream_iterator iter_t;

struct nmea_parser : boost::spirit::qi::grammar<iter_t>
{
    typedef boost::coroutines2::asymmetric_coroutine<gga_t>::push_type sink_t;

    nmea_parser(sink_t& sink)
        : nmea_parser::base_type(sentences), m_sink(sink)
    {
        using namespace boost::spirit;
        namespace phx = boost::phoenix;

        gga_sentence =
            qi::lit("$GPGGA_T") > ',' > qi::int_ > ',' >
            qi::double_ > ',' > qi::char_("NS") > ',' >
            qi::double_ > ',' > qi::char_("EW") > ',' >
            qi::int_ > ',' > qi::int_ > ',' >
            qi::double_ > ',' >
            qi::double_ > ',' > qi::char_ > ',' >
            qi::double_ > ',' > qi::char_ > ',' >
            -qi::int_ > ',' > -qi::int_ > ",*" > qi::int_;

        // define other "sentences" here

        // combine alternates
        // (get the right sink member operator first to avoid ambiguous overload)
        typedef sink_t & (sink_t::*oper_t)(gga_t const&);
        sentences = *(gga_sentence   // | bwc_sentence | gll_sentence | ... )
                      [phx::bind(static_cast<oper_t>(&sink_t::operator()), phx::ref(m_sink), qi::_1)]
                      >> -qi::eol);

    }

    boost::spirit::qi::rule<boost::spirit::istream_iterator, gga_t()> gga_sentence;
    boost::spirit::qi::rule<boost::spirit::istream_iterator> sentences;

    // destination for GGA sentences as they are parsed
    sink_t& m_sink;

};

std::ostream& operator<<(std::ostream& os, gga_t const& g) {
    os << '(' << g.latitude << g.lat_hemi << ',' << g.longitude << g.long_hemi << ')';
    return os;
}

int main() {
    std::string test("$GPGGA_T,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,,*47");
    test +=        "\n$GPGGA_T,170834,4124.8963,N,08151.6838,W,1,05,1.5,280.2,M,-34.0,M,,,*59";

    std::istringstream ss(test);
    ss.unsetf(std::ios::skipws);
    using namespace boost::coroutines2;
    asymmetric_coroutine<gga_t>::pull_type sequence(
        [&ss](asymmetric_coroutine<gga_t>::push_type& sink) {
            nmea_parser nmea(sink);
            iter_t beg(ss), end;
            if (!boost::spirit::qi::parse(beg, end, nmea)) {
                std::cerr << "parse failed!\n";
            }
        });

    for (gga_t const& g : sequence) {
        std::cout << g << "\n";
    }
}
