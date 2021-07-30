#! /bin/bash
#
#  Shell script for running Valgrind on tests of your solution.  Invoke as:
#
#   runvalgrind.sh <name of your executable> <command line parameters you'd use to run normally>
#
#  For example:
#
#    runvalgrind.sh c07driver NM_100.txt
#
#  (Make sure you have execute permissions set for this file.)
#
#  After running the script, look at the file vlog.txt for Valgrind results.
#
valgrind --leak-check=full --show-leak-kinds=all --log-file=vlog.txt --track-origins=yes -v $@
