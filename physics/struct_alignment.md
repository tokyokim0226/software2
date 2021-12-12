# 課題1
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
