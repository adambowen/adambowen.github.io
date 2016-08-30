#include "catch.hpp"

#include "baby_vector.h"

TEST_CASE("baby_vector")
{
    // check that size returns the size
    baby_vector v;
    v.m_size = 42;
    CHECK( v.size() == 42 );

    // check that push_back allocates and adds the item to the
    // end
    v.m_size = 0;
    v.push_back(42);
    REQUIRE( v.m_data );
    CHECK( v.m_data[0] == 42 );
    CHECK( v.m_size == 1 );
    CHECK( v.m_capacity == 1 );

    // check that pop_back removes something from the end of
    // the array
    v.pop_back();
    CHECK( v.m_size == 0 );

    // check that operator[] works
    int data[2] = {11,12};
    v.m_data = data;
    v.m_size = 2;
    v.m_capacity = 2;

    CHECK(v[0] == 11);
    CHECK(v[1] == 12);
    CHECK(static_cast<const baby_vector&>(v)[0] == 11);
    CHECK(static_cast<const baby_vector&>(v)[1] == 12);

    v.m_data = NULL;
}
