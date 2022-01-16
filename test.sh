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
$VALGRIND ./$TARGET -i tests/sample1ref.xml -o tests/out -x >> $LOG 2>&1 || fail
$VALGRIND ./$TARGET -i tests/sample1ref.xml -o tests/out -r > tests/out.txt 2>&1 || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/sample1refresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Erreur entrée non XML"
$VALGRIND ./$TARGET -i tests/sample.bin -o tests/out -x > tests/out.txt 2>&1 && fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/testnonXMLresult.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Execution graphe sample"
$VALGRIND ./$TARGET -o tests/sample.bin -g -s >> $LOG 2>&1 || fail
coloredEcho "OK" green

annoncer "Execution recherche auteurs"
$VALGRIND ./$TARGET -o tests/sample.bin -l "Rus" >> $LOG 2>&1 || fail
coloredEcho "OK" green

annoncer "Execution recherche articles (et auteurs) de l'année 2012"
$VALGRIND ./$TARGET -o tests/sample.bin -a "Yining Li" -y 2012 > tests/out.txt || fail
echo "===DIFF===" >> $LOG
diff -Z tests/out.txt tests/testarticles2012result.txt >> $LOG 2>&1
if [ $? -ne 0 ]
then
    fail
fi
coloredEcho "OK" green

annoncer "Execution plus court chemin"
$VALGRIND ./$TARGET -o tests/sample2.bin -p "Takaya Asano" -p "Takuya Iwamoto" >> $LOG 2>&1 || fail
coloredEcho "OK" green

annoncer "Execution distances"
$VALGRIND ./$TARGET -o tests/sample2.bin -p "Takaya Asano" -p "Takuya Iwamoto" -d >> $LOG 2>&1 || fail
coloredEcho "OK" green

annoncer "Execution auteurs à distance n"
$VALGRIND ./$TARGET -o tests/sample2.bin -a "Takaya Asano" -n 2 >> $LOG 2>&1 || fail
coloredEcho "OK" green

annoncer "Execution composantes connexes sample"
$VALGRIND ./$TARGET -o tests/sample.bin -c >> $LOG 2>&1 || fail
coloredEcho "OK" green

annoncer "Execution tests.c + cov"
cd tests/
make cov < in >> ../$LOG 2>&1 || fail
coloredEcho "OK" green
echo "Rapport de couverture disponible ici : ./tests/rapport/index.html"
cd ..

exit 0
