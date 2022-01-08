#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // 枝刈り部分を効率的にbool型で書くため導入
#include <assert.h>
#include "search.h"
#include "item.h"
 
 // ソルバーは search を index = 0 で呼び出すだけ
Answer solve(const Itemset *list,  double capacity)
{
    // 品物を入れたかどうかを記録するフラグ配列 => !!最大の組み合わせが返ってくる訳ではない!!
    unsigned char *flags = (unsigned char*)calloc(list->number, sizeof(unsigned char));
    Answer solution = search(0,list,capacity,flags, 0.0, 0.0);
    free(flags);
    return solution;
}
 
 // 再帰的な探索関数
Answer search(int index, const Itemset *list, double capacity, unsigned char *flags, double sum_v, double sum_w)
{
    int max_index = list->number;
    assert(index >= 0 && sum_v >= 0 && sum_w >= 0);
    if (index == max_index){
        const char *format_ok = ", total_value = %5.1f, total_weight = %5.1f\n";
        const char *format_ng = ", total_value = %5.1f, total_weight = %5.1f NG\n";
        bool pruned = (sum_w > capacity);
        const char *format = (pruned) ? format_ng : format_ok;
        unsigned char *arg = (pruned) ? NULL : (unsigned char *)malloc(sizeof(unsigned char) * list->number);

        for (int i = 0 ; i < max_index ; i++){
            if ( ! pruned ) arg[i] = flags[i];
            printf("%d", flags[i]);
        }
        printf(format, sum_v, sum_w);
        return (Answer){ .value = (pruned)?0:sum_v, .flags = arg};
    }
 
     // 以下は再帰の更新式: 現在のindex の品物を使う or 使わないで分岐し、index をインクリメントして再帰的にsearch() を実行する
     
    flags[index] = 0;
    const Answer a0 = search(index+1, list, capacity, flags, sum_v, sum_w);

    flags[index] = 1;
    Item item = get_item(list, index);
    bool pruned = sum_w + get_itemweight(item) > capacity;
    const Answer a1 = (pruned)?(Answer){ .value = 0, .flags = NULL}:search(index+1, list, capacity, flags, sum_v + get_itemvalue(item), sum_w + get_itemweight(item));

    // 使った場合の結果と使わなかった場合の結果を比較して返す
    if ( a0.value > a1.value){
        free(a1.flags);
        return a0;
    }
    else{
        free(a0.flags);
        return a1;
    }
 }