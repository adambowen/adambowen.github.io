#include "catch.hpp"
#include "baby_vector.h"

SCENARIO("You can add things to the end of the vector.")
{
    GIVEN("an empty vector")
    {
        WHEN("push_back is called with a value of 4")
        {
            THEN("the size is 1")
            {
            }

            THEN("the item at index 0 is 4")
            {
            }
        }
    }

    GIVEN("a vector containing 4 1 3")
    {
        WHEN("push_back is called with a value of 5")
        {
            THEN("the size is 4")
            {
            }

            THEN("the item at index 3 is 5")
            {
            }
        }
    }
}

SCENARIO("You can remove things from the end of the vector.")
{
    GIVEN("a vector containing 4 1 3")
    {
        WHEN("pop_back is called")
        {
            THEN("the size is 2")
            {
            }

            THEN("the first two items are unchanged")
            {
            }
        }
    }
}

SCENARIO("You can read and write using the [] operator.")
{
    GIVEN("a vector containing 4 1 3")
    {
        THEN("the item at index 0 is 4")
        {
        }

        THEN("the item at index 1 is 1")
        {
        }

        THEN("the item at index 2 is 3")
        {
        }

        WHEN("5 is written to index 1")
        {
            THEN("the item at index 0 is 4")
            {
            }

            THEN("the item at index 1 is 5")
            {
            }

            THEN("the item at index 2 is 3")
            {
            }
        }
    }
}
