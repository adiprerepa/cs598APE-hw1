#!/bin/bash
perf script | perl FlameGraph/stackcollapse-perf.pl | perl FlameGraph/flamegraph.pl --width 2400 --height 48 > $1