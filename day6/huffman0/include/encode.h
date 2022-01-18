#pragma once

typedef struct node Node;
   
struct node{
    int symbol;     //葉ノードなら文字コード、異なるなら-1(ダミーノード)
    int count;      //マージされたカウントまたは葉ノードなら文字等のカウント
    Node *left;
    Node *right;
};

// ファイルをエンコードし木のrootへのポインタを返す
Node *encode(const char *filename);
// Treeを走査して表示する
void traverse_tree(const int depth, const Node *root);

