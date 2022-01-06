#ifndef f_print.h
#define f_print.h

#include <MBR_print.h>

void print1()
{
  print_int("joySW", joySW);
  print_int("armState", armState);
  print_int("mode", mode);
  print_int("mode_last", mode_last);
  print_int("initalize_pos", initialize_pos);
  print_int("target_x", target_x);
  print_int("target_y", target_y);
  print_int("brightness", brightness);
  print_int("mirror_state", mirror_state);
  print_ln();
}

#endif
