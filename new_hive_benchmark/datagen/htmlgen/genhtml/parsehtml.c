/* Sample program to parse HTML files in dirs/ subdirectories and
   output URL-count pairs.

   Implementation scans HTML files, extracts URLs and then uses a hash table
   to store information about URL frequency.

   Usage:  parsehtml  (no parameters)
   Depends on docs/ directory generated by the genhtml tool.

   Author: Sam Madden

 */

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <string.h>
#include <openssl/md5.h>
#include <stdlib.h>

#include "hashtable.h"
#include "hashtable_itr.h"


char *dirname = "docs/";

/* Hash table helper functions (see hashtable.c) */

struct hashtable *ht;  //global hash table

static unsigned int         hash_from_key_fn(const unsigned char *k );
static int                  keys_equal_fn ( char *key1, char *key2 );


static unsigned int         hash_from_key_fn(const unsigned char *k ) {
  char md[16];
  MD5(k, (unsigned long)strlen((char *)k), (unsigned char *)&md);
  return *(unsigned int *)md;
}

static int                  keys_equal_fn ( char *key1, char *key2 ) {
  return (strcmp(key1,key2) == 0);
}

/* Filter function for scandir */ 
static int one (const struct dirent *unused) {
  return 1;
}

/* Scan a particular file, extract information about URLs, add them to hash table */

typedef enum {
  LOOKING_FOR_URL,
  EXTRACTING_URL
} s;

void scanfile(char *fname) {
  //  printf ("scanning %s\n", fname);
  int idx = 0;
  char cururl[200];
  int c;
  char *matchstr = "http://"; //string that begins URLs
  int state = LOOKING_FOR_URL;
  int curidx = 0;
  char nextchar = matchstr[curidx];
    
  FILE *f = fopen(fname, "r");

  if (f != NULL) {
    while ((c = fgetc(f)) != EOF) {
      if ((char)c != '\n') {  //skip newlines
	switch (state) {
	case LOOKING_FOR_URL:  //not currently extracting a URL
	  if ((char)c == matchstr[curidx]) {
	    curidx++;
	    if (curidx == strlen(matchstr)) { //found a complete match for the prefix
	      state = EXTRACTING_URL;
	      curidx = 0;
	    }
	  } else {
	    curidx = 0; //character is not in the match string
	  }
	  break;
	case EXTRACTING_URL:  //currently extracting a URL
	  if ((char)c != ' ' && (char)c != '\"' && (char)c != '>') {  //not a URL terminator
	    cururl[curidx++] = (char)c;
	  } else {
	    cururl[curidx] = 0;  //URL ended, add to hash table

	    //printf("Got URL: %s (%d)\n", cururl, hash_from_key_fn(cururl) );
	    char *key = malloc(strlen(cururl) + 1);
	    int *value;
	    strcpy (key, cururl);

	    value = hashtable_search(ht, key);
	    if (value == NULL) {  //new key -- add to table
	      value = malloc(sizeof(int));
	      *value = 1;
	      hashtable_insert(ht, key, value);
	    } else {  //old key -- increment count
	      (*value) = *value+1;
	      free(key);
	    }
	    //printf ("COUNT = %d\n",*value);
	    curidx = 0;
	    state = LOOKING_FOR_URL;
	  }
	  break;
	}
      }
    }
    fclose(f);
  } else {
    perror("couldn't open file.");
  }
}

/* Scan a subdirectory (e.g., docs/0/), calling scanfile on all contained files */
void scansubdir(char *fname) {
  struct dirent **eps;
  int n;
  char subfname[120];

  //  printf("scanning %s\n", fname);
  n = scandir (fname, &eps, (void *)one, alphasort);
  if (n >= 0) {
    int i;
    for ( i= 0; i < n; i++) {
      if (eps[i]->d_type == DT_REG) { //is a regular file
	sprintf(subfname, "%s%s", fname, eps[i]->d_name);
	scanfile(subfname);
      }
    }
  }
  else
    printf ("Couldn't open the directory %s\n", fname);

  free(eps);
}


int main(int argc, char **argv) {
  struct dirent **eps;
  int n;
  char fname[100];

  ht = create_hashtable(16, (void *)hash_from_key_fn,(void *)keys_equal_fn);

  //scan the docs/ directory looking for subdirectories
  n = scandir (dirname, &eps, (void *)one, alphasort);
  if (n >= 0) {
    int i;
    for ( i= 0; i < n; i++) {
      if (eps[i]->d_type == DT_DIR) {
	if (strcmp(eps[i]->d_name,".") != 0 && strcmp(eps[i]->d_name,"..") != 0) {
	  sprintf(fname, "%s%s/", dirname, eps[i]->d_name);
	  scansubdir(fname); //scan each subdirectory
	}
      }
    }
  }
  else
    printf ("Couldn't open the directory %s\n", dirname);
  
  free(eps);

  //iterate through hash table, print out key - value pairs

  struct hashtable_itr *itr;
  itr = hashtable_iterator(ht);
  if (hashtable_count(ht) > 0) {
    do {
      char *kk = (char *)hashtable_iterator_key(itr);
      int *v = (int *)hashtable_iterator_value(itr);
      printf ("%d,%s\n", *v, kk);
    } while (hashtable_iterator_advance(itr));
  }
  return 0;
}