#!/bin/sh

TARGET=bin/program

#
# Ce placer dans le répertoire courant
#
TESTDIR="$(cd "$(dirname "$0}")" && pwd)"
cd "$TESTDIR"

#
# Conserver une trace de toutes les actions
#
LOG="test.log"
rm -f "$LOG"
touch "$LOG"

#
# Importer les fonctions communes à tous les tests
#
. ./fonctions.sh


#
# Compilation du programme.
#

annoncer "Compilation"
make clean
make $TARGET >> $LOG 2>&1 || fail
coloredEcho "OK" green


annoncer "Error no argument"
$VALGRIND ./$TARGET >> $LOG 2>&1 && fail
coloredEcho "OK" green

annoncer "Execution parsing base sample"
$VALGRIND ./$TARGET -i tests/sample.xml -o tests/out -x > tests/out.txt || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/tests_outputs/testparsingsampleresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Execution parse base sample1ref and print structure"
$VALGRIND ./$TARGET -i tests/sample1ref.xml -o tests/out -x >> $LOG 2>&1 || fail
$VALGRIND ./$TARGET -i tests/sample1ref.xml -o tests/out -r > tests/out.txt 2>&1 || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/tests_outputs/sample1refresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Error entry not XML"
$VALGRIND ./$TARGET -i tests/sample.bin -o tests/out -x > tests/out.txt 2>&1 && fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/tests_outputs/testnonXMLresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Execution graphe sample"
$VALGRIND ./$TARGET -o tests/sample.bin -g -s > tests/out.txt || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/tests_outputs/testgraphesampleresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Execution parsing base sample + graphe in one command"
$VALGRIND ./$TARGET -i tests/sample.xml -o tests/out -x -g > tests/out.txt || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/tests_outputs/testparseandgrapheresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Execution research authors"
$VALGRIND ./$TARGET -o tests/sample.bin -l "Rus" > tests/out.txt || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/tests_outputs/testresearchauthorsresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Execution research articles (and authors) of the year 2012"
$VALGRIND ./$TARGET -o tests/sample.bin -a "Yining Li" -y 2012 > tests/out.txt || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/tests_outputs/testarticles2012result.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Execution shortest path"
$VALGRIND ./$TARGET -o tests/sample2.bin -p "Takaya Asano" -p "Takuya Iwamoto" > tests/out.txt || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/tests_outputs/testshortestpathresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Execution distances"
$VALGRIND ./$TARGET -o tests/sample2.bin -p "Takaya Asano" -p "Takuya Iwamoto" -d > tests/out.txt || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/tests_outputs/testdistancesresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Execution authors at distance n"
$VALGRIND ./$TARGET -o tests/sample2.bin -a "Takaya Asano" -n 2 > tests/out.txt || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/tests_outputs/testauthorsatdistnresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Execution connected components sample"
$VALGRIND ./$TARGET -o tests/sample.bin -c > tests/out.txt || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/tests_outputs/testconnectedcomponentsresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Execution tests.c + cov"
cd tests/
make cov < yes_for_tests_c >> ../$LOG 2>&1 || fail
coloredEcho "OK" green
coloredEcho "\nCoverage report available here : ./tests/rapport/index.html\n" cyan
cd ..

exit 0
