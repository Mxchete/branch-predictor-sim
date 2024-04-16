#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// mainly used for debugging issues I had with the cache
#define MORE_PRINTS

// just for fun
#define TRACK_TIME

#ifdef TRACK_TIME
#include <time.h>
#endif

uint32_t gpb_m = 0;
uint32_t rb_n = 0;
uint32_t bhr = 0;

uint64_t num_predicted = 0;
uint64_t num_missed = 0;
uint64_t total = 0;

uint8_t *history_table;

void init_global_history_table(uint64_t num_indices) {
  history_table = calloc(num_indices, sizeof(uint8_t));
  for (int i = 0; i < num_indices; i++) {
    i[history_table] = 0x10;
  }
}

void free_table() { free(history_table); }

void predict_taken(uint64_t index, bool res) {
  if (res) {
    // predict taken, is taken
    index[history_table] |= 0x1;
    num_predicted += 1;
  } else {
    // predict taken, is untaken
    index[history_table] -= 0x1;
    num_missed += 1;
  }
}

void predict_untaken(uint64_t index, bool res) {
  if (res) {
    // predict untaken, is taken
    index[history_table] += 0x1;
    num_missed += 1;
  } else {
    // predict untaken, is untaken
    index[history_table] = 0x0;
    num_predicted += 1;
  }
}

uint64_t gen_mask() {
  uint64_t mask = 0;
  for (int i = 0; i < gpb_m; i++) {
    mask |= (0x1 << i);
  }
  return mask;
}

void predict_branch(uint64_t pc, bool res) {
  uint64_t mask = gen_mask();
  uint64_t index = ((pc >> 2) & mask) ^ (bhr << (gpb_m - rb_n));
  switch (index[history_table]) {
  case 3:
  case 2:
    predict_taken(index, res);
    break;
  case 1:
  case 0:
  default:
    predict_untaken(index, res);
    break;
  }
  bhr = (bhr >> 1) | (res << (rb_n - 1));
}

int main(int argc, char **argv) {
#ifdef TRACK_TIME
  clock_t begin = clock();
#endif
  char res;
  uint64_t pc;
  switch (argc) {
  case 4:
    gpb_m = atoi(1 [argv]);
    rb_n = atoi(2 [argv]);
    uint64_t num_indices = (uint64_t)pow((double)2, (double)gpb_m);
    FILE *file = fopen(3 [argv], "r");
    if (!file) {
      printf(" Error : Could not open the trace file.\n");
      return 1;
    }
    init_global_history_table(num_indices);
    // Read until end of file
    while (!feof(file)) {
      fscanf(file, "%lx %c\n", &pc, &res);

      bool res_to_bit = (res == 't' || res == 'T') ? 1 : 0;
      predict_branch(pc, res_to_bit);
      total += 1;
    }

    double miss_rate = ((double)num_missed / (double)total) * 100;
    printf("%d %d %.2f\n", gpb_m, rb_n, miss_rate);
#ifdef MORE_PRINTS
#endif

    free_table();
    break;

  case 2:
    if (strcmp(1 [argv], "-h") == 0) {
      printf("Program usage guidelines:\n./sim <CACHE_SIZE> <ASSOC> "
             "<REPLACEMENT> <WB> <TRACE_FILE>\nWhere:\n<CACHE_SIZE> is the "
             "size of the simulated cache in bytes,\n<ASSOC> is the "
             "associativity,\n<REPLACEMENT> is the replacement policy: 0 means "
             "LRU and 1 means FIFO,\n<WB> is the Write-back policy: 0 means "
             "write-through and 1 means write-back and\n<TRACE_FILE> denotes "
             "trace file name with full path.\n");
      break;
    }

  default:
    printf("Invalid arguments supplied, the program takes 5 input arguments, "
           "use ./sim -h for argument requirements.\n");
  }
#ifdef TRACK_TIME
  clock_t end = clock();
  double total_exec_time = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Total time spent executing program: %f seconds\n", total_exec_time);
#endif
  return 0;
}
