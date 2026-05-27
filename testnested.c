typedef struct Handle
{
  int index;
  int generation;
};

typedef struct Random_Data
{
  int x;
  Handle handle;
};

// expected output

// output formatt string
Random_Data
{
  int x = %d,
  Handle handle =
  {
    int index = %d,
    int generation = %d,
  },
}

// output macro
#define mp_random_data_tostr(x, handle_index, handle_generation)
  printf(output_format_string, x, handle_index, handle_generation)
