template <typename AUTO> 
struct Widget {
  Widget(AUTO arg) {};   // deduction succeeds
  Widget(double arg1, double arg2) {}; // deduction fails
};
Widget(double, double) -> Widget<int>;

int main(){
    Widget w(1.0,1.0);
}