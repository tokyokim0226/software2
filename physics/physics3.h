// シミュレーション条件を格納する構造体
// 反発係数CORを追加
typedef struct condition
{
  int width; // 見えている範囲の幅
  int height; // 見えている範囲の高さ
  double G; // 重力定数
  double dt; // シミュレーションの時間幅
  double cor; // 壁の反発係数
  int bar_x; //下の棒の初期位置
} Condition;

// 個々の物体を表す構造体
typedef struct object
{
  double m;
  double y;
  double prev_y; // 壁からの反発に使用
  double vy;
  double prev_vy;
  double x;        //   ここから追加
  double prev_x;
  double vx;
  double prev_vx;
} Object;

typedef struct stage{
    int block_number;
    int block_position_x[10];
    int block_position_y[10];
    int block_width[10];
    int block_height[10];
} Stage;

// 授業で用意した関数のプロトタイプ宣言

// void plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond);
// void update_velocities(Object objs[], const size_t numobj, const Condition cond);
// void update_positions(Object objs[], const size_t numobj, const Condition cond);
// void bounce(Object objs[], const size_t numobj, const Condition cond);

// To do: 以下に上記と同じ引数で実行される my_* を作成
// 実装できたらmain関数で上記と入れ替えていく
// my_plot_objects(), my_update_velocities(), my_update_positions(), my_bounce の4つ 

int kbhit(void);
int select_stage(Object objs[], const size_t numobj,const Condition cond, Stage stage[],int bar_x);
double init_stage(Object objs[], const size_t numobj,const Condition cond, Stage stage[],int bar_x);
int my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond, Stage stage[],int num_stage,int bar_x);
void my_update_velocities(Object objs[], const size_t numobj, const Condition cond);
void my_update_positions(Object objs[], const size_t numobj, const Condition cond);
void my_bounce(Object objs[], const size_t numobj, const Condition cond, Stage stage[], int selected_stage,int bar_x);
void my_judge_results(Object objs[], const size_t numobj, const double t, const Condition cond, Stage stage[],int num_stage,int bar_x);



