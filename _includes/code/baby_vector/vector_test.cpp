#include <vector>
#include "catch.hpp"

namespace
{
    typedef std::vector<int> baby_vector;
}

SCENARIO("You can add things to the end of the std::vector")
{
    GIVEN("an empty vector")
    {
        baby_vector v;

        WHEN("push_back is called with a value of 4")
        {
            v.push_back(4);

            THEN("the size is 1")
            {
                CHECK(v.size() == 1);
            }

            THEN("the item at index 0 is 4")
            {
                CHECK(v[0] == 4);
            }
        }
    }

    GIVEN("a vector containing 4 1 3")
    {
        baby_vector v;
        v.push_back(4);
        v.push_back(1);
        v.push_back(3);

        WHEN("push_back is called with a value of 5")
        {
            v.push_back(5);

            THEN("the size is 4")
            {
                CHECK(v.size() == 4);
            }

            THEN("the item at index 3 is 5")
            {
                CHECK(v[3] == 5);
            }
        }
    }
}

SCENARIO("You can remove things from the end of the std::vector")
{
    GIVEN("a vector containing 4 1 3")
    {
        baby_vector v;
        v.push_back(4);
        v.push_back(1);
        v.push_back(3);

        WHEN("pop_back is called")
        {
            v.pop_back();

            THEN("the size is 2")
            {
                CHECK(v.size() == 2);
            }

            THEN("the first two items are unchanged")
            {
                CHECK(v[0] == 4);
                CHECK(v[1] == 1);
            }
        }
    }
}

SCENARIO("You can read and write using std::vector::operator[]")
{
    GIVEN("a vector containing 4 1 3")
    {
        baby_vector v;
        v.push_back(4);
        v.push_back(1);
        v.push_back(3);

        THEN("the item at index 0 is 4")
        {
            CHECK(v[0] == 4);
        }

        THEN("the item at index 1 is 1")
        {
            CHECK(v[1] == 1);
        }

        THEN("the item at index 2 is 3")
        {
            CHECK(v[2] == 3);
        }

        WHEN("5 is written to index 1")
        {
            v[1] = 5;

            THEN("the item at index 0 is 4")
            {
                CHECK(v[0] == 4);
            }

            THEN("the item at index 1 is 5")
            {
                CHECK(v[1] == 5);
            }

            THEN("the item at index 2 is 3")
            {
                CHECK(v[2] == 3);
            }
        }
    }
}
