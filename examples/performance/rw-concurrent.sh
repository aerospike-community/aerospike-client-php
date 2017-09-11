#!/bin/bash

help() {
    cat << EOB
Usage: rw-concurrent.sh [-c WORKERS] [-h HOST] [-p PORT] [-n TRANSACTIONS] [-w WRITE-EVERY-N] LOG_PATH
   -h HOST     the IP address of the server. default: 127.0.0.1
   -p PORT     the port of the server. default: 3000
   -c WORKERS  the number of concurrent workers. default: 4
   -n NUM      the number of transactions per-worker. default: 50000
   -w RW-RATIO write every Nth transaction. default: 10 (for 9:1 rw ratio)
Example: ./rw-concurrent.sh -h 192.168.119.3 -c 4 -n 50000 -w 10 run.log
EOB
}

concurrent=4
host="127.0.0.1"
port="3000"
num="50000"
write_every="10"
OPTIND=1
while getopts "c:h:p:n:w:" opt; do
    case "$opt" in
        c)  concurrent=$OPTARG
            ;;
        h)  host=$OPTARG
            ;;
        p)  port=$OPTARG
            ;;
        n)  num=$OPTARG
            ;;
        w)  write_every=$OPTARG
            ;;
        '?')
            help
            exit 1
            ;;
    esac
done
shift "$(($OPTIND-1))" # shift off the options
log=$1

if [ ! $log ]; then
    help
    exit 2
fi

if [ -f $log ]; then
    rm $log && touch $log
fi

for((i=0; i<$concurrent; i++))
do
    /usr/bin/env php "./rw-worker.php" "-h$host" "-p$port" "-n$num" "-w$write_every" "$log" &
done
wait;
/usr/bin/env php "./rw-summary.php" "$log"

