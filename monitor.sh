#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Użycie: $0 <katalog>"
	exit 1
fi

DIR="$1"
LOG="report.log"
FIFO="/tmp/monitor_fifo_$$"

SAFE_MODE=0
REMOVED=0

[ -p "$FIFO" ] || mkfifo "$FIFO"

#obsługa sygnałów
trap 'echo "Zakończono. Usunięto plików: $REMOVED"; rm -f "$FIFO"; exit 0' SIGINT
trap 'SAFE_MODE=$((1 - SAFE_MODE)); echo "SAFE MODE = $SAFE_MODE"' SIGUSR1

#co 2 sek liczy liczbe plikow i wysyla do fifo
(
	while true; do
		COUNT=$(find "$DIR" -maxdepth 1 -type f -print0 | tr -cd '\0' | wc -c)
		echo "$COUNT" > "$FIFO"
		sleep 2
	done
) &

while true; do
	if read COUNT < "$FIFO"; then
		if [ "$COUNT" -gt 5 ]; then
			OLDEST_FILE=$(find "$DIR" -maxdepth 1 -type f -printf '%T@ %p\n' | sort -n | head -n1 | cut -d' ' -f2-)

			if [ "$SAFE_MODE" -eq 1 ]; then
				echo "$(date) OSTRZEŻENIE: przekroczono limit plików" >> "$LOG"
			else
				rm -- "$OLDEST_FILE"
				REMOVED=$((REMOVED + 1))
				echo "$(date) Usunięto: $(basename "$OLDEST_FILE")" >> "$LOG"
			fi
		fi
	fi
done


wait
