---
layout: post
title: "Slicing: A Practical Example"
tags:
  - agile
  - slicing
---

I guarantee that the first time you try vertical splitting with a team someone
will say "some stories can't be split". There is a kernel of truth in this
statement (it is obviously the case that there will be some level below which
a story cannot be split any further) that makes it easy to say - but the truth
is a team splitting stories almost never reaches this limit; you have to push
through your instinctive reluctance to split vertically until it starts to
become natural.

### Merging organisations

Our example is taken from my current team's work (slightly simplified and
paraphrased):

> As a corporate administrator I would like to be able to merge duplicate
  organisations through the online portal so that I can report on the
  organisation correctly.
>
>  * Test that a list of all organisations can be viewed
>  * Test that the list of organisations loads in under a second with 1,000
>    organisations
>  * Test that when an organisation is deleted through the portal it is possible
>    to select an organisation to migrate all existing records to
>  * Test that only users with the manage organisations permission can delete
>    organisations
>  * Test that sites cannot see deleted organisations
>  * Test that site records are updated to reflect the merged organisation
>  * Test that sites on old versions continue to work correctly even when
>    connected to an updated online portal
>  * Test that when a site tries to save a record with a deleted organisation,
>    it is automatically migrated to the new organisation

The basic idea is pretty simple, we have a big list of organisations in our
database, and we would like to merge some together to get rid of duplicates. We
have a bunch of "test cases" that cover some behaviour we would like it to have
- in particular there are requirements around making sure the updated
information synchronises correctly.

The first estimate for this story was pretty high, either 40 or 100 points and
we know that those don't really fit into a single sprint - so we had to split
it, but where? If you just look at the title of the story splitting it can be
a bit difficult, how can you deliver "part" of the merge functionality and have
it be useful?

Let's apply [the patterns](/Agile-Part-7-Slicing/)!

* Is it a workflow? Nope - one button, one operation.
* Are there multiple operations? No...wait...not in the title, but looking at
  the test cases I can see "Test that a list of all organisations can be viewed"
  and "Test that when an organisation is deleted through the portal it is
  possible to select an organisation to migrate all existing records to". That's
  two operations! Hint: writing out your requirements like this makes splitting
  a lot easier!
  * We can split into adding a view to see the organisation list and the merge
    operation.
  * We can split the merge and delete operations.
* Are there business rules we can tweak?
  * We could split out the permission requirement.
  * We could split out the migration requirement.
* Are there any data variations to consider?
  * We could defer making it work with old versions.
* Are there any interface variations?
  * We could perhaps come up with a simpler select/delete/merge UI that doesn't
    need the entire list of organisations and therefore avoid the performance
    requirement.
* Is there an obvious split? No.
* Is there a simple core? No.
* Can we defer performance? Yes!
  * We can defer the requirement to have the organisation list load in under a
    second.
* Do we have any unknown quantities that need a research spike?
  * The technical details of how to perform the merge and maintain backward
    compatibility could do with some examination.

So, we've got some splits - we need to try to find ones that will let us devalue
one story. It turns out that the only split we can apply that truly devalues
work is the permission requirement (because we suspect our sales and support
teams will end up doing most of the migration, we can just restrict the
functionality to administrators.

However, we can still use the splits to get useful _incremental_ work. We ended
up splitting the story like this:

* Implement listing organisations on the portal.
* Implement deleting organisations through the portal.
* Implement migrating organisations after deletion.
* Implement automatic correction of data synchronised from older versions.
* Add configurable permission controls to organisation management.

The keen eyed among you might have spotted that there appear to be dependencies
between these stories - but there's a way of thinking about this that helps
explain why this ends up being acceptable. Steven Thomas writes about how
dependencies can be more apparent than real [^deps]. The core idea is that if
one of these stories had _value_ without the others we could just go ahead and
implement it. Take two obviously related stories - "Implement migrating
organisations after deletion" and "Implement automatic correction of data
synchronised from older versions". We can't implement the correction without the
migration, right? Wrong! Of course we could, if there were some way that old
versions could be syncing bad data (perhaps if we were manually performing
merges at the database level), then this story would have value without the
interface for generating the migrations. Thomas calls this kind of dependency a
"natural order" for the stories - but the reality is that we can implement them in
whatever order we want, it just so happens that, once implemented, some stories
can increase the value of other stories.

[^deps]: [User Story Dependencies are more Apparent than Real by Steven Thomas](http://itsadeliverything.com/user-story-dependencies-are-more-apparent-than-real)

Having done this split it turned out some of these stories were _still_ too big.
We could perhaps go further, for example with the migration we could have an
interface for creating a list of migrations and a separate story to _perform_
the migrations.
Again, the act of performing migrations only depends on a source of migrations -
that source does not have to be a user interface!
When you have gotten as far as you can splitting a story and it's still too big
the last question to ask is "why?" - chances are there's some [technical debt](/Refactoring/)
in your system that's holding your team back from delivering more value to your
customers. As slow accumulation of debt is extremely likely to become a
[normalisation of deviance](http://lmcontheline.blogspot.co.uk/2013/01/the-normalization-of-deviance-if-it-can.html),
when your stories are too big to implement and impossible to split then there's
a good chance your system is the problem. Generating and estimating subtasks
might be one way to try and pick apart what it is about a story that is complex
or debt ridden - it may even reveal vertical splits you hadn't considered. If
debt is the problem (perhaps one component is very complex, and integrating the
new feature with it is a lot of work) your [sustainable pace](/Agile-Part-1/)
has been compromised, and you need to take action to change that!
