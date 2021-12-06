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


annoncer "Execution erreur"
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

#annoncer "Execution erreur 2"
#$VALGRIND ./$TARGET -i tests/cat.bmp -o /NEXSTEPAS/OUT.TXT >> $LOG 2>&1 && fail
#coloredEcho "OK" green


#annoncer "Execution erreur 3"
#$VALGRIND ./$TARGET -e -i tests/cat.bmp >> $LOG 2>&1 && fail
#coloredEcho "OK" green



#annoncer "Execution encode"
#$VALGRIND ./$TARGET -e -m "$(cat tests/message.txt)" -i tests/cat.bmp -o tests/out.bmp >> $LOG 2>&1 || fail
#coloredEcho "OK" green

#annoncer "Execution dencode"
#$VALGRIND ./$TARGET -d -i tests/out.bmp > tests/out.txt || fail
#echo "===DIFF===" >> $LOG
#diff tests/out.txt tests/message.txt >> $LOG 2>&1
#if [ $? -ne 0 ]
#then
#    fail
#fi
#coloredEcho "OK" green


exit 0
