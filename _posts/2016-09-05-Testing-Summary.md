---
layout: post
title: "On Testing: Summary"
tags:
  - testing
---

This post is the TLDR of my On Testing series.

### Write tests

In any environment where you are starved for testing resources automated tests
are a good way to build confidence that your software works, and continues to
work. In environments where you have sufficient or excess testing resources
automated tests free up testers to perform more demanding activities such as
_exploratory testing_ that cannot be automated.

### Write tests at every level of the test pyramid

![The Test Pyramid]({{ site.baseurl }}/images/test-pyramid.png "The Test Toblerone did not market as well")

The test pyramid givens us a model that says layers of testing are built upon
one another. At high levels of the pyramid we cannot reasonably cover all the
possible states of the system, but we can cover many of the interactions. At the
lowest level we may cover little or no interactions in the system, but can
cover a reasonable number of states. Automated tests have the most value when
they cover all levels of the pyramid, the low level tests give us confidence
that, given the right interactions, the states are correct. The high level
tests give us confidence that, given the right states, the interactions are
correct.

### Write tests that deliver the most value

As developers we must always keep in mind that we should be trying to deliver
value for the business or project. Thus, we need to place our tests within the
pyramid in the places that let us achieve the _most value_ for the
_least effort_. As such, sometimes it will be easier to write an integration
test to cover some functionality because mocking out the dependencies requires
too much effort. On other occasions it will be easier to unit test certain
components, because integration tests require too much set-up. We take all
this into account when writing our tests, while also remaining mindful that we
need tests at all layers of the pyramid.

### The goal of automated testing is to build confidence that the software still works as designed.

Our tests must be designed to stand the test of time, so that tests can be run
with little or no modification in order to confirm that, after some changes,
the software works as it did before (i.e. as it was originally designed). In
the presence of the test pyramid this becomes even more important, as the
higher level tests are built on the lower level tests. Removing or modifying
low level tests invalidates the results of higher level tests - particularly any
manual testing that may have taken place.

### Delete tests that aren't useful

Brittle tests violate the principle that tests should stand the test of time. It
is better to delete (and perhaps rewrite) brittle tests than investing effort
maintaining them.

### Tests should be self-contained

When diagnosing test failures, it is important to be able to see everything that
a test does in order to understand its purpose. On the occasions when it is
necessary to modify a test it is important to be able to determine all the
interactions involved in the test by inspection in order to be able to
revalidate the test.

These two _requirements_ of a test suggest that we should _remove loops_ that
disguise multiple test cases as a single one, and _remove shared set-up and
tear-down_ that hides common test elements (nb. it is my belief that the
Catch/BDD style of writing tests achieves the best of both by allowing shared
set-up/tear-down _without_ hiding it).

### Test behaviour, not implementation

Behaviour is what survives change and refactoring, so in order to write tests
that stand the test of time we must focus on testing behaviour and not
implementation.

### Avoid "hand-offs" to testers

Agile principles already say that we should avoid hand-offs, but hand-offs to
testers are particularly troublesome in an agile environment as the hand-off
significantly disrupts the flow of the sprint. It is far more productive to
incorporate testing into the development in order to achieve continuous
feedback.

Any tester worth their salt aims to be an _expert_ in using the software.
Handing off work to them after it has been completed is a criminal waste of
these talents. By incorporating testers into the development they can provide
valuable insights into feature design from the point of view of an expert user.


### That's all folks!

This is the end of the articles I've planned for this series, I hope to add
more in the future whenever I learn new things, find ways to improve my approach,
or simply have some nice examples to share of things going well and going...
not so well!
