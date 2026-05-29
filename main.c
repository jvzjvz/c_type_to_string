#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// NOTE: Assumes the following coding convention for struct declarations
    // typedef struct Foo
    // {
    //   Bar bar;
    //   Baz baz;
    //   int num;
    // } Foo;

// Handle {
//   index = 0,
//   generation = 0,
//   random = Random {
//     x = 5,
//     y = 10,
//   },
// }
    

#define SPACES_PER_INDENT 2
#define MAX_LINE_LEN 128
#define MAX_TYPES 100
#define MAX_TYPE_NAME_LENGTH 64

char types[MAX_TYPES][MAX_TYPE_NAME_LENGTH];
size_t types_count;

char formatted_string[1024];
size_t formatted_string_len = 0;
int indentation_level = 0;

const char* builtin_types[] = {
    "char",
    "bool",
    "int",
    "float",
    "double",
};

typedef enum Field_Type
{
  Char,
  Bool,
  Int,
  Float,
  Double,
  BUILTIN_TYPE_COUNT,
  Custom,
} Field_Type;

Field_Type get_field_type(char* field_type)
{
  if (strcmp(field_type, "char") == 0) return Char;
  if (strcmp(field_type, "bool") == 0) return Bool;
  if (strcmp(field_type, "int") == 0) return Int;
  if (strcmp(field_type, "float") == 0) return Float;
  if (strcmp(field_type, "double") == 0) return Double;
  return Custom;
};

const char* BUILTIN_TYPE_FORMATTED_STRINGS[BUILTIN_TYPE_COUNT] =
{
  [Char] = "%s = %%c,\n",
  [Bool] = "%s = %%d,\n",
  [Int] = "%s = %%d,\n",
  [Float] = "%s = %%f,\n",
  [Double] = "%s = %%f,\n",

  // [Char] = "%%s = %%c,\n",
  // [Bool] = "%%s = %%d,\n",
  // [Int] = "%%s = %%d,\n",
  // [Float] = "%%s = %%f,\n",
  // [Double] = "%%s = %%f,\n",
};

void write_field_to_formatted_string(char* field_type, char* field_name)
{
  Field_Type type = get_field_type(field_type);
  // printf("Hasd;flkasjdfaskdl\n");
  if (type == Custom)
  {
    // ...
    fprintf(stderr, "TODO: implement custom types");
    exit(EXIT_FAILURE);
  }

  const char* field_formatted_string = BUILTIN_TYPE_FORMATTED_STRINGS[type];

  // size_t bytes_written = strlen(field_formatted_string);
  // memcpy(formatted_string + formatted_string_len, field_formatted_string, 
  //   bytes_written);
  // formatted_string_len += bytes_written;
  // formatted_string[formatted_string_len] = '\0';

  // "%s = %d"
  // "field_name = %d"

  int indentation_in_spaces = indentation_level * SPACES_PER_INDENT;
  for (int i = 0; i < indentation_in_spaces; i++)
  {
    formatted_string[formatted_string_len++] = ' ';
  }
  formatted_string[formatted_string_len] = '\0';

  int remaining_buffer_size = sizeof(formatted_string) - formatted_string_len;
  assert(remaining_buffer_size > 0);

  int bytes_written = snprintf(formatted_string + formatted_string_len, 
      remaining_buffer_size, field_formatted_string, field_name);
  assert(bytes_written > 0);
  formatted_string_len += bytes_written;

  formatted_string[formatted_string_len] = '\0';
  printf("formatted_string: \n%s", formatted_string);
}

bool get_line(FILE* file, char* line) 
{
  assert(line);
  if (!fgets(line, MAX_LINE_LEN, file)) return false;
  line[strlen(line) - 1] = '\0';
  return true;
}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "No arguments provided.\n");
    return EXIT_FAILURE;
  }

  char* input_file = argv[1];
  // printf("%s\n", input_file);

  const char* type_name_declaration_prefix;

  if (strstr(input_file, ".cpp"))
  {
    type_name_declaration_prefix = "struct ";
  } 
  else if (strstr(input_file, ".c"))
  {
    type_name_declaration_prefix = "typedef struct ";
  }
  else
  {
    fprintf(stderr, "Not a .c or .cpp file!\n");
    return EXIT_FAILURE;
  }

  int num_types = argc - 2;
  if (num_types <= 0)
  {
    fprintf(stderr, "No provided types", input_file);
    return EXIT_FAILURE;
  }
  char* types_to_generate = argv[num_types];

  FILE* source_file = fopen(input_file, "r");
  if (!source_file)
  {
    fprintf(stderr, "Failed to open source file: %s", input_file);
    return EXIT_FAILURE;
  }

  char line[MAX_LINE_LEN];
  int line_number = 0;


  while (fgets(line, MAX_LINE_LEN, source_file))
  {
    if (!strstr(line, type_name_declaration_prefix)) continue;

    bool commented = line[0] == '/';
    if (commented) continue;

    size_t line_len = strlen(line) - 1;
    line[line_len] = '\0';

    bool forward_declaration = line[line_len - 1] == ';'; 
    if (forward_declaration) continue;

    char* type_name = line + strlen(type_name_declaration_prefix);

    printf("%s\n", type_name);
    strcpy(types[types_count], type_name);
    types_count++;

    get_line(source_file, line);
    assert(strcmp(line, "{") == 0);

    indentation_level++;

    for (;;)
    {
      bool ok = get_line(source_file, line);
      if (strcmp(line, "};") == 0) break;

      // removes trailing ;
      line[strlen(line) - 1] = '\0';

      char* field_ptr = line;

      static char field_type[512];
      field_ptr = strtok(field_ptr, " "); assert(field_ptr);
      strcpy(field_type, field_ptr);

      static char field_name[512];
      field_ptr = strtok(NULL, " "); assert(field_ptr);
      strcpy(field_name, field_ptr);

      write_field_to_formatted_string(field_type, field_name);
      // goto quit;

      // printf("%s = %%d,\n", field_name);
    }
    line_number++;
    // printf("%s", formatted_string);
  }

// quit:

  // input file
  
  // search for some macro mp_tostr(type, var) in input file
    // mp_tostr(Foo, var);

  // find its type declaration
    // typedef struct Foo
    // {
    //   Bar bar;
    //   Baz baz;
    //   int num;
    // } Foo;

  // build string for that type

    // char* string = 
    // "Foo\n"
    // "{\n"
    //     "bar = Bar {...},\n" // recursively for subfields that are 
    //                          // also custom structures
    //     "baz = Baz {...},\n"
    //     "num = 5,\n"
    // "}\n";

  // replace that mp_to_str(type) with the string
}
