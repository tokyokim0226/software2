# 課題1
## Google Cloud Shellを用いて先頭アドレスと構造体のサイズを確認
## 構造体Student
```
typedef struct student
{
  int id;
  char name[100];
  int age;
  double height;
  double weight;
} Student;
```
を用いて、構造体`s_begin`,`s1`を作成した。
構造体のサイズと、そのメンバのアドレスは以下のようになった。
```
s_begin:128
s1:128
```
```
s1.id:0x7ffe821d6020
s1.name:0x7ffe821d6024
s1.age:0x7ffe821d6088
s1.height:0x7ffe821d6090
s1.weight:0x7ffe821d6098
s_begin.id:0x7ffe821d60a0
s_begin.name:0x7ffe821d60a4
s_begin.age:0x7ffe821d6108
s_begin.height:0x7ffe821d6110
s_begin.weight:0x7ffe821d6118
```
`id`が4bytes、`name`配列が100bytes、`age`が8bytes、`height`が8bytes、`weight`が8bytes使っていることがわかる。
`age`の後にパディングが4bytesあることがわかる。(`sizeof()`を使うと、`age`は4bytesになっているので、8-4=4)

また`s_begin`の`name`配列においてはサイズが340bytesあるように見えるが、構造体の実態のサイズは128bytesであるので、実際の大きさは100bytesであると予想できる。
## 構造体TStudent1
```
typedef struct tagged_student1
{
  int id;
  char name[100];
  int age;
  double height;
  double weight;
  char tag;
} TStudent1;
```
を用いて構造体's2'を作成した。
構造体のサイズと、そのメンバのアドレスは以下のようになった。
```
s2:136
```
```
s2.id:0x7ffe821d5f90
s2.name:0x7ffe821d5f94
s2.age:0x7ffe821d5ff8
s2.height:0x7ffe821d6000
s2.weight:0x7ffe821d6008
s2.tag:0x7ffe821d6010
````
`id`が4bytes、`name`配列が100bytes、`age`が8bytes、`height`が8bytes、`weight`が8bytes、`tag`が（136と今までの合計の差から）8bytes使っていることがわかる。
`age`の後に4bytesパディングが存在している。

また構造体`Student`との差は、最後の`char`型の`tag`のあるなしであるが、`tag`の後にパディングが7bytes存在している。
## 構造体TStudent2
```
typedef struct tagged_student2
{
  char tag;
  int id;
  char name[100];
  int age;
  double height;
  double weight;
} TStudent2;
```
を用いて構造体`s3`,`s_end`を作成した。
構造体のサイズと、そのメンバのアドレスは以下のようになった。
```
s3:128
s_end:128
```
```
s_end.tag:0x7ffe821d5e90
s_end.id:0x7ffe821d5e94
s_end.name:0x7ffe821d5e98
s_end.age:0x7ffe821d5efc
s_end.height:0x7ffe821d5f00
s_end.weight:0x7ffe821d5f08
s3.tag:0x7ffe821d5f10
s3.id:0x7ffe821d5f14
s3.name:0x7ffe821d5f18
s3.age:0x7ffe821d5f7c
s3.height:0x7ffe821d5f80
s3.weight:0x7ffe821d5f88
```
`tag`が4bytes,`id`が4bytes、`name`配列が100bytes(1x100)、`age`が4bytes、`height`が8bytes、`weight`が8bytes、使っていることがわかる。
`tag`の後にパディングが3bytes存在している。

また構造体`Student1`との差は`char`型の`tag`の場所であるが、`tag`の後のパディングが減っていることが確認できる。

## 構造体のアドレス
構造体の先頭アドレスは以下のようになっていた。
```
s_end:0x7ffe821d5e90
s3:0x7ffe821d5f10
s2:0x7ffe821d5f90
s1:0x7ffe821d6020
s_begin:0x7ffe821d60a0
```
構造体の先頭アドレスと各構造体の最初のメンバのアドレスが等しいことがわかる。

## まとめ
`Student`クラスの構造体`s_begin`、`s1`においては、`double`の前の`int`に4bytesのパディングがなされていた。

構造体`Student`の最後のメンバに`char`型の`tag`を足した`Student1`の構造体`s2`では、`double`の前の`int`に4bytes、doubleの後の`char`に7bytesのパディングがなされていた。

構造体`Student1`において`tag`を最初のメンバにした`Student2`クラスの構造体`s3`、`s_end`においては、`int`の前の`char`に3bytesパディングがなされていた。
また`double`の前の`int`に対するパディングはなくなっていた。

以上のことをまとめると、基本は近く（特に後ろ）のメンバ変数(`double`、`int`型など)と同じサイズになるようにパディングがなされるが、ほかにもパディングの条件（構造体の宣言回数？、その時のスタックの使用の様子？など）があるようにおもわれる。
