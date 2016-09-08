---
layout: post
title: "The Case for Exceptions Part 1: Introduction"
tags:
  - exceptions
---

One of the criticisms levelled at C++ is that its large and complex feature
set can be difficult to master, and as someone who's been programming
primarily in C++ for over 15 years and is still discovering new and
exciting aspects of the language that's certainly a viewpoint I can relate to.

That said, my personal belief is that it is this broad feature set that allows
you to choose exactly the right tools for the job at hand. In order to be able
to do that, you need to understand the benefits and drawbacks of each feature -
when it will help and when it will hinder you.

![Java toolbox small, C++ toolbox big]({{ site.baseurl }}/images/toolbox.png "To be honest, my Java knowledge is about 12 years out of date at this point.")

One of the features I often see criticised is the exception. Google, for example
[prohibit exceptions in C++ code](https://google.github.io/styleguide/cppguide.html#Exceptions)
and the ever prolific Joel Spolsky wrote an article some twelve years ago
[lamenting their use](http://www.joelonsoftware.com/items/2003/10/13.html)
(disclaimer: I have no idea if he still holds the same opinion). Even Raymond
Chen of Microsoft [considers exceptions to be "too hard"](http://blogs.msdn.com/b/oldnewthing/archive/2005/01/14/352949.aspx?wa=wsignin1.0).
Other authors make similar arguments [^neil][^ptgmedia].

[^neil]: [Exception Handling by Neil Fraser](http://neil.fraser.name/writing/exception/)
[^ptgmedia]: [Exception Handling: A False Sense of Security](http://ptgmedia.pearsoncmg.com/images/020163371x/supplements/Exception_Handling_Article.html)

These authors all make good arguments as to why they don't like exceptions, and
yet exceptions as a feature are prolific in almost all modern languages - so
there must be something to this idea if it keeps kicking around. In this article
I would like to address some of the issues with exceptions and explain why I
think they should be used.

The main arguments against exceptions boil down to

* Difficulty in identifying "good" error handling code.
* Difficulty in analysing the code path that might be followed when an error occurs.
* The potential for data to be left in an inconsistent state.
* Decrease in performance.

Whilst the arguments for them are

* More clarity in the primary flow of control.
* Improved transfer of information from the error site to the handling site.
* Stronger object invariants because constructors can fail.
* Exception safe code does not need to know anything about the errors that can
    occur if it has no reasonable way to handle them.

These points have been argued by (amongst others) [David Abrhams](http://www.boost.org/community/exception_safety.html)
and [Marshall Cline](http://www.parashift.com/c++-faq-lite/exceptions.html)
([see also, the C++ Super-FAQ](https://isocpp.org/wiki/faq/exceptions#why-exceptions)).

In the rest of this series I will go into more detail on the pros and cons of
exceptions, and try to explore real examples and alongside my own opinions.
