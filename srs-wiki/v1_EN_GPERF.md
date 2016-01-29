# Gperf memory and cpu analysis

The valgrind is a very good memory and cpu profile tool, but the st(state-threads)
used by SRS does not support valgrind.

The [gperf](https://code.google.com/p/gperftools) is a project of google,
to analysis memory and cpu, base on tcmalloc. SRS can use gperf.

The use scenarios:
* gmc: gperf memory check, read http://google-perftools.googlecode.com/svn/trunk/doc/heap_checker.html
* gmp: gperf memory profile, read http://google-perftools.googlecode.com/svn/trunk/doc/heapprofile.html
* gcp: gperf cpu profile, read http://google-perftools.googlecode.com/svn/trunk/doc/heapprofile.html

## gmc memory check

To use gmc for SRS:
* When configure gmc: `./configure --with-gperf --with-gmc`
* Compile srs: `make`
* Set env when start SRS: `env PPROF_PATH=./objs/pprof HEAPCHECK=normal ./objs/srs -c conf/srs.conf`
* Stop SRS, get the gmc report: press `CTRL+C` or send SIGINT to SRS.

Note: When make SRS success, it will show these commands.

It work when SRS print the following message:

```bash
[winlin@dev6 srs]$ env PPROF_PATH=./objs/pprof HEAPCHECK=normal ./objs/srs -c conf/srs.conf
WARNING: Perftools heap leak checker is active -- Performance may suffer
```

The gmc report:

```bash
Leak check _main_ detected leaks of 184 bytes in 4 objects
The 4 largest leaks:
Using local file ./objs/srs.
Leak of 56 bytes in 1 objects allocated from:
	@ 46fae8 _st_stack_new
	@ 46f6b1 st_thread_create
	@ 46ea65 st_init
	@ 433f41 SrsServer::initialize
	@ 46e4ca main
	@ 3855a1ec5d __libc_start_main
```

Other examples, see `research/gperftools/heap-checker`

## GMP memory profile

To use gmp for SRS:
* Configure SRS: `./configure --with-gperf --with-gmp`
* Build SRS: `make`
* Start SRS for memory profile: `rm -f gperf.srs.gmp*; ./objs/srs -c conf/srs.conf`
* Stop SRS to generate gmp file, press `CTRL+C` or send SIGINT to SRS.
* Analysis gmp file: `./objs/pprof --text objs/srs gperf.srs.gmp*`

Note: When make SRS success, it will show these commands.

It works when SRS show the messages:

```bash
[winlin@dev6 srs]$ ./objs/srs -c conf/srs.conf
Starting tracking the heap
```

The gmc result:

```bash
[winlin@dev6 srs]$ ./objs/pprof --text objs/srs gperf.srs.gmp*
Using local file objs/srs.
Using local file gperf.srs.gmp.0001.heap.
Total: 0.1 MB
     0.0  31.5%  31.5%      0.0  49.1% SrsConfDirective::parse_conf
     0.0  28.4%  59.9%      0.0  28.4% std::basic_string::_Rep::_S_create
     0.0  27.4%  87.3%      0.0  27.4% _st_epoll_init
     0.0  11.7%  99.0%      0.0  11.7% __gnu_cxx::new_allocator::allocate
     0.0   0.4%  99.5%      0.0  27.9% st_init
```

Other examples, read `research/gperftools/heap-profiler`

## GCP cpu profile

To use gmp for SRS:
* Configure with gcp: `./configure --with-gperf --with-gcp`
* Build SRS: `make`
* Start SRS with gcp: `rm -f gperf.srs.gcp*; ./objs/srs -c conf/srs.conf`
* Stop SRS to generate the gcp file, press `CTRL+C` or send SIGINT to SRS.
* Analysis gcp file: `./objs/pprof --text objs/srs gperf.srs.gcp*`

Note: When make SRS success, it will show these commands.

The gcp result:

```bash
[winlin@dev6 srs]$ ./objs/pprof --text objs/srs gperf.srs.gcp*
Using local file objs/srs.
Using local file gperf.srs.gcp.
Removing _L_unlock_16 from all stack traces.
Total: 20 samples
       8  40.0%  40.0%        8  40.0% 0x00007fff0ea35917
       4  20.0%  60.0%        4  20.0% __epoll_wait_nocancel
       2  10.0%  70.0%        2  10.0% __read_nocancel
       1   5.0%  95.0%        1   5.0% memset
       1   5.0% 100.0%        1   5.0% tc_delete
       0   0.0% 100.0%        5  25.0% 0x00007f9fad927c4f
       0   0.0% 100.0%        2  10.0% SrsBuffer::ensure_buffer_bytes
       0   0.0% 100.0%        5  25.0% SrsClient::do_cycle
       0   0.0% 100.0%        5  25.0% SrsClient::fmle_publish
       0   0.0% 100.0%        1   5.0% SrsClient::process_publish_message
```

Other examples, read `research/gperftools/cpu-profiler`

## Conflicts

The conflicts:
* gmc+gmp: Conflicts.
* gmc+gcp: OK.
* gmp+gcp: OK.

Note: The configure of SRS will check the conflicts.

Winlin 2014.11
