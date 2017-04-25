---
layout: post
title: "The Path to Agility Part 9: Our Retro"
tags:
  - agile
---

In my last post I rambled on about how important retrospectives are. This blog
is meant to be a real story of our adoption of agile, so this time I want to
write about one of our actual retrospectives - and what I learned from it.

### Setting the stage

I opened the retrospective with a summary of the stories we got done, and those
we didn't. The main aim was to help refresh everyone's memory about what they've
worked on, although so far I'm not sure this little opening has quite achieved
the right result - in the future I might stick with something more brief to
avoid spending the time unproductively.

Next, I gave everyone one minute to come up with one word to describe their
sprint. This forces everyone to think back over what happened in the sprint and
reduce their overall impression to a single word. Going round the room and
getting everyone to read out their word encourages the whole team to participate
in the retrospective. To save time I could have done this activity without the
one minute thinking time, but my worry with that is that instead of
listening, everyone who hadn't spoken would instead be thinking of their word.

![[a-z]+]({{ site.baseurl }}/images/one-word.png "The XKCD font is not case sensitive...")

I'm pretty happy with how this turned out, and I was pleased only one person
used "frustrating". Even though the activity got a few funny looks when I
described it, everyone managed to contribute. Having collected these answers I
didn't do anything with them, instead my hope was that the team would carry
forward their thoughts to the _gathering data_ phase.

Incidentally, our team structure lends itself to my participating in the
retrospectives. While this goes very much against my role as a facilitator it's
also difficult to convince the team to take part in these weird activities if
I don't do so myself. My word was "opaque", because I felt I wasn't sufficiently
engaged with the sprint to know what was going on throughout it.

### Gather data

We didn't have any particular focus this retrospective, so "gathering data" is a
pretty important phase - it's where we'll figure out what areas need addressing.
To do this I used an activity called "three wishes", I gave everyone 5 minutes
to come up with (and write down, on individual cards), three wishes:

* one for themselves,
* one for the team, and,
* one for the company.

Wishing for more wishes is cheating (these are engineers, and working around
limitations is basically in the job description so you've gotta watch them!)

By-and-large this worked _ok_, I got the obvious questions about wishing for
faster cars and bigger houses - but for the most part we managed to keep the
wishes "on topic" as it were. In hindsight, this is where failing to set a focus
for the retrospective caused problems. Without the focus, it is difficult to
limit the responses in activities like this to ones that are immediately useful.

After the time was up everyone read out their wishes, which in turn gave us a
whole load of useful data on what was frustrating the team. Having got that data
we needed to focus it down to a more manageable size - so we used dot voting
to figure out which "wishes" were most widely appreciated. To dot vote you
basically give every participant a fixed number of dots (I used three), and ask
them to place them wherever they wish.

### Generate insights

To generate insights I split the team up into groups of three, I asked each
group to pick three highly voted cards and discuss why they think it came up
and what they think we can do about it. I deliberately tried to create groups
of people who don't normally work together to allow ideas to spread to the
whole team and build relationships. I gave everyone 15 minutes for this.

I don't know what the name is for this activity, but it was probably my
favourite. There was a lot of productive discussion between the various team
members, and it was really nice to see even the quieter and newer team members
participating. Having seen this I'm a lot more likely to pick activities that
break up the team a little and help to avoid the one-speaking-eight-listening
(if you're lucky...) mode that feels like it could be more productive (let's
face it, anyone who's done a computer science degree is familiar with how
merge-sort is a better algorithm than bubble sort!)

### Decide what to do

The tricky part with my insights activity was turning the productive discussion
into real actions. For this I asked each group to come up with an action for
each of their cards; I gave them 10 minutes.

I then got the groups to read out their actions, and we dot voted on the ones
we were going to take. I took the top three actions, and asked for an
ambassador who would try to ensure that we followed through on it. Our actions
were:

- Replace one of our ageing servers (or rather, come up with a specification for
  the replacement so that we could get one purchased).
- Document a particularly tricky part of our system architecture.
- Write more (or make it easier to) end-to-end tests.

Looking back I can see problems with these actions, firstly we didn't allocate
time for them in the next sprint - this made them difficult to follow through
on. Secondly, we didn't find a way to measure the effectiveness or set any
limits on the activities to help the ambassadors focus the effort. At the time
it felt like the group was starting to get tired and the quality of discussion
was starting to dip - which is something I will have to look out for and address
in the future.

### Close the retrospective

I closed the retrospective with a simple activity - I asked everyone for

- One thing that pleased or surprised them this retrospective
- One person they would like to acknowledge for something they did this
  iteration.

A lot of the team struggled to name something that pleased or surprised them,
but one memorable comment was that someone was "surprised that they enjoyed a
retrospective". That's a really nice comment to hear, and a strong indication
that changing our retrospective format was long overdue.

I really enjoyed the round of thanks, most people could name someone who had
done something they appreciated this sprint - and I think creating opportunities
for this kind of praise is really good for overall morale.

As much as I liked this activity, it's not great at delivering useful feedback
on the retrospective itself. I'd use it again, but at some point will need to
change it to make sure our retrospectives are also continuously improving.

This retrospective was basically extracted from [plans-for-retrospectives.com](https://plans-for-retrospectives.com/),
but before you go there, hit random, and run your retrospective ten minutes
later I suggest making sure the activities gel. In this retrospective I tried
quite hard to make sure each activity acted as an input to the next. It was by
no means perfect, but each activity needs to build on the previous one otherwise
you're basically just messing around for no reason. Productive retrospectives
need to be tightly focussed on delivering value (actually, this is true of  any
kind of meeting - something more people would do well to remember) so every
activity needs to contribute something. Please also learn from my mistakes,
set a focus to keep people on task, find ways to measure the outcomes of your
actions and make sure to allocate time for them!
