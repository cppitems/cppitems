struct Widget {
  int m;
  operator int() { return m; }; // (1) conversion function from 'Widget' to 'int'
};
int main() {
  int d = Widget{3};          // implicit conversion used
  int dd = 3 + Widget{3} * 4; // implicit conversion used
}
