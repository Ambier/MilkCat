//
// The MIT License (MIT)
//
// Copyright 2013-2014 The MilkCat Project Developers
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// keyphrase.c --- Created at 2014-04-19
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/milkcat.h"

// Max buffer size 1s 10MB
const int kBufferSize = 10 * 1024 * 1024;

int main(int argc, char **argv) {
  char *buf = NULL;
  FILE *fd = NULL;
  milkcat_keyphrase_t *keyphrase = NULL;
  milkcat_model_t *model = NULL;
  keyphrase_item_t *item = NULL;
  bool success = true;
  int text_size;

  if (argc != 2) {
    fprintf(stderr, "Usage: milkcat-keyphrase TEXT-FILE\n");
    return 1;
  }

  model = milkcat_model_new(NULL);
  keyphrase = milkcat_keyphrase_new(model, MILKCAT_KEYPHRASE_DEFAULT);

  if (!keyphrase) {
    fprintf(stderr, "%s\n", milkcat_last_error());
    return 1;
  }

  buf = (char *)malloc(kBufferSize);
  fd = fopen(argv[1], "r");
  if (fd) {
    text_size = fread(buf, 1, kBufferSize - 1, fd);
    buf[text_size] = '\0';

    item = milkcat_keyphrase_extract(keyphrase, buf);
    while (item) {
      printf("%s %lf\n", item->keyphrase, item->weight);
      item = milkcat_keyphrase_extract(keyphrase, NULL);
    }
  }

  free(buf);
  milkcat_keyphrase_destroy(keyphrase);
  milkcat_model_destroy(model);

  return 0;
}