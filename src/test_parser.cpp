#include <catch2/catch.hpp>

#include "parser.h"

TEST_CASE(" Parser::parse() is executed", "[Parser::parse()")
{
    SECTION(" with invalid program as an argument")
    {
        REQUIRE_THROWS(Parser::get_instance().parse("tests/test_parse_invalid.txt"));
    }
    SECTION(" with correct program as an argument")
    {
        REQUIRE_NOTHROW(Parser::get_instance().parse("tests/test_parse_correct.txt"));
    }
}