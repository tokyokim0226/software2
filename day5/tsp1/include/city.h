#pragma once
// 町の構造体（今回は2次元座標）を定義
typedef struct
{
  int x;
  int y;
} City;

double distance(City a, City b);
City *load_cities(const char* filename,int *n);
