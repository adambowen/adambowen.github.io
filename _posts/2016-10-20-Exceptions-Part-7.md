---
layout: post
title: "The Case for Exceptions Part 7: Exception Safe Code"
tags:
  - exceptions
---

In 2014 Jon Kalb [^esc] gave a three hour presentation at cppcon on how to
write exception safe code [^kalb1][^kalb2][^kalb3]. In the talk, Kalb sets out
a series of guidelines on how to write exception safe code. In this post I'm
going to go over those guidelines, with a few small adaptations of my own. If
you're interested in using exceptions in your code I highly recommend the talk,
as you will find Kalb's thoughts mirror many of my own on the subject.

[^kalb1]: [Exception Safe Code (part 1) by Jon Kalb](https://www.youtube.com/watch?v=W7fIy_54y-w)
[^kalb2]: [Exception Safe Code (part 2) by Jon Kalb](https://www.youtube.com/watch?v=b9xMIKb1jMk)
[^kalb3]: [Exception Safe Code (part 3) by Jon Kalb](https://www.youtube.com/watch?v=MiKxfdkMJW8)
[^esc]: [Exception Safe Coding in C++](http://exceptionsafecode.com/)

## Guarantees

There are three guarantees that exception safe code can provide:

* The _nothrow_ guarantee: this code will not throw an exception
* The _basic_ guarantee: if an exception is thrown, invariants are preserved and no resources
    are leaked
* The _strong_ guarantee: if an exception is thrown, no state is changed (a "transaction")

All exception safe code must provide one or more of these guarantees, and at a
minimum all code must provide the basic guarantee even if it itself will not
throw an exception [^nothrow].

Some code can provide the strong guarantee, but in doing so there is sometimes
a performance penalty. Thus we should only provide the strong guarantee when
it is _natural_ and _free_ to do so. In practice the strong guarantee is not
required as often as you would expect, because the _basic_ exception guarantee
ensures that we will not leak resources or violate invariants we can know that
the program will still be in a valid (if not ideal) state.

> Guideline 1: All code must provide the basic exception safety guarantee

> Guideline 2: Code _should_ provide the strong exception safety guarantee when
	it is natural and free to do so.

[^nothrow]: That is to say, even if a function itself guarantees not to allow an
	exception to propagate, it must correctly handle exceptions raised by any
	functions that it calls.

## Assumptions

There are few assumptions we can make about code that uses exceptions, as we saw
in [part 6](/Exceptions-Part-6/) we should assume all code can throw unless
the no-throw guarantee is an explicit part of the interface. Note that this is
not quite the same as the C++11 nothrow decorator, as there may be circumstances
where we wish to state that code will never throw but not explicitly declare it
as such (C++ 2003 code for example).

> Guideline 3: Assume all code may throw unless it explicitly documented not to

![And you're sure this code won't throw?]({{ site.baseurl }}/images/nothrow.png "Dynamic exception specifications in practice!")


## Exception Specifications

C++ 2003 has the `throw()` dynamic exception specification mechanism. This is
deprecated in C++11, and replaced by the `nothrow` mechanism. Unlike `throw()`,
`nothrow` is useful because it documents one of the guarantees. The numerous
problems with the `throw()` mechanism are well documented [^throwbad].

[^throwbad]: [Why are exception specifications bad?](http://programmers.stackexchange.com/questions/114338/why-are-exception-specifications-bad)

> Guideline 4: DO NOT use dynamic exception specifications. DO use `nothrow`.

## Some no-throw functions are required

In C++, destructors should be no-throw (and are `nothrow` by default in C++11)
because destructor code is the only code that executes while unwinding the stack
during exception handling. If a destructor throws during an unwind with an
existing, active, exception the standard says the program will terminate - which
is very bad.

In order to write exception safe code it is often necessary to swap two objects
with the guarantee that the swap operation cannot fail. Thus, it is important
to provide either a no-throw swap implementation (C++ 2003) or a no-throw
move constructor (C++ 2011) to ensure this is possible.

> Guideline 5: Destructors must never throw

> Guideline 6: Swapping two instances of a class should never throw

## Know how to throw and catch

C++ lets us throw any type of object, but in practice we want to limit the types
of object that can be thrown. Firstly it is extremely useful to have all
exceptions derive from `std::exception`, this means that a generic exception
handler can catch a known type and still output a meaningful error message using
the `what()` method.

Secondly, we should always throw by value. Throwing a pointer implies the
lifetime of the object pointed to must at least continue until the exception is
caught and handled. In practice this means heap allocated or global objects, but
there is _no way for the catching site to reasonably know which case is
occurring_. It also makes generic exception handling impossible, as we cannot
necessarily know the correct type of the exception being caught in order to do
things like free the heap memory.

Finally, because we use inheritance to establish an exception hierarchy we need
to catch by reference to ensure we do not slice off any relevant information
held in the exception.

> Guideline 7: Derive exceptions from `std::exception` (directly or indirectly)

> Guideline 8: Throw by value, catch by reference

## Know when to catch

Exception safe code catches _all_ exceptions at program boundaries. These can
be API boundaries (e.g. between C++ code and a C API), process boundaries
(`main`), thread boundaries, or even event handler boundaries (for example, it
is advisable to overload `QApplication::notify` to trap unexpected exceptions
from event handlers).

In general there three times you need to catch an exception:

* To _switch_ error handling method. For example, changing from an exception to
	a user-facing error message, to log the error and resume, or to translate
	an exception to another exception type or to an error code.
* To recover from the error in code that has a _strategy_ for doing so. For
	example, if there is an alternative way to complete a failed operation.
* To continue processing in cases where _some success_ is acceptable. For
	example, when processing a list of items it may be acceptable if processing
	some items fails.

Most of the time, catch statements fall into the first class - in particular
the first class will be present at the _boundaries_ present in an application.

> Guideline 9: Catch all exceptions at boundaries

> Guideline 10: Catch when you need to _switch_ error handling mechanism,
	have a _strategy_ for recovering, or when _some success_ is acceptable.

## RAII: _Responsibility_ Acquisition Is Initialisation

For any _symmetric_ operation we must immediately and uniquely assign the
second half of the operation to an RAII container to ensure that it will be
performed as part of the stack unwinding should an error occur. For example,
for every `malloc` we should have a `free`, for every `new` a `delete`, for
every `init` a `release`, or for every `push` a `pop`.

Assignment must be immediate because we must assume that any intervening
operation can throw, should an intervening operation throw then the
_responsibility_ to complete the second half of the operation would be leaked.

Assignment must be unique because we must assume that acquiring a second (or
third, or fourth) responsibility may fail, which would leave the container in a
potentially indeterminate state and may result in the first responsibility being
leaked.

More generally, we have been talking about the acquisition of responsibilities,
any time we do something or reach some point where we are have a _responsibility_
to always perform some additional operation in the future we have acquired a
responsibility and we should be using an RAII container to guarantee execution
of that responsibility.

Whenever you see symmetric operations or responsibilities executed outside of a
destructor it should always raise a flag in your mind that the code is _wrong_.
Whenever you see cleanup code outside of a destructor (cleanup is a
responsibility!) it should always raise a flag in your mind that the code is
wrong.

One thing to be aware of here is that the C++ standard does not mandate the
particular order of evaluation of subexpressions.
For example, consider the following code:

	some_function(
		std::shared_ptr<A>(new A()),
		std::shared_ptr<B>(new B())
	);

This code is not safe because the compiler is free to implement this as:

	B *tmbB = new B();
	A *tmpA = new A();
	std::shared_ptr<A> ptrA(tmpA);
	std::shared_ptr<B> ptrB(tmpB);
	some_function(ptrA, ptrB);

So if the constructor for `A` throws, `B` _could_ be leaked. In fact, the
preferred way to do this in C++11 is to use `make_shared` (and also
`make_unique` for C++14).

	some_function(
		std::make_shared<A>(),
		std::make_shared<B>()
	);

This is because the `make_shared` function _uniquely_ and _immediately_ assigns
the allocated instance to the shared pointer.

> Guideline 11: Immediately and uniquely assign responsibilities to RAII
	containers. As a consequence, all _symmetric_ operations should be performed
	by constructor/destructor pairs.

> Guideline 12: Never acquire responsibilities as part of an expression that
	can throw (except to report that the acquisition failed). Prefer
	`std::make_shared` and `std::make_unique` to `new`.

## The Critical Line

One of the most important ideas in Kalb's cppcon presentation is that of the
critical line. This is what allows us to tell the difference between code that
provides the strong guarantee and code that does not (or at least, is unlikely
to). It can also be important for identifying whether code will preserve
invariants or not.

The idea of the critical line is simple. All code that can throw in a function
is _before_ the critical line - but no code before the critical line modifies
state. All code that modifies state is _after_ the critical line - but no code
after the critical line can throw.

If we cannot draw a critical line then there is a good chance the code will not
provide some guarantees in the presence of exceptions.

Consider an implementation for the assignment operator of a pair:

    template<typename A, typename B> pair<A,B>::operator=(const pair<A,B>& other)
    {
    	first = other.first;
    	second = other.second;
    }

We can start by identifying code that can fail, and code that can modify state

    template<typename A, typename B> pair<A,B>::operator=(const pair<A,B>& other)
    {
    	first = other.first;   // assignment operator could throw, modifies first
    	second = other.second; // assignment operator could throw, modifies second
    }

Immediately we see that we have code that modifies state interleaved with code
that can throw. Can we fix this? Well, it turns out we can - because earlier
we defined swap operations to provide the no-throw guarantee as one of our
guidelines:

    template<typename A, typename B> pair<A,B>::operator=(const pair<A,B>& other)
    {
    	A copyA(other.first);  // copy constructor could throw
    	B copyB(other.second); // copy constructor could throw

    	// ------ the critical line ------

    	swap(first, copyA);  // modifies first, cannot throw
    	swap(second, copyB); // modifies second, cannot throw
    }

This surprisingly simple concept suddenly makes it easy to differentiate "good"
exception safe code from "bad" exception safe code. All we need to do is find
the first statement that modifies state (for the strong guarantee) and determine
if any subsequent statements can throw.

> Guideline 13: meet exception safety guarantees using the "critical line"

## Conclusion

In this post I've gone over 13 guidelines for writing exception safe code.
In the final part of this series I will introduce a process via which legacy
code can be converted to exception safe code.
