#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdio.h>

typedef enum hashing_e {LINEAR_P, DOUBLE_H } hashing_t;
typedef struct htablerec *htable;

extern void htable_free(htable h);/*free memory*/
extern int htable_insert(htable h, char *str);/*insert value*/
extern htable htable_new(int capacity, hashing_t hash);/*create new table*/
extern void htable_print(htable h, FILE *stream);/*write to file ir stdout*/
extern void htable_print_stats(htable h, FILE *stream, int num_stats);
extern void htable_print_line(htable h, FILE *stream, int percent_full);
extern void htable_print_entire_table(htable h, FILE *stream);
extern int htable_search(htable h, char *str);/*find value in table*/

#endif
