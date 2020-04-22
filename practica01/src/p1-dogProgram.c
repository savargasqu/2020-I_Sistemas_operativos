#include "../p1-dogProgram.h" // For ccls. Make handles dependencies

int main() {
  Table ht = load_table();
  display_menu(ht);
  save_table(ht);
  return 0;
}

int display_menu(Table ht) {
  struct dogType *dog_record; // Pointer to structure
  int menu_selection = -1;
  char wait;

  while (1) {
    system("clear"); // "cls" for windows
    printf("Veterinaria:\n1. Ingresar registro\n2. Ver registro\n"
           "3. Borrar registro\n4. Buscar registro\n5. Salir\n"
           "Ingrese el número de una opción: ");
    scanf("%d", &menu_selection);

    switch (menu_selection) {
    case 1: // Ingresar registro
      create_record(ht);
      break;
    case 2: // Ver registro
      view_record(ht);
      break;
    case 3: // Borrar registro
      delete_record(ht);
      break;
    case 4: // Buscar registro
      search_record_by_name(ht);
      break;
    case 5: // Salir
      return 0;
      break;
    default:
      printf(INPUT_WARNING);
      break;
    }
    printf("Presione cualquier tecla para continuar.\n");
    scanf(" %c", &wait); // works as getchar()
  }
}

void create_record(Table ht) {
  struct dogType *dog_ptr = (struct dogType *)malloc(sizeof(struct dogType));
  if (dog_ptr == NULL) {
    perror(MEMORY_ERROR);
  }
  request_data(dog_ptr); // Write the necessary information in the structure
  unsigned long id = insert_to_table(ht, dog_ptr->name, *dog_ptr);
  if (id > 0) {
    printf("Nuevo registro: %lu\n", id);
  } else {
    printf("No se pudo crear un nuevo registro\n");
  }
}

void view_record(Table ht) {
  int view_selection;
  Node node = search_by_id(ht);
  if (node != NULL) {
    while (1) {
      printf("Registro %lu\n1. Ver datos\n2. Ver historia clínica\n",
             node->dog.ID);
      scanf("%d", &view_selection);
      if (view_selection == 1) {
        display_dog_data(&(node->dog)); // Argument is dogType
        break;
      } else if (view_selection == 2) {
        display_clinical_history(node->dog.ID); // Argument is ID
        break;
      } else {
        printf(INPUT_WARNING);
      }
    }
  }
}

void delete_record(Table ht) {
  Node temp = search_by_id(ht);
  if (temp != NULL) {
    unsigned long id = temp->dog.ID;
    delete_in_table(ht, temp);
    printf("Se ha eliminado el registro %lu\n", id);
    return;
  }
  return;
}

void search_record_by_name(Table ht) {
  char search_name[32];
  printf("Escriba el nombre a buscar: ");
  scanf("%s", search_name);
  string_lower_case(search_name);
  unsigned int count = search_keys_in_table(ht, search_name);
  printf("Se encontraron %u registros\n", count);
}

/* AUXILIARY FUNCTIONS */

void request_data(struct dogType *dog_record) {
  fflush(stdin);
  printf("Por favor ingrese los datos solicitados.\n");
  printf("Nombre: ");
  char *temp;
  scanf("%s", dog_record->name);
  string_lower_case(dog_record->name);
  printf("Tipo: ");
  scanf("%s", dog_record->species);
  printf("Edad (en años): ");
  scanf("%i", &dog_record->age);
  printf("Raza: ");
  scanf("%s", dog_record->breed);
  printf("Estatura (cm): ");
  scanf("%i", &dog_record->height);
  printf("Peso (Kg): ");
  scanf("%f", &dog_record->weight);
  printf("Sexo (H/M): ");
  scanf(" %c", &dog_record->sex);
}

void display_dog_data(struct dogType *dog_ptr) {
  printf("Nombre: %s\n", dog_ptr->name);
  printf("  Tipo: %s\n", dog_ptr->species);
  printf("  Edad: %i\n", dog_ptr->age);
  printf("  Raza: %s\n", dog_ptr->breed);
  printf("Altura: %i\n", dog_ptr->height);
  printf("  Peso: %.3f\n", dog_ptr->weight);
  printf("  Sexo: %c\n", dog_ptr->sex);
}

void display_clinical_history(unsigned long ID) {
  char *file_name;
  FILE *text_file;
  sprintf(file_name, "%lu.txt", ID); // File name is dog's ID
  text_file = fopen(file_name, "w"); // Write file in binary format
  if (text_file == NULL) {           // If pointer is NULL, fopen failed
    perror(OPEN_ERROR);
    exit(-1);
  } else {
    char command[64];
    sprintf(command, "open %s", file_name);
    system(command);
  }
}

Node search_by_id(Table ht) {
  unsigned long id; // ID of dogType record
  printf("La tabla tiene %lu registros\n", ht->size);
  printf("Ingrese el ID de un registro: ");
  scanf("%lu", &id);
  if (id >= 0) {
    Node node = search_value_in_table(ht, id);
    if (node != NULL) {
      return node;
    }
  }
  printf("El registro %lu no existe\n", id);
  return NULL;
}

void string_lower_case(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = tolower(str[i]);
  }
}
