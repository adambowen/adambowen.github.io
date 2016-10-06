---
layout: post
title: "The Case for Exceptions Part 5: Ignorance is not Bliss"
tags:
  - exceptions
---

Before I start looking at the drawbacks of exceptions, I want to go over some
of the other benefits. To illustrate these benefits I'm going to use a simple
function.

For this example, imagine we are developing some kind of client/server
application.
For security purposes we want to verify some data the server has sent us, and
we might choose to do this by using some kind of public/private key encryption.
Client side, therefore, we will need a function to get the public key.
In the first version we could implement the function something like this:

{% highlight cpp %}
{% include code/public_key/naive.cpp %}
{% endhighlight %}

This is a slightly contrived example, and for now we'll ignore the fact that
this might result in `std::bad_alloc` being thrown, but it should serve the
purpose I intend in this post.
We'll also ignore that this function _could_ have been designed to just return
the string.
The code that uses this function is obvious, it would look something like this:

{% highlight cpp %}
std::string public_key;
get_public_key(public_key);
// verify the data using the key...
{% endhighlight %}

I'm a big believer in Agile, one of the beliefs of Agile is that it's impossible
to know all the requirements up front.
When the code above was written, it was envisaged that there would be one
instance of the application sold in a SaaS (software-as-a-service) style,
however, it's easy to imagine that the requirements change and we need to run
multiple instances of the application.
Since we don't want the servers to share keys, we need to modify this function
to read the key from some configuration instead of hard coding it [^hardcoded].

[^hardcoded]: Obviously we should never have hard-coded the key in the first
    place, but firstly no-one writes perfect code all the time, and secondly the
    point I'm trying to illustrate is that the function changes from one that
    can never fail to one that can fail!

Without exceptions the code might look something like this:

{% highlight cpp %}
{% include code/public_key/file.cpp %}
{% endhighlight %}

The error checking on the key is a bit basic, but the key difference between
this implementation and the old one is that it can now fail.
Because this function fail where it didn't before we must audit all the places
where the function was called to ensure that they handle the error condition:

{% highlight cpp %}
std::string public_key;
if (get_public_key(public_key) != no_error)
{
    report_bad_key_error();
    return false; // data is invalid
}
// verify the data using the key...
{% endhighlight %}

So why would exceptions make this better?
In both cases (with and without exceptions), we probably need to audit the
places where the function is used to make sure the error will be handled
correctly.
We may even need to make changes in both cases (although I would argue that
it will _always_ be necessary to modify calls to this function when using
return values, but only _sometimes_ be necessary with exceptions).

The difference is what happens when we accidentally forget to handle the error,
and this is just one example of how this accidental omission might come about.
The difference is that _it is impossible to accidentally ignore an exception_.
Ignoring an exception must be a deliberate act, and is one that is easy to spot
in code review.
Ignoring a return value can be accidental, and can be easy to _miss_ in code
review.

![One guy with a bomb behind him, and one guy with a throwing a bomb for someone else to catch.]({{ site.baseurl }}/images/ignorance.png "Stunts are performed by trained professionals, please do not attempt to recreate anything you see in my doodles.")

The effect of ignoring the exception can be catastrophic for the program, it
could result in a thread dying unexpectedly or the program aborting - these are
obviously bad; but in my view preferable to the program blundering on in a
potentially ill defined state.
At the very least, with a few basic practices, it is possible to write handlers
that will log what has happened to enable you to diagnose the error easily after
the fact.
Anyone who's written a shell script knows it's often preferable to stop when you
encounter unexpected errors, and I believe the same is true of software in
general.

There is another benefit that exceptions offer in this case.
With exceptions it is possible to propagate detailed information about the
error up the call stack.
In the above example, we could propagate the result of `get_public_key_filename`
to the site that reports the error to improve the quality of the error message
shown to the user.

{% highlight cpp %}
std::ifstream in;
std::string key_file = get_public_key_filename();
in.open(key_file);
if (!in.is_open())
{
    throw key_file_not_found_error(key_file);
}
{% endhighlight %}

{% highlight cpp %}
try
{
    std::string public_key;
    get_public_key(public_key);
    // verify the data using the key...
}
catch(const key_file_not_found_error& error)
{
    report_bad_key_error(error.filename());
    return false; // data is invalid
}
{% endhighlight %}

This leads us fairly naturally onto a wider discussion about the power of
exceptions to communicate information about the error.
In the above example we are throwing a `key_file_not_found_error`, but the site
that handles the error might not care exactly what's wrong with the key, maybe
it just prints "invalid key <filename>".
If we're interested in handling the error, but not the specific error, then we
can use inheritance to express the idea that specific errors are specialisations
of a more generic error class, for example:

{% highlight cpp %}
struct bad_key_file_error : std::exception
{
    explicit bad_key_file_error(const std::string& filename);
    const std::string filename() const;
};

struct key_file_not_found_error : bad_key_file_error
{
    explicit key_file_not_found_error(const std::string& filename);
};
{% endhighlight %}

now we can just catch `bad_key_file_error` and know that we will handle all
errors that relate to a bad key file, even if they fall into a more specific
class.

{% highlight cpp %}
try
{
    std::string public_key;
    get_public_key(public_key);
    // verify the data using the key...
}
catch(const bad_key_file_error& error)
{
    report_bad_key_error(error.filename());
    return false; // data is invalid
}
{% endhighlight %}

This also means that we can mix errors from difference sources, for example, we
could use `std::ios::exceptions` to set the `ifstream` to throw on error, and
propagate `std::ios_base::failure` exceptions, while still maintaining the
ability to propagate our own error exceptions for invalid key formats.
Using return values we would need to translate errors at every stage into a
"unified" error code in order to avoid losing information about the type of
error that occurred.

With exceptions, functions that do not need to handle exceptions don't need to
do anything special in order to report errors to higher levels of the stack.
This is important because as we discover new ways a function can fail, we can
add new types to represent those exceptions without having to modify any code
responsible for propagating the error.

Another benefit is that exceptions allow us to use the return value as
intended, to return the result of the function.
If we use exceptions for error handling, then we can just have `get_public_key`
return the key directly, instead of through an output parameter.
This allows us to simplify the code above to ensure that the `public_key`
variable is _at all times initialised with a valid key_.

{% highlight cpp %}
try
{
    std::string public_key = get_public_key(public_key);
    // verify the data using the key...
}
catch(const bad_key_file_error& error)
{
    report_bad_key_error(error.filename());
    return false; // data is invalid
}
{% endhighlight %}

This makes our code more readable, helps to strengthen our invariants, and
allows us to avoid any dangers of leaving occasionally uninitialised variables
lying around for mistakes to creep in later.
We can also then use the result of the function directly as a parameter to
another function, again, opening possibilities that allow us to make our code
more readable.

{% highlight cpp %}
try
{
    return validate_data_using_public_key(
        data,
        get_public_key(public_key));
}
catch(const key_file_not_found& error)
{
    report_bad_key_error(error.filename());
    return false; // data is invalid
}
{% endhighlight %}

In summary:

- It is impossible to accidentally ignore errors communicated via exceptions.
- Exceptions allow us to propagate more information about the error to the site
    that will handle it.
- Exceptions allow us to handle errors with the appropriate level of granularity
    (or abstraction) using inheritance and other object-oriented techniques.
- Exceptions allow us to mix errors from different sources without specialised
    code.
- When using exceptions, we can add new failure modes without modifying code
    that is responsible for propagating error information.
- Exceptions allow us to write functions with clean interfaces.

Next time we start on the problems with exceptions!
