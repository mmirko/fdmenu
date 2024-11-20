/* See LICENSE file for copyright and license details. */

/* Defines all the combinatorial calculation functions necessary for the program to work */
double fatt(double arg);
double binom(double n, double k);

/* ############# defines ############### */

/* The element types */
#define EL_CHAR 1
#define EL_STRING 2

/* ############# data structures ############### */

typedef struct element
/* This is the element definition*/
{
	/* Define is the element is a basic element (0) or a compound element (1) */
	int isbasic;
	
	/* Define the type of the element, useful to identify the right probabilistic function  */
	int type;
	
	/* relevant for basic elements (not generic) */
	/* set a value for a basic element (othervise is NULL) */
	char * value;
	
	/* relevant for compound elements (not generic) */
	struct element * leaf;
	int howmanyinthecircle;
	
	/* Define the next element in a circle */
	struct element * next;
	
}element;

typedef struct patterndb
/* This is the database of the known patterns */
{
        int dbextension;
        /* says how many patterns are in db */

        int * howmanyinthecircle;
        /* says the lenght for any pattern in db */

        element ** pattern;
        /* point to the starting element of each pattern */

}patterndb;


/* ############# engine functions ############### */

double * getgr(element * num1, element * num2, int max);
/* This is the function exctracting the probabilities (the sums of) relative to an offset when confronting two compound elements */

double find_prob(element * num1, element * num2);
/* The most important function, calculates the probability between two elements */

/* ############# utils functions ############### */

element * str2elem (char * input_string);
/* this function creates an element starting from a string */

double char_char (element * num1, element * num2);
double string_string (element * num1, element * num2);
void destroy_element(element * el);
