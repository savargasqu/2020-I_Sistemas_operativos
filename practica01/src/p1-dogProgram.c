#include "../p1-dogProgram.h" // For ccls. Make handles dependencies

int main() {
  //Table ht = generate_table();
  Table ht = load_table();
  display_menu(ht);
  save_table(ht);
  return 0;
}
