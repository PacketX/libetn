libetn - C library for effective TLD names
==============

```
                    Domain                    eTLD                  eTLD+1      Manage By
>             amazon.co.uk                   co.uk            amazon.co.uk  is  ICANN Managed
>       books.amazon.co.uk                   co.uk            amazon.co.uk  is  ICANN Managed
>   www.books.amazon.co.uk                   co.uk            amazon.co.uk  is  ICANN Managed
>               amazon.com                     com              amazon.com  is  ICANN Managed

>      example0.debian.net              debian.net     example0.debian.net  is  Privately Managed
>      example1.debian.org                     org              debian.org  is  ICANN Managed

>               golang.dev                     dev              golang.dev  is  ICANN Managed
>               golang.net                     net              golang.net  is  ICANN Managed
>          play.golang.org                     org              golang.org  is  ICANN Managed
>  gophers.in.space.museum            space.museum         in.space.museum  is  ICANN Managed

>                 0emm.com                     com                0emm.com  is  Unmanaged
>               a.0emm.com              a.0emm.com                          is  Privately Managed
>           b.c.d.0emm.com              d.0emm.com            c.d.0emm.com  is  Privately Managed

>     there.is.no.such-tld                such-tld             no.such-tld  is  Unmanaged

>                  foo.org                     org                 foo.org  is  ICANN Managed
>                foo.co.uk                   co.uk               foo.co.uk  is  ICANN Managed
>           foo.dyndns.org              dyndns.org          foo.dyndns.org  is  Privately Managed
>       foo.blogspot.co.uk          blogspot.co.uk      foo.blogspot.co.uk  is  Privately Managed
>                cromulent               cromulent                          is  Unmanaged
```

For dependency
-----------

```
# do once
$ go get -u golang.org/x/net/idna

# or
$ make go-dep
```

Prepare a pre-compiled data file
-----------

```
$ cd ci
$ go run precompile.go -h
Usage of precompile
    -output string
        Output filename (default "public_suffix_compiled.dat")
    -url string
        URL of the publicsuffix.org list. If empty, stdin is read instead (default "https://publicsuffix.org/list/effective_tld_names.dat")
exit status 2
$ go run precompile.go
$ ls public_suffix_compiled.dat
public_suffix_compiled.dat

# or
$ make precompile
$ ls ci/public_suffix_compiled.dat
ci/public_suffix_compiled.dat
```

Benchmark
-----------

```
Load 'ci/public_suffix_compiled.dat' spent: 0.000103 seconds
Get public suffix 680000 times, spent 0.082537 seconds
Get eTLD 620000 times, spent 0.088502 seconds
```
