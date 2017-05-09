---
layout: post
title: "The Path to Agility Part 11: Yet Another Retro"
tags:
  - agile
---

More retrospective! This time I wrote up [my retrospective plan]({{ site.baseurl }}/docs/Retrospective27417Plan.pdf)
before the retrospective. Read on to find out how it went...

### Setting the stage

This time I didn't go through the sprint before we began, in the last two
retrospectives I've written up I said I felt that going through the content
of the sprint in a rather rote fashion was dry, unengaging and ultimately
unhelpful. No-one noticed, and I don't think the retrospective was any worse
for it.

I also set a focus - we did a more in depth planning than normal for this sprint
because we had some tricky commitments, and I wanted to make sure we had a plan
to meet them. Since we did something a bit different, I thought it would make a
nice focus. This part I don't think I got right, there wasn't a noticeable
change in the type of information we gathered, the insights, or the actions. I
didn't steer the retrospective back toward the focus because there was nothing
wrong with the discussions the team were having and - crucially - I the team
should _own_ the retrospective - if they don't agree with my focus that's on _me_,
not on them.

Finally, I used the "one word" activity to get everyone involved. Hyphenation
became popular this time, so in the future I think allowing up to three words
would be helpful. I wrote all the words on a board, as this time I decided to
follow each activity with Derby and Larson's [^agileretro] debriefing
questions. Upon starting the debriefing, I immediately hit a problem - it wasn't
clear whether we were trying to analyse the activity, or it's content. I'm
pretty sure the intent is to debrief the content, but the team (and - initially
- myself) found this confusing. I forgot to take a picture of the words.

[^agileretro]: [Agile Retrospectives: Making Good Teams Great by Esther Derby and Diana Larson](https://www.amazon.co.uk/gp/product/0977616649/ref=as_li_qf_sp_asin_il_tl?ie=UTF8&camp=1634&creative=6738&creativeASIN=0977616649&linkCode=as2&tag=deuscouk-21)

### Gather data

To gather data I used the [timeline](/Agile-Part-5/) activity again, only this
time I just asked for facts and not feelings. I find the timeline activity
regularly useful, I often see something on there that I wasn't aware of because
my main interaction with the team is during the daily stand-ups. This time was
no exception as several people mentioned changing requirements.

![picture of our timeline]({{ site.baseurl }}/images/agile11-timeline.jpg "One of the few times you'll see someone sad to encounter a bank holiday weekend!")

### Generate insights

For this step we used "learning matrix". The learning matrix consists of four
quadrants, labelled :), :(, idea, and appreciate. The idea is to get the team
to examine the data on the timeline and add insights to the quadrants.

![our learning matrix]({{ site.baseurl }}/images/agile11-matrix.jpg "One of the few times you'll see someone sad to encounter a bank holiday weekend!")

Looking back, the end result was decent - we managed to get a good set of ideas,
good, and bad points - and they were a good distillation of the timeline.
However, there were a couple of problems. Firstly, it took a while to get going,
as the team gets used to the activity I expect it will get better - but this
first time there was a long period of silence. Secondly, there were a lot of
appreciations - I like giving the team an opportunity to recognise the things
they do for one another, but in order for the activity to be effective we do
need a balance between the four quadrants.

Having written some thoughts in each quadrants we then used "dot voting" to
pick out items we thought worthy of action. This immediately leads to a third
problem - two quadrants aren't generating useful insights to take forward. It's
fairly obvious that the :) and appreciate quadrants have other benefits, but
toward the end of this activity we need to make sure we bring the focus back to
things we can act on.

![The Iron Throne]({{ site.baseurl }}/images/throne.png "This was an actual question.")


### Decide what to do

As I've mentioned in previous retrospectives, I'd noticed a trend to not follow
through on retrospective actions - so this time I used an activity designed to
generate SMART goals. SMART goals are intended to be easier to understand and
measure than less well defined objectives, in turn this is supposed to make
them easier to follow through on. The acronym stands for:

* Specific
* Measurable
* Attainable
* Relevant
* Timely

For this activity I split the team into groups, and asked each group to create
a SMART objective for one of the most highly voted items on our learning matrix.

This activity was partially successful, with the success dictated mostly by how
well the teams spent their time creating their objectives - one of the teams
finished fairly quickly, and gave a reasonable objective, while one of the teams
spent a lot of time discussing the problem without creating a firm objective.

I think if I were going to do this activity again I would try to structure the
creation of the goals a bit more to help those unfamiliar with goal setting get
started.

I don't have a picture, but here are a couple of the goals we set

> At the start of each sprint someone on the team will review each of the
  requirements of the stories in the sprint and clarify any that are not clear
  or up to date. We will count the number of requirements that change during
  the sprint and revisit this activity in 2 sprints time.

> Prior to each sprint, someone from the test team will review the requirements
  for the stories planned for the next sprint. They will use their knowledge to
  identify any missing requirements and add them to the stories. We will count
  the number of requirements that are missing during the sprint and revisit this
  activity in 2 sprints time.

Both of these activities centre around requirements changing during the sprint,
of course, one of the Agile principles is that we "welcome changing requirements,
even late in development" but we are concerned with identifying _easily avoidable_
changes in the requirements. The first activity centres around when our
requirements have not been updated - often there is discussion about stories
during the planning meeting, and some of this discussion doesn't get written
up. This first objective compels the team to spend some time making sure the
stories are clear and up to date.

The second objective comes from problems identified during testing,
we [test during the sprint](/Testing-Part-5/), with the test team reviewing
requirements early in the sprint and testing development work as soon as it is
in a suitable state - however, we have found that the test team are often adding
requirements once they begin testing, indicating that we could be doing a better
job of identifying corner cases and other problems earlier in the development of
a story. This objective compels the team to spend some time seeking missing
requirements before the sprint has been planned.

### Close the retrospective

To close the retrospective I used a different activity this time, I used
"return on time invested" to gauge how well I had used the team's time.

![ROTI chart]({{ site.baseurl }}/images/agile11-roti.jpg "So...you've not labelled your axis, 0 is not at the origin, and that scale makes very little sense if the chart is meant to be showing the relationship between time invested and benefit")

The basic idea is to ask the team how well they think their time was spent, and
then ask why they chose the answers they did. I was well over time at this point,
so I kept it fairly short. For the most part the team seemed to feel like the
time was moderately well spent, with a few individuals deriving little benefit
(mainly because of how their roles interact with the development team).
