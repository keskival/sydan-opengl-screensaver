#ifndef HIUTALE_HH
#define HIUTALE_HH

#include "common.hh"

struct CTriangle * iterate_sflake(struct Vertex alku, struct Vertex loppu,
				  struct CTriangle* next, int kierros);

void generate_flake(void);

void draw_flake(int detail);

#endif
