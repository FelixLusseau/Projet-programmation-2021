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


annoncer "Erreur pas d'arguments"
$VALGRIND ./$TARGET >> $LOG 2>&1 && fail
coloredEcho "OK" green

annoncer "Execution parsing base sample"
$VALGRIND ./$TARGET -i tests/sample.xml -o tests/out -x >> $LOG 2>&1 || fail
coloredEcho "OK" green

annoncer "Execution parse base sample1ref et affichage structure"
$VALGRIND ./$TARGET -i tests/sample1ref.xml -o tests/out -x
$VALGRIND ./$TARGET -i tests/sample1ref.xml -o tests/out -r > tests/out.txt || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/sample1refresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Erreur entrée non XML"
$VALGRIND ./$TARGET -i tests/sample.bin -o tests/out >> $LOG 2>&1 && fail
coloredEcho "OK" green

annoncer "Execution graphe sample"
$VALGRIND ./$TARGET -o tests/sample.bin -m >> $LOG 2>&1 || fail
coloredEcho "OK" green

annoncer "Execution recherche articles (et auteurs)"
$VALGRIND ./$TARGET -o tests/sample.bin -a "Russell Turpin" >> $LOG 2>&1 || fail
coloredEcho "OK" green

annoncer "Execution tests.c + cov"
cd tests/
make cov  >> ../$LOG 2>&1 || fail
coloredEcho "OK" green
echo "Rapport de couverture disponible ici : ./tests/rapport/index.html"
cd ..

exit 0
