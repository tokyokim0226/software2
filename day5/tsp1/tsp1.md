## 初期解を増やしていったときの結果の変化

初期解が100000個の時
```
total distance = 221.241685
17 -> 13 -> 12 -> 3 -> 14 -> 0 -> 4 -> 19 -> 6 -> 18 -> 5 -> 2 -> 15 -> 11 -> 9 -> 10 -> 16 -> 7 -> 1 -> 8 -> 17
```

初期解が1000000個の時
```
total distance = 218.928694
7 -> 19 -> 11 -> 9 -> 15 -> 6 -> 18 -> 10 -> 16 -> 3 -> 1 -> 17 -> 8 -> 13 -> 12 -> 4 -> 14 -> 0 -> 5 -> 2 -> 7
```

初期解が5000000個の時
```
total distance = 192.238218
5 -> 7 -> 4 -> 19 -> 2 -> 18 -> 6 -> 15 -> 9 -> 11 -> 10 -> 16 -> 14 -> 3 -> 0 -> 1 -> 17 -> 8 -> 13 -> 12 -> 5
```

初期解の個数を増やすほど解析解に近づいていき,5000000個でほぼ最短距離で一周するような解になった。