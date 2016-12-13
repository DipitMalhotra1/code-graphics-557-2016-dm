#ifndef SNAKE_H_
#define SNAKE_H_

class Snake {
 public:
  Snake(int maxlen, int x, int y);
  Snake();
  ~Snake();

  void grab_point();
  bool full();
  bool self_collision(int nX, int nY);
  bool move_direction_to(int x, int y);
  void reset();
  int current_x_At(int i);
  int current_y_At(int i);
  int x();
  int y();

  int initX;
  int initY;
  int len;
  int maxlen;

  const int minlen = 3;
  double ** snake_tail;

 private:
  void init();
};

#endif 
