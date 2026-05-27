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


#define MAX_LINE_LEN 128
#define MAX_TYPES 100
#define MAX_TYPE_NAME_LENGTH 64

char types[MAX_TYPES][MAX_TYPE_NAME_LENGTH];
int types_count;

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

    for (;;)
    {
      bool ok = get_line(source_file, line);
      if (strcmp(line, "};") == 0) break;
      printf("field: %s\n", line);
    }

    line_number++;
  }

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
