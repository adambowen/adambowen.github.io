---
layout: post
title: "The Case for Exceptions Part 6: The Wrong Way"
tags:
  - exceptions
---

Back in 2005 Raymond Chen of Microsoft [^chen] wrote a piece lamenting exceptions as
making a hard problem harder. In this article I want to revisit his thoughts
and see how they fit in with what we know 11 years later.

[^chen]: [Cleaner, more elegant, and harder to recognize by Raymond Chen](https://blogs.msdn.microsoft.com/oldnewthing/20050114-00/?p=36693)

The first thing he gives us is a breakdown of how hard it is to write error
handling code.

| Really easy                       | Hard                               | Really hard                       |
|-----------------------------------|------------------------------------|-----------------------------------|
| Writing bad error-code-based code | Writing good error-code-based code | Writing good exception-based code |
| Writing bad exception-based code  |                                    |                                   |

Obviously, writing bad code is easy and writing good code is hard, but why does
he believe exception based code is really hard?

> It's really hard to write good exception-based code since you have to check every single line of code (indeed, every sub-expression) and think about what exceptions it might raise and how your code will react to it

So Chen believes that you need to think about how every line of code might fail,
and how the code will react to it.
But in 2014, Jon Kalb gave a talk at cppcon where he said this is how he used
to think about exceptions [^kalb1]:

[^kalb1]: [Exception Safe Code (part 1) by Jon Kalb](https://www.youtube.com/watch?v=W7fIy_54y-w)

> The Wrong Way
>
> * Carefully check return values / error codes to detect and correct problems
> * Identify functions that can throw and think about what to do when they fail
> * Use exception specifications so the compiler can help create safe code
> * Use try / catch blocks to control code flow

Kalb did not only say this the hard way to write exception safe code (as Chen
had previously identified), he went as far as to say this is the _wrong_ way to
write exception safe code - that we must change how we think about the problem
in order to solve it.
If there is an easier way (and I believe there is), then all we have to do is to
teach that easier way and we should be able to adjust Chen's categorisation:

| Really easy                       | Hard                                                     | Really hard                                                |
|-----------------------------------|----------------------------------------------------------|------------------------------------------------------------|
| Writing bad error-code-based code | Writing good error-code-based code                       | Writing good exception-based code with improper techniques |
| Writing bad exception-based code  | Writing good exception-based code with proper techniques |                                                            |

Kalb offers a lot of advice in his talk, some of which we'll look at later, but
in essence he says we must "think structurally and maintain invariants".
In order to do this we must ensure that all code either offers the "no-throw
guarantee" or the "basic exception guarantee". The no-throw guarantee is obvious,
the basic guarantee is as follows [^abrahams]:

> Basic exception safety guarantee: invariants are preserved and no resources
    are leaked.

[^abrahams]: [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library by David Abrahams](http://www.boost.org/community/exception_safety.html)

We must also assume that all code can throw unless it explicitly states
otherwise.

We've already talked about [how exceptions can strengthen invariants](/Exceptions-Part-4/),
but what's interesting about this guarantee is that _everything it says also
applies to error handling_. We are not having to to work harder, we merely have
to consider, for each point of the program, whether or not _invariants are
preserved_ or _resources are leaked_.

Chen provided a second table, which talks about how hard it is to recognise good
and bad error handling code:

| Really easy                       | Hard                                                     | Really hard                                                |
|-----------------------------------|----------------------------------------------------------|------------------------------------------------------------|
| Recognizing that error-code-based code is badly-written | Recognizing that error-code-base code is not badly-written | Recognizing that exception-based code is badly-written |
| Recognizing the difference between bad error-code-based code and not-bad error-code-based code. | | Recognizing that exception-based code is not badly-written |
| | | Recognizing the difference between bad exception-based code and not-bad exception-based code |

There are two concepts in this table, "bad code" and "not bad code". Chen's
definition of "bad" code is code that makes no attempt to handle error
conditions. His definition of "not bad" code is code that makes some attempt to
handle error conditions, even if it is not actually correct.

He also offers some examples to support his case, which in my view are
deliberately chosen to make exceptions look bad. The return code examples are
obvious, and I won't repeat them here as I'm sure you can imagine what a code
snippet that does not examine return codes looks like as compared to one that
does.

The example for exceptions is more interesting, but as I discussed in part 5
[you cannot ignore an exception](/Exceptions-Part-5/). By definition it is not
possible to write exception based code that is as bad as the worst return value
based code. It is, however, possible to make no effort to avoid leaking
resources or preserving invariants - these are the cases we need to worry about,
but remember, this is not unique to exception based code. We must always do
these things in the face of errors.

So, let's start by looking at Chen's examples in more detail. First, the "bad"
exception based code (I think it's C#):

    NotifyIcon CreateNotifyIcon()
    {
        NotifyIcon icon = new NotifyIcon();
        icon.Text = "Blah blah blah";
        icon.Visible = true;
        icon.Icon = new Icon(GetType(), "cool.ico");
        return icon;
    }

and now the "good" exception based code:

    NotifyIcon CreateNotifyIcon()
    {
        NotifyIcon icon = new NotifyIcon();
        icon.Text = "Blah blah blah";
        icon.Icon = new Icon(GetType(), "cool.ico");
        icon.Visible = true;
        return icon;
    }

Wow, ok, that's a subtle difference.
The problem is that the notification can't be made visible without a valid icon.
Here's why I think this is an unfair example.
Both cases are _just plain bad code_ and that is _obvious_.
Why? Let me list the ways:

* The `NotifyIcon` interface does not maintain the invariants, it seems that it
  is invalid for a `NotifyIcon` to exist without a valid icon - so why is it even
  permitted to be constructed in that state?
* Encapsulation. The interface exposes the "properties" as though `NotifyIcon`
  is a simple [data object](/Exceptions-Part-4/), but it isn't.
* Untested. This code will fail 100% of the time, if this made it to production
  that means it isn't tested at all.

Moreover, the error here _has nothing to do with exceptions_.
It's a programming error, if we assume this is C# then we can also assume the
garbage collector will clean up the allocations so no resources can be leaked
and `CreateNotifyIcon` does not modify any global state, or any data members, so
it cannot violate invariants.
In other words it appears to provide the basic exception guarantee _in both
cases_, which is all that is required of it.

![Venn diagram]({{ site.baseurl }}/images/vennex.png "'Bad code' is more likely to be 'code written to meet a very specific set of requirements without enough time or specification'. It always is.")

Good news though! I can provide a better example, here is some bad exception
handling code (this time in C++):

{% highlight cpp %}
{% include code/notify_icon/bad.cpp %}
{% endhighlight %}

Super! So is it obvious to you what is wrong with this code?
We've used Chen's "correct" version, so it won't throw, right?
Well hang on - we said _assume all code throws unless it specifies otherwise_ -
any of those setters could throw, and in that case we will certainly leak a
`NotifyIcon`, and possibly even an `Icon` instance.

To anyone who has been working with C++ for any reasonable part of their career
this is, however, obviously wrong.
RAII (Resource Acquisition Is Initialisation) is built into our bones, it is
our bread and butter and it _is not optional_.

Here's a better version:

{% highlight cpp %}
{% include code/notify_icon/good.cpp %}
{% endhighlight %}

The mental check is easy: are resources uniquely (and immediately) assigned to
RAII containers? If yes, it is "not-bad" code, if no, it is "bad" code.

At this point the observant will be crying out about invariants, and rightly so.
Determining if invariants will hold is by far the hardest part of meeting the
basic guarantee. I am not convinced, however, that this is much more difficult
with exceptions than it is with return codes, we must consider the points at
which the code can fail (remembering that all code can fail unless otherwise
specified), and whether the invariants still hold at that position.

Thus, I present my own classification

| Impossible                       | Easy                                                     | Hard                                                     |
|----------------------------------|----------------------------------------------------------|----------------------------------------------------------|
|                                  | Identifying code that ignores return value errors        | Identifying return value code does not leak resources    |
|                                  |                                                          | Identifying return value code maintains invariants       |
| Accidentally ignoring exceptions | Identifying exception based code that can leak resources | Determining if exception based code maintains invariants |

In this classification it is evident that what is difficult is maintaining
invariants. In the coming parts we will look at what good exception-based code
that maintains invariants looks like.
