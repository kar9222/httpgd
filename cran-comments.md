## Test environments
GitHub rlib/actions:
* Windows Server 2019 10.0.20348, R 4.2.2
* Mac OS X 12.6.2, R 4.2.2
* Ubuntu 22.04.1, R 4.2.2
* Ubuntu 22.04.1, R 4.1.3
* Ubuntu 22.04.1, R devel (2023-01-06 r83576)

## R CMD check results
There were no ERRORs or WARNINGs. 

There was 1 NOTE:

* checking installed package size ... NOTE
    installed size is  5.6Mb
    sub-directories of 1Mb or more:
      libs   5.4Mb

  This is the directory where build time dependency png.h is downloaded on windows.

## Downstream dependencies
There are no downstream dependencies.


## CRAN packages missing inclusion of ´<cstdint>´ 

> POSetR RcppAlgos duckdb fixest flexpolyline fstcore gdalcubes ggiraph
> gkmSVM groupedSurv httpgd libgeos naryn rayrender readxlsb rgeoda rvg s2
> svglite spiderbar tgstat vdiffr wkutils
>
> See the logs and README.txt at https://www.stats.ox.ac.uk/pub/bdr/gcc13/ .
> 
> Although you may not have access to gcc 13 to test this, the missingness
> of the header should be easy to identify from the logs: it defines types
> such as uint32_t and places them in the std namespace.
> 
> Please correct before 2023-01-24 to safely retain your package on CRAN.
> (CRAN submissions are shut until 2023-01-06.)

I have included the missing header.


## Compilation issues with upcoming version of dependency ´BH´ 1.81.0

> Notified by Dirk Eddelbuettel on Thu, Dec 15, 2022.

These have been fixed.


## Comments from CRAN at last submission

> Please add \value to .Rd files regarding exported methods and explain
> the functions results in the documentation. Please write about the
> structure of the output (class) and also what the output means. (If a
> function does not return a value, please document that too, e.g.
> \value{No return value, called for side effects} or similar)
> Missing Rd-tags:
>       hgd_test_pattern.Rd:  \value

\value field has been added.

> \dontrun{} should only be used if the example really cannot be executed
> (e.g. because of missing additional software, missing API keys, ...) by
> the user. That's why wrapping examples in \dontrun{} adds the comment
> ("# Not run:") as a warning for the user.
> Does not seem necessary.
> Please unwrap the examples if they are executable in < 5 sec, or replace
> \dontrun{} with \donttest{}.

All occurrences of \dontrun have been replaced.

> Please ensure that your functions do not write by default or in your
> examples/vignettes/tests in the user's home filespace (including the
> package directory and getwd()). This is not allowed by CRAN policies.
> Please omit any default path in writing functions. In your
> examples/vignettes/tests you can write to tempdir().

To our knowledge no function writes any file in the user's home directory.
Please let us know if you have observed this behaviour.

> Please always make sure to reset to user's options(), working directory
> or par() after you changed it in examples and vignettes and demos.
> e.g.:
> oldpar <- par(mfrow = c(1,2))
> ...
> par(oldpar)

We do not have any occurence of par() in examples and vignettes and demos.
