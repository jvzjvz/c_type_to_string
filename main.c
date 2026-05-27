#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TYPES 100
#define MAX_TYPE_NAME_LENGTH 64

char types[MAX_TYPES][MAX_TYPE_NAME_LENGTH];
int types_count;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "No arguments provided");
    return EXIT_SUCCESS;
  }

  char* input_file = argv[1];
  printf("%s\n", input_file);

  int num_types = argc - 2;
  if (num_types <= 0)
  {
    fprintf(stderr, "No provided types", input_file);
    return EXIT_SUCCESS;
  }
  char* types_to_generate = argv[num_types];

  FILE* source_file = fopen(input_file, "r");
  if (!source_file)
  {
    fprintf(stderr, "Failed to open source file: %s", input_file);
    return EXIT_SUCCESS;
  }

  char line[128];
  int i = 0;

  while (fgets(line, sizeof(line), source_file))
  {

    if (!strstr(line, "struct")) continue;

    bool commented = line[0] == '/' && line[1] == '/';
    if (commented) continue;

    // might cause bug in future, add some bound checks
    bool forward_declaration = line[strlen(line) - 2] == ';'; 
    if (forward_declaration) continue;

    char* type_name = line + strlen("struct ");
    printf("%s\n", type_name);
    i++;
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
