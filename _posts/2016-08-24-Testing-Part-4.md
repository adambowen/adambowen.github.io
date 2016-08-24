---
layout: post
title: "On Testing Part 4: Behaviour-driven development"
tags:
  - testing
---

[Behaviour-driven development](http://dannorth.net/introducing-bdd/) specifies
that tests of any unit of software should be specified in terms of the desired
behaviour of the unit.
Which is good, because, as we've already established testing behaviour is what
lets our tests stand the test of time.

BDD has a lot of useful ideas, but for now, let's focus on the idea that a test
case is made up of natural language.
In BDD every test you write contains these three statements:

* Given some initial context (the givens),
* When an event occurs,
* Then ensure some outcomes.

If you've got something to hand that has (or needs) tests, stop reading this
article and go away and try writing the above statements out for every test you
have (or plan to have).
Move code around if you have to so that it falls under the right statement.
Did you manage it?
If not, chances are there's something wrong with your test!
The most likely thing is that you're trying to test too much in one go, try
splitting the test up.

Now look at your statements, do any of them refer to implementation details?
If so, then you're testing implementation and not behaviour - either rephrase
them in terms of behaviour, or delete the test altogether.

The above "template" for test cases is a bit restrictive right now, actually
your tests probably form a kind of hierarchy:

* Given some initial context (the givens),
    * When an event occurs,
        * Then ensure some outcomes,
        * Then ensure some other outcomes.
    * When another event occurs,
        * Then ensure some outcomes,
        * And When yet another event occurs
            * Then ensure yet more outcomes

Some frameworks support this kind of syntax and some don't, but don't be afraid
to group tests together here if you can - each unique path through the tree
represents ONE test case.
Grouping them together like this helps to minimise code duplication and makes it
easier to understand which test cases need deleting or updating when the
behaviour has changed.
Frameworks that support BDD let you express nested relationships like the above
in a clear and linear fashion, without the need for test fixtures or functions.
Being able to do this is an important part of making a test case easy to read.

At this point it's probably worth presenting some kind of example, just so you
can see what I think BDD tests should look like.
This example will be C++ using the [Catch](https://github.com/philsquared/Catch)
framework, but don't be afraid - even if you don't know C++, so long as you're
vaguely familiar with the syntax then you should be able to read the code and
get what's going on.
In C++ a vector is basically a dynamic, 1D array.
Here's the prototype for a much stripped down baby vector of integers.

    class baby_vector
    {
    public:
        baby_vector();
        ~baby_vector();

        int size() const;
        void push_back(int value);
        void pop_back();
        int& operator[](int index);
        const int& operator[](int index) const);

        int *m_data;
        int m_size;
        int m_capacity;
    };

    baby_vector::baby_vector() :
        m_data(),
        m_size(),
        m_capacity()
    {
    }

    baby_vector::~baby_vector()
    {
        delete[] m_data;
        m_data = NULL;
        m_size = m_capacity = 0;
    }

    int baby_vector::size() const
    {
        return m_size;
    }

    void baby_vector::push_back(int value)
    {
        if(m_size+1 < m_capacity)
        {
            m_capacity = m_size+1;

            int *tmp = new int[m_capacity];
            for(int i=0; i<m_size; ++i)
                tmp[i] = m_data[i];
            delete[] m_data;
            m_data = tmp;
        }

        m_data[m_size] = value;
        ++m_size;
    }

    void baby_vector::pop_back()
    {
        if(m_size>0)
            --m_size;
    }

    int& baby_vector::operator[](int index)
    {
        return m_data[index];
    }

    const int& baby_vector::operator[](int index) const
    {
        return m_data[index];
    }

Please note, this is deliberately basic, and includes some poor design decisions
purely for illustrative purposes.
If you copy and paste this code into your own code base and someone doesn't
immediately call you out for not using `std::vector<int>` then something is very wrong.

So, let's write a super bad unit test for this guy.

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

When I see tests like this, two things jump out at me.
First, judging by the comments, the person writing it wasn't thinking
behaviourally - see how each test is trying to test a specific member of the
class, and not testing the *behaviour* of the class as a whole?
Secondly, this test won't survive even small changes in implementation - not
only does it rely on internals to check the results of some operations, but it
relies on internals to set-up the operations in the first place.

![A surgeon and a crib]({{ site.baseurl }}/images/testing-internals.png "It's fine, I just need to take a look at the heart to see if he has a pulse")

Let's get behavioural!
There's quite a few behaviours we might want to test here, let's pick three:

* You can add things to the end of the vector.
* You can remove things from the end of the vector.
* You can read and write using the [] operator.

For each behaviour we can construct some scenarios to give us confidence that
the vector behaves in the way we would expect.

1. You can add things to the end of the vector
    * GIVEN an empty vector
        * WHEN `push_back` is called with a value of 4
            * THEN the size is 1
            * THEN the item at index 0 is 4
    * GIVEN a vector containing 4 1 3
        * WHEN `push_back` is called with a value of 5
            * THEN the size is 4
            * THEN the item at index 3 is 5

2. You can remove things from the end of the vector.
    * GIVEN a vector containing 4 1 3
        * WHEN `pop_back` is called
            * THEN the size is 2
            * THEN the first two items are unchanged

3. You can read and write using the [] operator.
    * GIVEN a vector containing 4 1 3
        * THEN the item at index 0 is 4
        * THEN the item at index 1 is 1
        * THEN the item at index 2 is 3
        * WHEN 5 is written to index 1
            * THEN the item at index 0 is 4
            * THEN the item at index 1 is 5
            * THEN the item at index 2 is 3

You'll find if you write all these cases out _before_ you start to write tests
(in true TDD/BDD style before you've even started to implement the functionality)
then you'll find it a lot easier to design and implement your tests.
The nice thing about the Catch framework is that we can translate these
statements directly into the scaffolding of our test.

    #define CATCH_WITH_MAIN
    #include "catch.hpp"

    SCENARIO("You can add things to the end of the vector")
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

See...no code written, but already the structure of our test is mapped out.
If you like, have a go at filling it in, it should pass.

Here's what I get:

    #define CATCH_WITH_MAIN
    #include "catch.hpp"

    SCENARIO("You can add things to the end of the vector")
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

    SCENARIO("You can remove things from the end of the vector.")
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

    SCENARIO("You can read and write using the [] operator.")
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

Now, let's make a small implementation change and see if the tests survive.
Let's change the definition of the `baby_vector` class to this:

    #include <vector>
    typedef std::vector<int> baby_vector;

I'm sure you'll agree this is a pretty small change, but the original test case
would be utterly broken.
Try the new test case...it should still pass (if it doesn't you might want to
consider a new compiler).

That's it for this article.
The best way to explore BDD is to try it yourself - the next time you're
writing some automated tests (at any level in the pyramid), try writing out the
GIVEN/THEN/WHEN statements for the tests you're writing and see if you like the
way it feels.
Even better, if you can, give the Catch framework a try.
When we first started combining the BDD style with the Catch framework, we found
it was a big boost for the overall readability and quality of our test cases.
Next time I plan to write a few comments about manual testing in an Agile
environment.
