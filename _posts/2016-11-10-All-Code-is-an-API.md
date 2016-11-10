---
layout: post
title: "All Code is an API"
tags:
  - conventions
---
Many years ago I read an article whose key premise was that all code is in one
way or another an API.
The original article is lost in the depths of the Internet, and I can't for the
life of me find it (leave a comment if you do manage to find it, I'd be
interested in re-reading it!).
The article changed how I thought about the code I was writing, so I thought it
would be nice to examine the idea here.

An Application Programming Interface (API) is a defined mechanism for
programmatically controlling the behaviour of some component.
This term is most often used to describe methods by which third parties can
integrate with another application - for example [Facebook](https://www.facebook.com)
provide an API through which third parties can build applications on top of
their platform, [Slack](https://slack.com) expose one that lets you post
messages, write bots and more.
At [Pattern Analytics](http://patternanalytics.co.uk/) we provided APIs for the
various libraries, and at [Allinea](http://www.allinea.com) we had an API for
adding custom performance metrics to the MAP profiler.
As a programmer, APIs are incredibly useful - anything I can't do natively in
[JIRA](https://www.atlassian.com/software/jira) I know I can probably get to
work with a bit of [Python](https://www.python.org/) or
[Node.js](https://nodejs.org/en/).
When [Rhodecode](https://rhodecode.com/) was lacking a view that let me see all
open pull requests (even ones I wasn't a reviewer on) a few lines of HTML and
[AngularJS](https://angularjs.org/) made one (although it isn't the prettiest
thing I've ever done).
The theme here is that if your product is targeted at developers, you sure as
hell need an API if you want them to be invested in your solution.

So what makes an API different from normal code?
It's really only two things, firstly, APIs should be well documented interfaces
to some underlying logic.
They usually form explicit contracts with their users about how they can be
used and how they will behave.
Secondly, it is often not possible to look at the implementations - and even
when it is possible, it may not be _practical_ to look into the implementation in
order to simply _use_ the API.
In even more cases it is either not possible or not practical to _change_ the
implementation.

## Everything is an API

On any reasonably sized project, no matter how many developers there are, every
piece of code is an API.
Every module, class, or function is a piece of code that a developer might want
to reuse in the future, so it needs to be documented, otherwise the developer
will be wasting their time digging through the internals to figure out how to
avoid duplicating the code.
In many cases, the developer might find it easier to just re-write the code
(assuming they even realise the undocumented code exists!)
Looking at the implementation is impractical - so we need documentation, but
what about change?
Well, every piece of code in the system might well be used somewhere, so if we
change a piece of code we need to make sure we preserve the contracts otherwise
we run the risk of breaking dependent code elsewhere in the system.

So, in practice, every trait we want of an API we probably want in normal code.
If you're writing an API you probably put a reasonable amount of thought into
your design, because once it's in the wild you can't take it back (and because
you probably have to support the thing) - if we want all the same traits in
normal code, then we should be putting the same amount of thought into our
internal modules, classes and functions.

## What should an API be?

So what should we be thinking about when we're designing an API?
The Qt project has a nice set of [API design principles](https://wiki.qt.io/API_Design_Principles)
that I think everyone should know.

* *Minimal:* a minimal API exposes only the minimum number of functions and
  classes. It makes it easier to use because there is less to remember. It is
  tied to the [Single Responsibility Principle](https://en.wikipedia.org/wiki/Single_responsibility_principle)
  and the idea that the component should only do one thing.
* *Complete:* a complete API exposes everything a user would _expect_ it to provide,
  and is about ensuring the component is usable in a variety of contexts.
* *Clear and simple semantics:* The [Principle of Least Astonishment](https://en.wikipedia.org/wiki/Principle_of_least_astonishment)
  is a simple idea that prevents nasty surprises for users of your API. Common
  tasks need to be easy (I'm looking at _you_ [Boost date-time library string formatting](http://www.boost.org/doc/libs/1_61_0/doc/html/date_time/date_time_io.html#date_time.io_objects),
  and unexpected side effects should be avoided.
* *Intuitive:* an intuitive component is one that is _easy to use correctly_
  and _difficult to use incorrectly_. As a guideline, imagine trying to figure
  out how to use the component for common tasks _without_ looking at the
  documentation or the implementation. A good API is one where an intermediate
  programmer can get away with this.
* *Easy to memorise:* no-one wants to constantly cross reference documentation
  when using a library to remember common tasks. A good library has strong
  naming conventions to ease the cognitive burden, and shares as much as it
  can with recognisable patterns and concepts (as an example, we have a naming
  convention where any function that does something analogous to a standard
  library function will have the same name).
* *Readable:* some people might not believe in self documenting code, but APIs
  should strive to be so. Even worse than having to reference documentation to
  _write_ code is having to reference documentation to _read_ it. What code does
  needs to be obvious from the names of the classes and functions alone. A
  classic example is avoiding the use of boolean function arguments, because it
  is often necessary to read the documentation to figure out what `true` and
  `false` actually mean.

It's easy to see that all of the above make for better code, so the sooner we
start thinking about all code we write as an API for someone else the higher
quality our code bases will be.
