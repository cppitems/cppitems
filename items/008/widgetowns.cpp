#include <utility>
struct WidgetOwns {
  double *data;
    double d;
  int size;
  WidgetOwns(int size) : size(size), data(new double[size]), d(*data) {}
  ~WidgetOwns() { delete[] data;}
};


int main(){
    WidgetOwns w1(1);
    WidgetOwns w2(w1); // copy-construction
    w1 = w2; // copy-assignment
}