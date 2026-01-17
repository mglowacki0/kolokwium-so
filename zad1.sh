#!/bin/bash

if [ -z "$1" ]; then
	echo "Podaj czas trwania w sekundach."
	exit 1
fi

time=$1

sleep $time &
pid=$!

echo "Uruchamiam zadanie. PID: $pid. Zadanie zakończy się za $time sekund."

#do ctrl + c
clean_up(){
	echo "Sygnał przerwania odebrany. Zatrzymuję zadanie."
	kill $pid
	exit 1
}

trap clean_up SIGINT

wait $pid

if [ $? -eq 0 ]; then
	echo "Zadanie zakończone pomyślnie."
else
	echo "Błąd podczas wykonywania zadania."
fi
