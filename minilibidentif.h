/*
MIT/X Consortium License

© 2007-2008 Mirko Mariotti <mirko dot mariotti at fisica dot unipg dot it>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software. 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
*/


/* Definisce tutte le funzioni di calcolo combinatorio necessarie al programma per funzionare */
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
