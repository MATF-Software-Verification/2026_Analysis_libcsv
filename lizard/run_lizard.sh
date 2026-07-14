#!/bin/bash

lizard -V ../libcsv/libcsv.c > lizard_report.txt

lizard -V --html -o lizard_report.html ../libcsv/libcsv.c
