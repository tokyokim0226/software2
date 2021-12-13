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
s1.id:0x7ffc804acb70
s1.name:0x7ffc804acb74
s1.age:0x7ffc804acbd8
s1.height:0x7ffc804acbe0
s1.weight:0x7ffc804acbe8
s_begin.id:0x7ffc804acbf0
s_begin.name:0x7ffc804acbf4
s_begin.age:0x7ffc804acc58
s_begin.height:0x7ffc804acc60
s_begin.weight:0x7ffc804acc68
```
`id`が4bytes、`s_begin`の`name`配列が340bytes、`s1`の`name`配列が100bytes、`age`が8bytes、`height`が8bytes、`weight`が8bytes使っていることがわかる。
`age`の後にパディングが4bytesあることがわかる。(`sizeof()`を使うと、`age`は4bytesになっているので、8-4=4)
また`s_begin`の`name`配列においてもサイズが4の倍数になるようにパディングが240bytesあることがわかる。
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
s2.id:0x7ffc804acae0
s2.name:0x7ffc804acae4
s2.age:0x7ffc804acb48
s2.height:0x7ffc804acb50
s2.weight:0x7ffc804acb58
s2.tag:0x7ffc804acb60
````
`id`が4bytes、`name`配列が340bytes、`age`が8bytes、`height`が8bytes、`weight`が8bytes、`tag`が（前述のs1の先頭アドレスとの比較から）16bytes使っていることがわかる。
`age`の後にパディング4bytes、`name`配列の後にパディングが存在している。
また構造体`Student`との差は、最後の`char`型の`tag`のあるなしであるが、`tag`の後にパディングが15bytes存在している。
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
を用いて構造体`s3,s_end`を作成した。
構造体のサイズと、そのメンバのアドレスは以下のようになった。
```
s3:128
s_end:128
```
```
s_end.tag:0x7ffc804ac9e0
s_end.id:0x7ffc804ac9e4
s_end.name:0x7ffc804ac9e8
s_end.age:0x7ffc804aca4c
s_end.height:0x7ffc804aca50
s_end.weight:0x7ffc804aca58
s3.tag:0x7ffc804aca60
s3.id:0x7ffc804aca64
s3.name:0x7ffc804aca68
s3.age:0x7ffc804acacc
s3.height:0x7ffc804acad0
s3.weight:0x7ffc804acad8
```
`tag`が4bytes,`id`が4bytes、`s_end`の`name`配列が340bytes、`s3`の`name`配列が100bytes(1x100)、`age`が4bytes、`height`が8bytes、`weight`が8bytes、使っていることがわかる。
`tag`の後にパディングが3bytes、一回目の`name`配列の後にパディングが存在している。
また構造体`Student1`との差は`char`型の`tag`の場所であるが、`tag`の後のパディングが減っていることが確認できる。

## 構造体のアドレス
構造体の先頭アドレスは以下のようになっていた。
```
s_end:0x7ffc804ac9e0
s3:0x7ffc804aca60
s2:0x7ffc804acae0
s1:0x7ffc804acb70
s_begin:0x7ffc804acbf0
```
構造体の先頭アドレスと各構造体の最初のメンバのアドレスが等しいことがわかる。

## まとめ
構造体`Student`においては、`double`の前の`int`に4bytes、二回目の構造体使用時の`char`型配列にパディングがなされていた。

構造体`Student`の最後のメンバに`char`型の`tag`を足した構造体`Student1`では、`double`の前の`int`に4bytes、doubleの後の`char`に15bytes、`char`配列にパディングがなされていた。

構造体`Student1`において`tag`を最初のメンバにした構造体`Student2`においては、`int`の前の`char`に3bytes、一回目の構造体使用時の`char`配列にパディングがなされていた。
また`double`の前の`int`に対するパディングはなくなっていた。

以上のことをまとめると、基本は近く（特に後ろ）のメンバ変数(`double`、`int`型の)と同じサイズになるようにパディングがなされるが、ほかにもパディングの条件（構造体の宣言回数？、その時のスタックの使用の様子？など）があるようにおもわれる。
