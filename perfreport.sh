#!/bin/bash

# Parse flags
PIANOROOM=false
ELEPHANT=false
GLOBE=false

while getopts "PEG" opt; do
  case ${opt} in
    P ) PIANOROOM=true ;;
    E ) ELEPHANT=true ;;
    G ) GLOBE=true ;;
    \? ) echo "Usage: $0 [-P] [-E] [-G]"; exit 1 ;;
  esac
done

make -j
rm -rf perf && mkdir perf && chmod 777 perf
cp main.exe perf/
mkdir -p perf/pianoroom perf/elephant perf/globe

if [ "$PIANOROOM" = true ]; then
  perf record -o perf/pianoroom/perf.data -F 99 -g -- ./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500
  perf stat -o perf/pianoroom/perfstat.txt -- ./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500
  perf script -i perf/pianoroom/perf.data | perl FlameGraph/stackcollapse-perf.pl | perl FlameGraph/flamegraph.pl --width 2400 --height 48 > perf/pianoroom/flamegraph.svg
fi

if [ "$ELEPHANT" = true ]; then
  perf record -o perf/elephant/perf.data -F 99 -g -- ./main.exe -i inputs/elephant.ray --ppm -a inputs/elephant.animate --movie -F 24 -W 100 -H 100 -o output/sphere.mp4
  perf stat -o perf/elephant/perfstat.txt -- ./main.exe -i inputs/elephant.ray --ppm -a inputs/elephant.animate --movie -F 24 -W 100 -H 100 -o output/sphere.mp4
  perf script -i perf/elephant/perf.data | perl FlameGraph/stackcollapse-perf.pl | perl FlameGraph/flamegraph.pl --width 2400 --height 48 > perf/elephant/flamegraph.svg
fi

if [ "$GLOBE" = true ]; then
  perf record -o perf/globe/perf.data -F 99 -g -- ./main.exe -i inputs/globe.ray --ppm -a inputs/globe.animate --movie -F 24 -W 100 -H 100 -o output/sphere.mp4
  perf stat -o perf/globe/perfstat.txt -- ./main.exe -i inputs/globe.ray --ppm -a inputs/globe.animate --movie -F 24 -W 100 -H 100 -o output/sphere.mp4
  perf script -i perf/globe/perf.data | perl FlameGraph/stackcollapse-perf.pl | perl FlameGraph/flamegraph.pl --width 2400 --height 48 > perf/globe/flamegraph.svg
fi
