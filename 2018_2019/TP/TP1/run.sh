#! /bin/bash

rm -f r.gnp r1.gnp r2.gnp

echo "
set term pdf
set output 'r.pdf'
" > r.gnp

function run {
    rm -f r s && touch r s
    c=$1
    code=./build/matmul${c}
    for i in {1..50}
    do
        let n=i*200
        echo matmul${c} $n
        echo $n >> s
        perf stat -e cache-misses ${code} $n $n >& x
        cat x >> r
    done
    grep cache-misses r > r1 && \
        sed -i -e 's/cache-misses//' -e 's/[^0-9]*//g' r1
    grep "seconds time elapsed" r > r2 && \
        sed -i -e 's/seconds time elapsed//' -e 's/,/./' -e 's/[^.0-9]*//g' r2
    paste s r1 r2 > r_${c}
    echo -n "'r_"${c}"' using 1:2 w lp title 'parcours "${c}"', " >> r1.gnp
    echo -n "'r_"${c}"' using 1:3 w lp title 'parcours "${c}"', " >> r2.gnp
}

function runb {
    rm -f r s && touch r s
    c=$1
    code=./build/matmul${c}
    p=$2
    for i in {1..50}
    do
        let n=i*200
        echo matmul${c} $n $p
        echo $n >> s
        perf stat -e cache-misses ${code} $n $n $p >& x
        cat x >> r
    done
    grep cache-misses r > r1 && \
        sed -i -e 's/cache-misses//' -e 's/[^0-9]*//g' r1
    grep "seconds time elapsed" r > r2 && \
        sed -i -e 's/seconds time elapsed//' -e 's/,/./' -e 's/[^.0-9]*//g' r2
    paste s r1 r2 > r_${c}_${p}
    echo -n "'r_"${c}"_"${p}"' using 1:2 w lp title 'bloc "${p}" parcours "${c}"', " >> r1.gnp
    echo -n "'r_"${c}"_"${p}"' using 1:3 w lp title 'bloc "${p}" parcours "${c}"', " >> r2.gnp
}

echo -n "plot " > r1.gnp
echo -n "plot " > r2.gnp

run 1
run 2
for c in 3 4 5
do
for p in 50
do
    runb $c $p
done
done

echo >> r1.gnp
echo >> r2.gnp
cat r1.gnp r2.gnp >> r.gnp
sed -i -e "s/, *$//" r.gnp
gnuplot r.gnp

