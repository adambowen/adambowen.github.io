---
layout: post
title: "Writing a Better Options Library"
tags:
  - options
---

Something I find myself doing often is writing small tools that need to take
various command line arguments.
Many years ago, when I first encountered this problem, I investigated the
options (pun intended) and decided I didn't like any of them - so I wrote
my own [^woptions], which was eventually open-sourced by
[Warwick Warp](http://www.warwickwarp.com).
At [Pattern Analytics](http://www.patternanalytics.co.uk) we again wrote a
similar library.
When I started at [Allinea](http://www.allinea.com) I discovered that their
internal options library shared some of the concepts I had used previously, I
thought it was interesting that people hit on the same problems and work through
similar solutions - so I thought I'd write a series about it.

[^woptions]: This used to be on Google code, you can find it in the [archive](https://code.google.com/archive/p/woptions/).

### What would we like in an options library?

We can start with the [API guidelines](/All-Code-is-an-API]), because this is
going to be an API that other programmers use.

* Minimal
* Complete
* Clear and simple semantics
* Intuitive
* Easy to memorise
* Readable

For minimal we want to only cover aspects relating to the parsing of command
line options, while for completeness we want to ensure we cover all the common
use cases and have an interface powerful enough to cope with the more esoteric
use cases.

We certainly want an easy-to-memorise interface, as an options library is
something likely to be used infrequently compared to the rest of an application,
and we will also need it to be readable so that someone looking over the code
does not need to be familiar with the library to figure out the options it
takes and how they are represented in the code.

There are also a few other aspects to consider. We would like:

* Minimal dependencies
* Broad compiler support
* Permissively licensed
* Header only

An options library has a broad range of use cases, from simple sample programs
all the way to full applications. We do not want to limit these use cases by
imposing unnecessary restrictions on the contexts in which the library is
supported. Thus, we want a permissive licence that allows us to freely
distribute programs that use the library and we want to avoid decisions that
greatly restrict the supported platforms or compilers. Finally, header only
libraries are the easiest to integrate with existing build systems - although
we may want to relax this requirement if we find it to be overly restrictive
on our implementation.

We're going to write our library in C++, so we would also like:

* Type safety
* C++11 (and beyond) support where available

### What else is out there?

[NIH'ing ]() solutions is not usually a good idea if something already exists
that can meet your needs - so as a first step we should look at the existing
libraries and see if they meet our needs and if not, why not. We can also look
for inspiration for the sort of interface we might expose and features we could
support.

#### Boost Program Options

	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help", "produce help message")
	    ("compression", po::value<int>(), "set compression level")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(ac, av, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
	    cout << desc << "\n";
	    return 1;
	}

	if (vm.count("compression")) {
	    cout << "Compression level was set to "
	         << vm["compression"].as<int>() << ".\n";
	} else {
	    cout << "Compression level was not set.\n";
	}

By-and-large this isn't terrible, it it's also not the prettiest syntactically.
Having to call `store`, `parse_command_line` and `notify` seems cumbersome - are
we really going to use the library often enough that we will remember those
three steps?
It is not even obvious why we need `notify`, or what `store` does that
`parse_command_line` couldn't.
If you dig further into the library there seem to be a lot of syntactic quirks
that are not the sort of thing I look for in a clean interface.
Finally, although the `value<int>` and `as<int>` allows us to be type safe it's
not a [DRY](https://en.wikipedia.org/wiki/Don't_repeat_yourself) interface, and
mismatches throw at runtime instead of compile time.

On the other hand, [Boost Program Options](http://www.boost.org/doc/libs/1_62_0/doc/html/program_options.html)
has a permissive licence and broad compiler support.
The library isn't header only, but it doesn't (appear to) depend on much from
the rest of boost so the dependencies are at least minimal.

### The Lean Mean C++ Option Parser
	#include <iostream>
	#include "optionparser.h"

	enum  optionIndex { UNKNOWN, HELP, PLUS };
	const option::Descriptor usage[] =
	{
	  {UNKNOWN, 0,"" , ""    ,option::Arg::None, "USAGE: example [options]\n\n"
	                                             "Options:" },
	  {HELP,    0,"" , "help",option::Arg::None, "  --help  \tPrint usage and exit." },
	  {PLUS,    0,"p", "plus",option::Arg::None, "  --plus, -p  \tIncrement count." },
	  {UNKNOWN, 0,"" ,  ""   ,option::Arg::None, "\nExamples:\n"
	                                             "  example --unknown -- --this_is_no_option\n"
	                                             "  example -unk --plus -ppp file1 file2\n" },
	  {0,0,0,0,0,0}
	};

	int main(int argc, char* argv[])
	{
	  argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
	  option::Stats  stats(usage, argc, argv);
	  option::Option options[stats.options_max], buffer[stats.buffer_max];
	  option::Parser parse(usage, argc, argv, options, buffer);

	  if (parse.error())
	    return 1;

	  if (options[HELP] || argc == 0) {
	    option::printUsage(std::cout, usage);
	    return 0;
	  }

	  std::cout << "--plus count: " <<
	    options[PLUS].count() << "\n";

	  for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
	    std::cout << "Unknown option: " << opt->name << "\n";

	  for (int i = 0; i < parse.nonOptionsCount(); ++i)
	    std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";
	}

Yeah. That's not pretty. It is header only though, with zero dependencies.
[The Lean Mean C++ Option Parser](http://optionparser.sourceforge.net/) looks
very much like C++ written with C in mind - in our library we would prefer a
more modern and natural syntax.

### TCLAP

	#include <string>
	#include <iostream>
	#include <algorithm>
	#include <tclap/CmdLine.h>

	int main(int argc, char** argv)
	{

		// Wrap everything in a try block.  Do this every time,
		// because exceptions will be thrown for problems.
		try {

		// Define the command line object, and insert a message
		// that describes the program. The "Command description message"
		// is printed last in the help text. The second argument is the
		// delimiter (usually space) and the last one is the version number.
		// The CmdLine object parses the argv array based on the Arg objects
		// that it contains.
		TCLAP::CmdLine cmd("Command description message", ' ', "0.9");

		// Define a value argument and add it to the command line.
		// A value arg defines a flag and a type of value that it expects,
		// such as "-n Bishop".
		TCLAP::ValueArg<std::string> nameArg("n","name","Name to print",true,"homer","string");

		// Add the argument nameArg to the CmdLine object. The CmdLine object
		// uses this Arg to parse the command line.
		cmd.add( nameArg );

		// Define a switch and add it to the command line.
		// A switch arg is a boolean argument and only defines a flag that
		// indicates true or false.  In this example the SwitchArg adds itself
		// to the CmdLine object as part of the constructor.  This eliminates
		// the need to call the cmd.add() method.  All args have support in
		// their constructors to add themselves directly to the CmdLine object.
		// It doesn't matter which idiom you choose, they accomplish the same thing.
		TCLAP::SwitchArg reverseSwitch("r","reverse","Print name backwards", cmd, false);

		// Parse the argv array.
		cmd.parse( argc, argv );

		// Get the value parsed by each arg.
		std::string name = nameArg.getValue();
		bool reverseName = reverseSwitch.getValue();

		// Do what you intend.
		if ( reverseName )
		{
			std::reverse(name.begin(),name.end());
			std::cout << "My name (spelled backwards) is: " << name << std::endl;
		}
		else
			std::cout << "My name is: " << name << std::endl;


		} catch (TCLAP::ArgException &e)  // catch any exceptions
		{ std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }
	}

Again, [TCLAP](http://tclap.sourceforge.net/) is not pretty, but with a
permissive licence and header only it at least meets some of our requirements.

If any of these samples look reasonable to you, then by all means grab those
libraries - but for now we'll look for inspiration in some other languages.

#### Python argsparser

	import argparse

	parser = argparse.ArgumentParser(description='Process some integers.')
	parser.add_argument('integers', metavar='N', type=int, nargs='+',
	                    help='an integer for the accumulator')
	parser.add_argument('--sum', dest='accumulate', action='store_const',
	                    const=sum, default=max,
	                    help='sum the integers (default: find the max)')

	args = parser.parse_args()
	print(args.accumulate(args.integers))

Unlike C++, Python doesn't offer much in the way of type safety - but the
relaxation of certain requirements, combined with named arguments, make for a
very readable interface. We define our arguments, we parse our arguments, we
use our arguments.

`boost::program_options` is similar, but Python's [argparse](https://docs.python.org/3/library/argparse.html)
library really reduces the amount of non-essential code to the bear minimum.
The naming of the various argument parameters is also essential to the
readability - but I would also argue that some of the actions can be difficult
to remember and use.

#### Node yargs

	#!/usr/bin/env node

	require('yargs')
	  .usage('$0 <cmd> [args]')
	  .command('hello [name]', 'welcome ter yargs!', {
	    name: {
	      default: 'default name'
	    }
	  }, function (argv) {
	    console.log('hello', argv.name, 'welcome to yargs!')
	  })
	  .help()
	  .argv

The beautiful thing about [yargs](http://yargs.js.org/) is that if you don't
need any special behaviour you don't need to remember anything other than that
the library is called yargs [^yargs], importing the library is enough to have it parse
the arguments and assign them to a hash:

	#!/usr/bin/env node
	var argv = require('yargs').argv;

	if (argv.ships > 3 && argv.distance < 53.5) {
	    console.log('Plunder more riffiwobbles!');
	} else {
	    console.log('Retreat from the xupptumblers!');
	}

the downside is, of course, that the default behaviour offers nothing in the
way of validation and can't generate sensible help messages.

None-the-less, it's worth keeping in mind that an interface this simple would
automatically meet many of the requirements we set out with.

[^yargs]: And who doesn't like pirate references littered throughout their code?

### Dramatic Fade to Black

This post has mostly been setting the stage for developing an options library,
something I'll delve into in future posts in this series.

![What, that's it?]({{ site.baseurl }}/images/thatsit.png "This is worse than the time Netflix released Designated Survivor one episode at a time and I couldn't binge the entire series.")
