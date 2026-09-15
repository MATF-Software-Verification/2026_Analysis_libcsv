#!/bin/bash

echo "Pokretanje Flawfinder analize..."

flawfinder --html ../libcsv > "flawfinder_report.html" 2>&1

if [ $? -ne 0 ]; then
    echo "Flawfinder analiza nije uspešno završena."
    exit 1
fi

echo "Flawfinder analiza završena."
echo "Rezultati su u flawfinder_report.html"
