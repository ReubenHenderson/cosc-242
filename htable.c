#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

struct htablerec{
    int capacity;
    int num_keys;
    char **keys;
    int *frequencies;
    int *stats;
    hashing_t method;
};

static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;
    while (*word != '\0') {
        result = (*word++ + 31 * result);
    }
    return result;
}

static unsigned int htable_step(htable h, unsigned int i_key) {
    if(h->method == DOUBLE_H){
        return 1 + (i_key % (h->capacity - 1));
    }else{
        return 1;
    }
}

htable htable_new(int capacity, hashing_t method) {
    int i;
    htable result = emalloc(sizeof * result);
    result->method = method;
    result->capacity = capacity;
    result->num_keys = 0;
    result->frequencies = emalloc(result->capacity * sizeof result->frequencies[0]);
    result-> keys = emalloc(result->capacity * sizeof result->keys[0]);
    result->stats = emalloc(result->capacity * sizeof result->stats[0]);
  

    for(i = 0; i <capacity; i++){
        result->keys[i] = NULL;
        result->frequencies[i] = 0;
        result->stats[i] = -1;/*set default value to 0 or -1?*/
    }
    return result;
}

int htable_insert(htable h, char *str){
    unsigned int word_int = htable_word_to_int(str);
    unsigned int word_index = word_int % h->capacity;

    unsigned int step = htable_step(h, word_index);
    int collisions = 0;

    /*if nothing here*/
    if(h->keys[word_index] == NULL){
        h->keys[word_index] = emalloc((strlen(str) + 1) * sizeof (char));
        strcpy(h->keys[word_index], str);
        h->frequencies[word_index] += 1;
        h->num_keys++;
        /*no collision*/
        /*Dont know if need cos initialised to 0 anyways*/
        h->stats[h->num_keys] = 0;

        return 1;

    }else if(strcmp(h->keys[word_index], str) == 0){/*if word here, but is the same word*/
        h->frequencies[word_index] += 1;
        return h->frequencies[word_index];
    }else{
        /*if word here and not the same word*/
        unsigned int i;
    
        /*add step to index to check again*/
        i = (word_index+step)%h->capacity;
        collisions++;
        /*loop through table until i gets back around to index*/
        while(i != word_index){
            /*found an empty space to put the word*/
            if(h->keys[i] == NULL){
                h->keys[i] = emalloc((strlen(str) + 1) * sizeof (char));
                strcpy(h->keys[i], str);
                h->frequencies[i] += 1;
                h->num_keys++;
                h->stats[h->num_keys] = collisions;
                return 1;
            }else if(strcmp(h->keys[i], str) == 0){/*found a space with the same word*/
                h->frequencies[i] += 1;
                return h->frequencies[i];
            }
            /*Increment i by step*/
            i = (i + step)%h->capacity;
            collisions++;
        }
        return 0;
    }
    return 0;
}

void htable_print(htable h, FILE *stream){
    int i;
    for(i = 0;i < h->capacity;i++){
        if(h->keys[i] != NULL){
            fprintf(stream, "%d of %s\n", h->frequencies[i], h->keys[i]);
        }
    }
}

void htable_print_entire_table(htable h, FILE *stream){
    int i;
    fprintf(stream,"%5s %5s %5s   %s\n", "Ind", "Freq", "Stats", "Key");
    fprintf(stream, "-----------------------------------------------------\n");
    for(i = 0;i<h->capacity;i++){
        fprintf(stream, "%5d %5d %5d   %s\n", i, h->frequencies[i], h->stats[i], h->keys[i] == NULL ? "NULL" : h->keys[i]);
    }
}

int htable_search(htable h, char *key){
    int collisions = 0;
    int index = htable_word_to_int(key) % h->capacity;
    
    unsigned int step = htable_step(h, index);
    
    while (h->keys[index] != NULL && collisions < h->capacity && strcmp(h->keys[index], key) != 0){
        index = (index + step) % h->capacity;
        collisions ++;
    }
    if (collisions == h->capacity){
        return 0;
    }else {
        return h->frequencies[index];
    }
}


void htable_free(htable h){
    int i;
    for(i = 0; i < h->capacity; i++){
        free(h->keys[i]);
    }
    free(h->stats);
    free(h->keys);
    free(h->frequencies);
    free(h);
}

/* Provided code */

static void print_stats_line(htable h, FILE *stream, int percent_full) {
    int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            }
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }

        fprintf(stream, "%4d %10d %10.1f %10.2f %11d\n", percent_full,
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}

void htable_print_stats(htable h, FILE *stream, int num_stats) {
    int i;

    fprintf(stream, "\nUsing %s\n\n",
            h->method == LINEAR_P ? "Linear Probing. " : "Double Hashing.");
    fprintf(stream, "Percent   Current   Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries   At Home   Collisions   Collisions\n");
    fprintf(stream, "-----------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "-----------------------------------------------------\n\n");
}
