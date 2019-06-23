# compiletime

Running benchmarks of complex PHP code on the CLI? The numbers are lying to
you. Even with `opcache.enable_cli=1` you will have compilation times of the
files in timings. With this extension you can have access how much milliseconds
the current PHP script has spent in compiling (not executing) code.

## Installation

    phpize
    ./configure
    make
    sudo make install
    echo 'extension=compiletime.so' > /path/to/php.ini.d/compiletime.ini

## Usage

Extension exposes one simple function:

    /**
     * Returns the current time spent in compiling since the beginning of script.
     */
    function compiletime_get_current() : int

Substracting two compile time durations allows to compute the time compiling
between those calls.

License: MIT
