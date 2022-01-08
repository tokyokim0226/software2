#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "item.h"

// 構造体をポインタで確保するお作法を確認してみよう
Itemset *init_itemset(size_t number, int seed)
{
  Itemset *list = (Itemset*)malloc(sizeof(Itemset));

  Item *item = (Item*)malloc(sizeof(Item)*number);

  srand(seed);
  for (int i = 0 ; i < number ; i++){
    item[i].value = 0.1 * (rand() % 200);
    item[i].weight = 0.1 * (rand() % 200 + 1);
  }
  *list = (Itemset){.number = number, .item = item};
  return list;
}

// itemset の free関数
void free_itemset(Itemset *list)
{
  free(list->item);
  free(list);
}

// 表示関数
void print_itemset(const Itemset *list)
{
  size_t n = list->number;
  const char *format = "v[%d] = %4.1f, w[%d] = %4.1f\n";
  for(int i = 0 ; i < n ; i++){
      Item item = get_item(list, i);
      printf(format, i, get_itemvalue(item), i, get_itemweight(item));
  }
  printf("----\n");
}

Item get_item(const Itemset *list, size_t index)
{
    return list->item[index];
}

double get_itemweight(Item item)
{
    return item.weight;
}

double get_itemvalue(Item item){
    return item.value;
}

Itemset *load_itemset(char *filename){
    FILE *fp;
    if ( (fp = fopen(filename,"rb")) == NULL){
        printf("cannot open file\n");
	    exit(EXIT_FAILURE);
    }

    Itemset *list = (Itemset*)malloc(sizeof(Itemset));

    int number;
    fread(&number, sizeof(int), 1, fp); 
    list->number = number;

    //個数の上限
    const int max_items = 100;
    assert( list->number <= max_items ); // assert で止める      
    assert( list->number > 0); // 0以下も抑止する

    Item *item = (Item*)malloc(sizeof(Item)*list->number);

    for (int i = 0 ; i < list->number ; i++){
        fread(&(item[i].value), sizeof(double), 1, fp); 
    }
    
    for (int i = 0 ; i < list->number ; i++){
        fread(&(item[i].weight), sizeof(double), 1, fp); 
    }

    fclose(fp);

    *list = (Itemset){.number = list->number, .item = item};
    
    return list;
}