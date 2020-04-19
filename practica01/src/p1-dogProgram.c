#include "../p1-dogProgram.h" // For ccls. Make handles dependencies

// static Table ht;

int main() {
  Table ht = create_table();
  // ht = serialize(ht, 'r');
  display_menu(ht);
  // serialize(ht, 'w');
  return 0;
}
