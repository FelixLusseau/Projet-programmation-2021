#
# Ce fichier contient des définitions communes à tous les tests
#

VALGRIND="valgrind -q --leak-check=full --error-exitcode=43"


annoncer ()
{
    COURANT="$1"
    coloredEcho "==== $1 ====" blue
    echo "
####
#### $1
####" >> $LOG
}

fail ()
{
    coloredEcho "==> Échec du test '$COURANT'" red
    coloredEcho "==> Voir le fichier ./$TESTDIR/$LOG" red
    coloredEcho "==> Exit" red
    echo "$NOTE/100" > note.txt
    exit 1
}

success ()
{
  echo "100/100" > note.txt
  exit 0
}


# créer un fichier de taille et de nom indiqués
creer_fichier ()
{
    local taille="$1" nom="$2"
    dd if=/dev/zero of=$nom bs=$taille count=1 2> /dev/null
}


# Normaliser la sortie (supprimer espaces superflus gênant la comparaison)
normaliser ()
{
    sed -e 's/[ 	][ 	]*/ /g' -e 's/^ //'
}



# Fonctions de test:
# prend en argument 1 un fichier contenant la sortie du programme
# prend en argument 2 le texte attendu

test_grep()
{
  echo "#### grep" >> $LOG
  cat "$1" | grep "$2"
}

test_diff()
{
  echo "#### diff" >> $LOG
  echo "$2" | diff -Z -B "$1" -
}

test_diff_end()
{
  echo "#### diff end" >> $LOG
  echo "$1"
  LC=$(echo "$2" | wc -l -)
  echo "LINEC: $LC"
  CONTENT=$(cat "$1" | tail -n $LC)
  echo "$CONTENT" > "$1"
  echo "$CONTENT"
  echo "$2" | diff "$1" -
}


# Teste si un programme affiche bien ce qui est attendu
# pour une entrée donnée.

test_entree_sortie ()
{

  STDIN=$1
  ATTENDU=$2
  FONCTION_TEST=$3

  SORTIE=/tmp/tps-c.out.$$

  echo "\n#### Execution" >> $LOG
  echo "#### $VALGRIND ./$TARGET" >> $LOG

  # execute le programme, conserve la sortie (stdout et stderr confondu)
  echo "$STDIN" | $VALGRIND ./$TARGET 2>&1 > $SORTIE
  EXITCODE=$?

  # conserve la sorte dans le log
  cat $SORTIE >> $LOG
  echo "\n==> exitcode: $EXITCODE" >> $LOG

  # si le programme quitte avec un code d'erreur, c'est la fin
  if [ $EXITCODE -ne 0 ]
  then
    fail
  fi
  # sinon on vérifie qu'il retourne bien ce qui été attendu


  echo "\n#### Comparaison" >> $LOG

  # teste la sortie avec l'attendu en utilisant la
  # fonction de teste fournie (grep ou diff ou diff_end)
  $FONCTION_TEST "$SORTIE" "$ATTENDU" >> $LOG

  if [ $? -ne 0 ]
  then
    fail
  else
    coloredEcho "OK" green
  fi

}



test_fichiers_entree_sortie ()
{
  STDIN=$(cat $1)
  ATTENDU=$(cat $2)
  test_entree_sortie "$STDIN" "$ATTENDU" "$3"
}



# Affichage en couleur
coloredEcho ()
{
    local exp="$1";
    local color=$2;
     case $(echo $color | tr '[:upper:]' '[:lower:]') in
      black) color=16 ;;
      red) color=5 ;;
      green) color=2 ;;
      yellow) color=3 ;;
      blue) color=4 ;;
      orange) color=1 ;;
      cyan) color=6 ;;
      gray) color=8 ;;
      white|*) color=15 ;; # white or invalid color
     esac
     if [ $COLORIZE -eq 1 ]; then
      tput setaf $color;
      echo "$exp";
      tput sgr0;
    else
      echo "$exp";
    fi
}
if [ -z ${COLORIZE+x} ]; then
  COLORIZE=1
fi
