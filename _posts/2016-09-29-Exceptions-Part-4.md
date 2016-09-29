---
layout: post
title: "The Case for Exceptions Part 4: Invariance"
tags:
  - exceptions
---

In C++ there is one situation where it is _only_ possible to use exceptions to
report an error - when object constructors can fail. In exception-free code
bases this leads to the additional requirement that constructors must always
succeed. For certain design patterns having constructors that always succeed can
simplify code - but in this article I want to look at the knock on effect this
decision has on the design of the entire system, and how exceptions actually
permit a simplified, more coherent design.

To understand how this comes about, I first need to explain the way I view
object design. Most objects can be classified into one of three types:

- Data
- Functional Objects
- Data + Invariants [^interfaces]

_Data_ objects are merely containers for data, where the set of valid states for
objects is the outer product of the set of valid states of the members of the
object. In C++ I typically represent this type of object with a `struct`. These
are most often used for "options" structures, or to logically group objects that
fall into the other categories. Sometimes we might use a `class` to represent
this type if the class is a form of "state-space" mapping, for example, a class
that stores fixed point values in a 32 bit signed integer has the same number of
states as the signed integer, but the states have different meanings. Because
there are no invalid states, it is always possible to construct such a class in
a valid state, provided that the same is true of the members of the class.

_Functional Objects_ are abstractions for some functional operation. In C++
these are often lambdas and they may include some state that is part of the
algorithm or some configuration that determines how the algorithm should
operate. It is possible, but not essential, for exceptions to be used to
handle invalid inputs when the constructor accepts inputs such as data or
configuration. However, it is also entirely possible to design objects in this
category such that they are always constructed in a valid state.

The last type, _Data + Invariants_ (D+I), is the one we are most interested in.
It is also the one that most objects fall into. Unlike _data_ classes, the set
of valid states of D+I classes is a _subset_ of the outer product of the valid
states of the member variables. The operations on a D+I class represent the
invariant-preserving operations that can be performed on the data. D+I classes
are useful because they help to reduce the set of valid states of the system.
In "[On Testing Part 2](/Testing-Part-2/)" I talk about the different layers of
the test pyramid. With D+I classes we can test that invariants are preserved at
the unit level, and then assume that the invariants will hold at the levels
above. Thus, we implicitly increase the coverage of the higher level tests.

[^interfaces]: You might wonder where interfaces fit in this categorisation. My
	view is that interfaces can fall into any of the categories. Some interfaces
	are just "accessors" for data which do not specify how that data is
	represented. Some are "accessors" for functionality, and some provide an
	interface that preserves invariants of one form or another.

So why are exceptions beneficial to D+I classes? Well, it comes down to the
strength of the invariants. If the initialisation of a class can fail then
without exceptions we need to split the initialisation between the constructor
and an initialisation function (the `init` method). This means that between the
constructor and `init` call _the class can exist in an uninitialised state_.
This greatly weakens the invariants on the class, as they must all accommodate
this possibility. In turn, all _users_ of the class should also consider the
case where the class is in an uninitialised state. In essence we must push the
testing up the pyramid from the unit level to the integration level, where it
is much harder to cover all the possible states of the system.

![Tweedledum, Tweedledee, and Tweedledumber]({{ site.baseurl }}/images/dum-dee-dumber.png "They're actually quadruplets, the other brother keeps telling you there's a goat behind his door.")

Objects that can exist in an uninitialised state are analogous to pointers, and
it is generally poor form to assume that a pointer is not NULL. However, if you
do all your initialisation in your constructor and fail with exceptions, then
you can add 'the object is always initialised' to your list of invariants, and
so it becomes easier and safer to make assumptions about the state of the object.

So exceptions can strengthen object invariants, but this comes at the cost of
having to maintain invariants in an exception-safe fashion.
It is true that exceptions can make this more difficult because the fact that
errors occur can be hidden - however, the _requirement_ to maintain invariants
through error conditions is not additional, all code that handles errors must
meet this requirement.

Toward the end of the series I intend to look at the practices you can employ
that make the process of writing exception safe code easier, and hopefully we
will be able to look at how to go about preserving invariants. For now though,
that's the end of this part of the series. In the next part I continue to
discuss the benefits of exceptions.
