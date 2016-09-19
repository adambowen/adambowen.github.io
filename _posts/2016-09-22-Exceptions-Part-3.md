---
layout: post
title: "The Case for Exceptions Part 3: Flow"
tags:
  - exceptions
---

In this post I would like to talk about what I consider to be one of the primary
benefits _for_ using exceptions to handle errors.
That is, I believe that exceptions improve the _flow_ of the code.

It is certainly true that using exceptions for error handling changes the way
you have to think about a lot of code, and can make it more likely that error
conditions will lead to faults such as leaked resources or inconsistent states.
It is also the case that thinking about how exceptions will propagate can
make writing correct code more complex. Both of these issues are about the
_exceptional_ code path - and I intend to address those in future posts.

In this post I want to talk about the effect exceptions have on the _normal_
code path, the _normal flow of control_. This is important because most of the
time execution will follow this path, so we need to consider the impact
exceptions have on our ability to write this code, because this is often the
most important code in the system, the code that actually executes the business
logic or performs the functions the system was intended to perform.

It is my belief that using exceptions substantially increases the clarity of the
normal path of execution. Error handling code is separated from the normal flow,
so it is clear which code is intended to execute in each scenario. Typically,
less error handling code is also required - meaning that more code can be seen
at a time. Finally, when code cannot handle errors no specific error handling
code is required - making it clear which code can handle errors and which is
just forwarding them on.

![Fork in the river]({{ site.baseurl }}/images/exception-flow.png "The case against exceptions points out that there are icebergs on the exception route.")

I feel this is most clear with a simple example. Some years ago I worked for a
company called [Pattern Analytics Research](http://www.patternanalytics.co.uk),
developing an SDK for fusing results from multiple biometric algorithms. Like
all good SDKs, with ours we provided complete examples illustrating how to use
the SDK, including handling errors. We provided both a C and C++ interface (the
C++ interface was a wrapper on the C interface that allowed typical C++ patterns
such as RAII and exception handling to be used).

First, here is the sample code for the C interface:

{% highlight cpp %}
{% include code/par_fusion/c_example.c %}
{% endhighlight %}

Now, here is the sample code for the C++ interface:

{% highlight cpp %}
{% include code/par_fusion/cpp_example.cpp %}
{% endhighlight %}

I feel that this example illustrates quite clearly the difference. Firstly, the
C++ example is 38 lines, vs. 65 for the C example. Given that they do exactly
the same thing, this clearly makes it easier to read (especially since this is
sample code). Obviously some of this can be attributed to RAII - but I've
found that code bases that don't generally permit exceptions become lax about
RAII, so this isn't an entirely unfair comparison.

Secondly, the error handling is confined to (almost only) one place in the C++
example. This makes it easier to see how errors are handled, and what happens
when errors don't occur.

Lastly, there is no duplication of error handling or clean-up. Again, this is a
result of good C++ practices such as RAII and consolidation of common code - but
exceptions make this easier.

So, in summary, exceptions make the normal flow of control easier to read, and
makes it easier to _write_ code that is easier to read. This is the code that
is executed most often, and therefore likely to be most read both when being
written, modified, and reviewed. It seems to me that this makes exceptions
a clear benefit for the normal flow of control. In the next few posts we'll
look at how exceptions benefit object oriented programs, and then start to look
at the cases that cause all the problems with exceptions - the times when an
error actually occurs!
