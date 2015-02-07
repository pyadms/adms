#!/bin/bash
set -e
progname="$0"
curdir=`dirname "$progname"`
binfile="${curdir}/../../cmake/admsXml/admsXml"
${binfile} $* -e $curdir/ngspice.xml
