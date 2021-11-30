#skrypt odpalający programy

ZESTAW=""
ARG_STR=""
ARG=""
NR=0

function uruchom() {
    zenity --info --title="Przed uruchomieniem" --text="Kliknij ok by uruchomić program." --width=500
}

function wczytaj(){
    ARG=$(zenity --entry --title "Wpisz argument ${ARG_STR}" --width 500 --text "Wpisz argument numer ${NR}:")
}

function wybor_zestawu() {
case $ZESTAW in

    Zestaw01)
        zestaw01
        ;;

    Zestaw02)
        zestaw02
        ;;

    Zestaw03)
        zestaw03
        ;;

    Zestaw045)
        zestaw045
        ;;

    Zestaw06)
        zestaw06
        ;;

    Zestaw07)
        zestaw07
        ;;

    Zestaw08)
        zestaw08
        ;;

    Zestaw09)
        zestaw09
        ;;

    Zestaw10)
        zestaw10
        ;;

esac
}

function zestaw01() {

    ZADANIE=$(zenity --list --title="Wybierz zadanie" --column="Zadanie" --column="Nazwa" --height 225 1 "Procinfo" \
2 "Kopiowanie" \
3 "Procesy" \
4 "Lider")


    case $ZADANIE in

    1)
        code procinfo.h procinfo.c main.c
        sleep 1
        uruchom
        ./procinfo.x
        ;;
    2)
        code kopiuj.c
        sleep 1
        NR=1
        wczytaj
        ARG1=$ARG
        NR=2
        wczytaj
        ARG2=$ARG
        uruchom
        ./kopiuj.x $ARG1 $ARG2
        ;;
    3)
        code procesy.c
        sleep 1
        uruchom
        ./procesy.x
        ;;
    4) 
        code lider.c
        sleep 1
        uruchom
        ./lider.x
        ;;
    esac
}

function zestaw02(){
    ZADANIE=$(zenity --list --title="Wybierz zadanie" --column="Zadanie" --column="Nazwa" --height 225 1a "Drzewko1" \
1b "Drzewko2" \
2 "Macierzysty" \
3a "Zombi" \
3b "Potomny")

    case $ZADANIE in

        1a)
            ;;
        1b)
            ;;
        2)
            ;;
        3a)
            ;;
        3b)
            ;;

    esac
}

function zestaw03(){
    ZADANIE=$(zenity --list --title="Wybierz zadanie" --column="Zadanie" --column="Nazwa" --height 225 1a "Drzewko1" \
1b "Drzewko2" \
2 "Macierzysty" \
3a "Zombi" \
3b "Potomny")

    case $ZADANIE in

        1a)
            ;;
        1b)
            ;;
        2)
            ;;
        3a)
            ;;
        3b)
            ;;

    esac
}

function zestaw045(){
    ZADANIE=$(zenity --list --title="Wybierz zadanie" --column="Zadanie" --column="Nazwa" --height 225 1a "Drzewko1" \
1b "Drzewko2" \
2 "Macierzysty" \
3a "Zombi" \
3b "Potomny")

    case $ZADANIE in

        1a)
            ;;
        1b)
            ;;
        2)
            ;;
        3a)
            ;;
        3b)
            ;;

    esac
}

function zestaw06(){
    ZADANIE=$(zenity --list --title="Wybierz zadanie" --column="Zadanie" --column="Nazwa" --width 300 --height 225 1 "libsem.a" \
2 "Semuse" \
3 "Prog, powielacz")

    case $ZADANIE in

        1)
            code sem.h sem.c
            sleep 1
            ;;
        2)
            code semuse.c
            sleep 1
            uruchom
            xterm -ls /bin/bash &
            xterm -ls /bin/bash
            ;;
        3)
            code prog.c powielacz.c
            sleep 1
            ARG_STR="Liczba procesów"
            NR=1
            wczytaj
            ARG1=$ARG
            ARG_STR="Liczba pętli"
            NR=2
            wczytaj
            ARG2=$ARG
            ARG_STR=""
            uruchom
            ./powielacz.x ./prog.x $ARG1 $ARG2
            ;;

    esac
}

function zestaw07(){
    ZADANIE=$(zenity --list --title="Wybierz zadanie" --column="Zadanie" --column="Nazwa" --width 300 --height 225 1 "Libshmem.so" \
2 "Shmuse" \
3 "Producent i konsument")

    case $ZADANIE in

        1)
            code shmem.h shmem.c
            sleep 1
            ;;
        2)
            code shmuse.c
            sleep 1
            uruchom
            ./shmuse1.x
            xterm -ls /bin/bash
            ;;
        3)
            cp ../magazyn.txt magazyn.txt
            code glowny.c producent.c konsument.c bufor.c bufor.h
            sleep 1
            uruchom
            ./glowny.x
            sleep 1
            ;;

    esac
}

function zestaw08(){
    ZADANIE=$(zenity --list --title="Wybierz zadanie" --column="Zadanie" --column="Nazwa" --height 225 1 "libmsg" \
2 "Serwer-klient")

    case $ZADANIE in

        1)
            code msg.h msg.c
            sleep 1
            ;;
        2)
            code serwer.c klient.c srv.h
            sleep 1
            uruchom
            make run
            ;;

    esac
}

function zestaw09(){
    ZADANIE=$(zenity --list --title="Wybierz zadanie" --column="Zadanie" --column="Nazwa" --height 225 1 "Wykluczanie")

    case $ZADANIE in

        1)
            code wykluczanie.c
            sleep 1
            uruchom
            ./wykluczanie.x
            ;;

    esac
}

function zestaw10(){
    ZADANIE=$(zenity --list --title="Wybierz zadanie" --column="Zadanie" --column="Nazwa" --width 350 --height 225 1 "Algorytm piekarni" \
2 "Algorytm piekarni z licznikiem")

    case $ZADANIE in

        1)
            code piekarnia.c
            sleep 1
            uruchom
            ./piekarnia.x
            ;;
        2)

            code piekarnia_licznik.c
            sleep 1
            uruchom
            ./piekarnia_licznik.x
            ;;

    esac
}

#PROGRAM


while true;
do

    clear

    #Wczytaj numer zestawu
    ZESTAW=$(zenity --list --title="Wybierz zestaw" --column="Zestaw" --column="Nazwa" --width=350 --height=350 01 "Procesy" \
    02 "Procesy c.d." \
    03 "Sygnały" \
    045 "Potoki" \
    06 "Semafory" \
    07 "Obiekty pamięci dzielonej" \
    08 "Kolejki komunikatów" \
    09 "Wątki" \
    10 "Synchronizacja wątków")

    ZESTAW="Zestaw${ZESTAW}"

    if [[ $ZESTAW == "Zestaw" ]]; 
        then 
        break; 

    else
        #Przejdź do danego zestawu i skompiluj wszystko
        cd ~/Code/C/${ZESTAW} 
        make all

        wybor_zestawu

        if zenity --question --title "Inne zadanie?" --text "Chcesz wybrać inne zadanie?" --no-wrap --ok-label "Nie" --cancel-label "Tak" 
            then 
            make clean
            break;
        else
            make clean
        fi



    fi


done

echo "DONE"