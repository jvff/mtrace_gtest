# mtrace_gtest #

## Summary

mtrace_gtest is a simple library that integrates with Google Test in
order to check for memory leaks. It uses the GNU Libc mtrace
functionality that registers all memory allocations and deallocations.
Two libraries are provided, one with the mtrace_gtest functionality and
another called mtrace_gtest_main that implements a `main()` function
that runs all the tests with the memory leak checks enabled.

The library is very simple, and will fail a test if memory leaks are
detected or if invalid deallocations (deallocations of memory that
wasn't allocated during the test, for example) were performed. Not much
information is reported, only the number of memory leaks wth the total
amount of leaked memory and/or the number of invalid deallocations.

It will help you check for memory leaks, but not in actually finding
them. Therefore, it is recommended to keep your tests focused and simple
in order to help with the process to find any eventual problems.

## Dependencies

To compile the libraries, you will need Java to run the Gradle build
system. Gradle itself isn't needed, because the wrapper script should
download the appropriate version of it automatically.

You will also need to have the Google Test library and the FakeIt
mocking library installed. You can set the path to the `libgtest.a`
library and the FakeIt headers in the `build.gradle` file.


## Compiling

The libraries are built with the following command:

    ./gradlew assemble

## Testing

You can run all of the built-in tests running the command

    ./gradlew test

## Usage

There are two ways to use the library

### Using the provided `main()` function

You can statically link your code with `libmtrace_gtest.a` and
`libmtrace_gtest_main.a`, and all your tests should run with the
provided memory checks.

Note that this replaces Google Test's default `main()` function provided
by `libgtest_main.a`, therefore don't link an executable with both
libraries.

### Using a custom `main()` function

If you need a custom `main()` function, you don't have to link with
`libmtrace_gtest_main.a`. You only have to link with `libmtrace_gtest.a`
and write your own `main()` function.

The provided `main()` function is rather simple, as can be seen in
`src/mtrace_gtest_main/cpp/main.cpp`. The only thing you have to do
before executing any tests is register a MemoryTracerListener instance
with Google Test. The line to do that follows:

    UnitTest::GetInstance()->listeners().Append(new MemoryTracerListener());

## Contributing

Feel free to send feedback, questions, suggestions, fixes, bug reports,
or anything else you think can help this project.

If you provide code, my only request is to also provide test cases of
what is being done and/or what bugs were fixed (ie. a test that fails
without the new code and passes with the new code).

Personally, I develop using TDD, so I would recommend that. However,
feel free to add tests after development if you prefer. I might reorder
the commits to push the tests first though. Also keep the tests small.
I'd rather have a million small and focused tests than one that does the
same thing but makes it hard to isolate future problems.
