---
layout: post
title: "On Testing Part 1: Why?"
tags:
  - testing
---

Lately I've been thinking about testing, so in this series I want to talk about
how I approach testing and some of my personal philosophy.
I intend to focus on what I believe it takes to make tests work as part of a
robust development process.

# Is this series for me?

Have you never written automated tests before?
Does Dave [^dave] frequently lament the fact that every time he adds
functionality he has to fix up the tests to actually run?
Does Brian [^brian] WTF every time he looks at someone else's test code to
see why it's failing?
Do you see a lot of regressions between software versions?

![A hang glider full of holes]({{ site.baseurl }}/images/no-testing.png "Turns out I can't draw")

If you answered "yes" to any of these, then there might be something for you
here.
If not, then congratulations - it looks like you've succeeded in making
automated testing work for you! You're probably better off ignoring what I have
to say because what works for me might not be what works for you.

[^dave]: He's probably not called Dave in your office. Might be Terry.
[^brian]: Sometimes this is also Dave.

# Why Testing?

Testing is becoming a fundamental part of software development these days.
To quote the Django tutorial, "other developers want to see tests in your
software before they take it seriously" [^seriously]. Unfortunately, being a
capable developer doesn't always mean you're capable of writing good tests.

When I started professional software development I was the sole developer for a
small company, and somewhat inexperienced.
Which means I didn't write many tests.
Which, in turn, means that the first thing I was responsible for shipping was
horribly broken.
I'm not ashamed to admit this because learning how to do things well is part of
the process of becoming an experienced developer.
As a result, something I learned the hard way early on is that testing
is really important.
I also quickly realised that automated testing is a way to minimise the burden
in a small company without any kind of dedicated test or QA.

![The push-first-fix-later approach]({{ site.baseurl }}/images/live-patching.png "Inspired by XKCD!")

So, at this point in my career I had established that testing is good.
Testing makes life better, less stressful, and gives you confidence that what
you think you're shipping is what you're actually shipping.
Soon after that I realised something else: no-one had taught me how to write
good tests and I needed to learn.

Being a good software developer is all about practice, first you write code to
learn how to write code, after a while the code comes naturally - but then you
have to learn how to write code that stands the test of time, code that other
people can work with.
This second stage takes time because the feedback cycle is so long. You might
not realise that your code is bad until months or years down the line when you,
or someone else, comes back to it.
You might not realise until several iterations after that when, coming back to
something, you see that change heaped upon change has turned what was once
beautiful into something incomprehensibly ugly.

It's the same with writing tests, unless you do it, you don't learn to do it
well.
You don't learn which tests are useful and catch bugs, and which tests are
nothing but a maintenance nightmare.
You don't learn how to structure your tests so that, when you come back to them
because they fail, you can understand what they were trying to achieve.

There are really two types of testing you can do, automated and manual testing.
Automated testing means that tests are run automatically, without user
intervention, and give you a binary PASS/FAIL result.
Manual testing means that someone sits down and physically drives the software
to see if it does what it's supposed to.

Both kinds of testing have strengths and weaknesses, but the fact is I'm a
software developer, and I'm interested in how we go about using automated
testing to make software better.
I'm hoping that by writing down some of things I've learned, I might make that
learning phase a bit less painful for someone else.
I'm sure you're super excited to read more, but that's it until the next post
when I'm going to introduce the first theme of the series - "The Test Pyramid".

[^seriously]: [Writing your first Django app, part 5](https://docs.djangoproject.com/en/1.9/intro/tutorial05/)
