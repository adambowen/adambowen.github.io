---
layout: post
title: "The Case for Exceptions Part 8: Legacy Code"
tags:
  - exceptions
---

In his cppcon 2014 talk, Jon Kalb [^esc] gave a quick summary of how to convert
legacy code to exception safe code [^kalb3]. In this post I thought I'd go
over his technique, because it is useful to know how to safely and
incrementally convert exception-unsafe-code to exception-safe-code.

[^kalb3]: [Exception Safe Code (part 3) by Jon Kalb](https://www.youtube.com/watch?v=MiKxfdkMJW8)
[^esc]: [Exception Safe Coding in C++](http://exceptionsafecode.com/)

The core of Jon's approach is one that is fundamental to maintaining any
software system, be it legacy or otherwise - Sean Parent's iron law of legacy
refactoring:

> Existing contracts cannot be broken

A legacy code base which does not use exceptions has the implicit contract that
no code can throw, in essence every function in the legacy code base is
providing the nothrow contract.

![Contract with the Devil]({{ site.baseurl }}/images/contract.png "Most demons write their contracts in C, so they're not used to having to handle exceptions.")

How can we introduce exceptions into a code base that currently permits none?

Take this carefully contrived sample code:

{% highlight cpp %}
{% include code/legacy_esc/legacy.cpp %}
{% endhighlight %}

This code is obviously not exception safe, and clearly makes the nothrow
assumption for several functions it calls. How do we start making our code
exception safe?

To do so we need to follow three simple rules

1. All new code must provide the basic exception guarantee
2. Old (pre-exception-safety) code must never call new (post-exception-safety)
   code without a _switch_ from exceptions to alternative error handling.
3. As required we incrementally refactor old code into an exception-based code
   with a legacy wrapper function.

_1_ is necessary because, as we have already discussed, we cannot write _any_
exception safe code if we do not provide the basic exception safety guarantee.
_2_ is necessary because we cannot break the existing contract that legacy code
does not throw.
_3_ is how we make progress, any time we touch legacy code instead of modifying
it directly, we first convert it to code that provides the basic guarantee and
then expose a wrapper that maintains the original contract.

For the above example, the result looks like this:

{% highlight cpp %}
{% include code/legacy_esc/esc.cpp %}
{% endhighlight %}

Over time, as all the usages of `create_object_from_config` are replaced with
`create_object_from_config_esc`, we can remove `create_object_from_config` from
the code altogether.

The first question that comes to mind when seeing this technique is "how do we
know a function we are calling is exception safe?". This is a tricky problem,
but fortunately we don't need to know. Rule 3 is _any_ time we touch legacy code
we make it exception safe - so any time we are modifying an "unsafe" function,
we also refactor it to make it exception safe as part of the process.

If we follow this rule rigidly, we can never be in a situation where unsafe code
calls code that raises exceptions. Under these conditions, code that raises
exceptions must be new code (because we didn't have any before) and all new
code is exception safe (rule 1). Therefore, in order to call the new code from
unsafe code we must be modifying legacy code - but rule 3 tells us that we must
refactor the unsafe code to be safe at that time.

In addition, we say that we cannot modify contracts, we when we modify legacy
code we guarantee that exceptions will not be permitted to propagate.

The next question we might ask is "how do we know if a function needs updating
to provide the basic guarantee?". Again, this is less of a problem than we
might think - because the problem here is the same as ensuring all new code
provides the basic exception guarantee. We must _train_ ourselves to spot unsafe
code (using the [guidelines](/Exceptions-Part-7/)), and we must enforce it
through basic code-quality practices such as code review and pair programming.

The last question I can think of is the most difficult - "how do we know what
contract a function provides?". This is the most difficult because the lack of
exceptions is an _implicit_ contract provided by the legacy code. As such, it
is likely to be undocumented, either through comments or language specifications
like `nothrow`. We could adopt naming conventions (such as the `_esc` suffix in
the example code), or we could add documentation or language specifications to
the legacy code, or, my personal favourite, we could _partition_ the code base.

By "partition" I mean that we keep all the legacy code in one directory, and all
new (exception-safe) code in a separate directory. We may need to tweak our
build system to accommodate this structure - but that is surely less effort and
less error prone than the other systems we can adopt.

This also has the nice side effect of making code easy to review - we should
only see two kinds of change on the legacy side:

1. To convert a legacy function to a wrapper for an exception-safe alternative.
2. To delete legacy code that has been completely converted to exception-safe
   alternatives.

Finally, I think it's worth briefly mentioning data structures. These are
difficult to manage because it may be difficult or undesirable to create a
parallel, exception-safe, data structure. In these cases we will have to use
documentation and language specifications to mark functions that are part of the
legacy interface of the data structure as `nothrow`.

In this article we have looked at how we can define a process via which a legacy
code base can be incrementally converted to an exception safe code base. I hope
I've made a case that gets you thinking about whether introducing exceptions
into your code base might bring some benefits, but even if I haven't I would
urge you to to try write code that always provides the basic guarantee. Even
when using return value based error handling, proper use of RAII to handle
responsibilities, invariants, and the critical line is a strong way to write
_good_ code that is easier to read and maintain.

That's it for the planned content in this series, in November I'll start a
whole new topic!
