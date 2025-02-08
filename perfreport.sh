#!/bin/bash
make -j
rm -rf perf && mkdir perf
cp main.exe perf/
perf record -o perf/perf.data -F 99 -g -- ./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500
chmod 777 perf/perf.data
perf stat -o perf/perfstat.txt -- ./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500
perf script -i perf/perf.data | perl FlameGraph/stackcollapse-perf.pl | perl FlameGraph/flamegraph.pl --width 2400 --height 48 > perf/flamegraph.svg