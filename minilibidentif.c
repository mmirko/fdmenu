/*MIT/X Consortium License

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
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <malloc.h>

#include "minilibidentif.h"

double * getgr(element * num1, element * num2, int max)
/* This is the function exctracting the probabilities (the sums of) relative to an offset when confronting two compound elements */
{
	/*Incudi svariati check */
	int i,offset;
	int maxelem,minelem;
	double * result;

        result=(double *) malloc(max*sizeof(double));

	element * current1;
	element * current2;

	maxelem=num1->howmanyinthecircle;
	minelem=num1->howmanyinthecircle;

	if (num2->howmanyinthecircle>maxelem) maxelem=num2->howmanyinthecircle;
	if (num2->howmanyinthecircle<minelem) minelem=num2->howmanyinthecircle;

	for (offset=0;offset<maxelem;offset++)
	{
		if (num1->howmanyinthecircle>=num2->howmanyinthecircle)
		{
			current1 = num1->leaf;
			current2 = num2->leaf;
		}
		else
		{
			current1 = num2->leaf;
			current2 = num1->leaf;
		}

		for (i=0;i<offset;i++)
		{
			current1 = current1->next;
		}

		*(result+offset)=0;
		for (i=0;i<minelem;i++)
		{
			*(result+offset)=*(result+offset)+find_prob(current1,current2);
			current1 = current1->next;
			current2 = current2->next;
		}
	}
	return result;
}

double find_prob(element * num1, element * num2)
/* The most important function, calculates the probability between two elements */
{

	double result;
	
	if ((num1->type==EL_CHAR)&&(num2->type==EL_CHAR))
	{
		result=char_char(num1,num2);
		return(result);
	}	
	if ((num1->type==EL_STRING)&&(num2->type==EL_STRING))
	{
		result=string_string(num1,num2);
		return(result);
	}
	
	/* There is no matching: samething has gone bad, let's assume a zero probaility*/
	result=0.0;
	return(result);
}

double fatt(double arg)
/* this is the factorial function extended to the non integer numbers */
{
	if (floor(arg)==arg)
	/* k is an integer number */
	{
		double aux=1;
		for(;arg>0;arg--)
		{
			aux=aux*arg;
		}
		return(aux);
	}
	else
	/* approssima i arg non interi con la retta passante per i due interi piu' vicini con y=(x-i)*(fatt(i+1)-fatt(i))-fatt(i) */
	{
		double aux;
		double min,max;
		min=floor(arg);
		max=ceil(arg);
		aux=arg-min;
		max=fatt(max);
		min=fatt(min);
		return(aux*(max-min)+min);
	}
}

double binom(double n, double k)
{
	if (floor(k)==k)
	/* gestisce i numeri con k intero */
	{
		double i;
		double aux=1;
		for(i=n;i>n-k;i--)
		{
			aux=aux*i;
		}
		return(aux/fatt(k));	
	}
	else	
	/* approssima i k non interi con la retta passante per i due interi piu' vicini con y=(x-i)*(binom(n,i+1)-binom(n,i))-binom(n.i) */
	{
		double aux;
		double min,max;
		min=floor(k);
		max=ceil(k);
		aux=k-min;
		max=binom(n,max);
		min=binom(n,min);
		return(aux*(max-min)+min);
	}
}

double char_char (element * num1, element * num2)
/* This is the matching function for a char-char match */
{
	double result;
	
	if (!strcmp(num1->value,num2->value))
	{
		result=1.0;
	}
	else
	{
		result=0.0;
	}
		
	return (result);
}

double string_string (element * num1, element * num2)
/* This is the matching function for a string-string match */
{
	double result;
	
	int i;
	int max;
	double aux;
	double prob;
	double * graph;
		
	max=num1->howmanyinthecircle;
	if (num2->howmanyinthecircle>max) max=num2->howmanyinthecircle;
		
	graph=getgr(num1,num2,max);
		
	prob=1.0;
		
	for (i=0;i<max;i++)
	{
		aux=binom(max,*(graph+i)) * pow(0.035,*(graph+i)) * pow(1-0.035,max-(*(graph+i)));
		if (aux <= 0.05)
		{
			prob=prob*aux;
		}
	}
	result=1-prob;
	free(graph);
	return (result);
}

element * str2elem (char * input_string)
/* this function creates an element starting from a string */
{
	int i;
	int len;
	element ** chars;
	element * string;

	/* Space for the string */
	string=(element *) malloc(sizeof(element));
	
	/* Space for the chars */
	len=strlen(input_string);
	chars=(element **) calloc(len,sizeof(element *));
	

	for (i=0;i<len;i++)
	{
		*(chars+i)=(element *)malloc(sizeof(element));
		
		(*(chars+i))->isbasic=0;
		(*(chars+i))->type=EL_CHAR;
		(*(chars+i))->value=(char *) malloc(2*sizeof(char));
		strncpy((*(chars+i))->value,input_string+i,1);
		*(((*(chars+i))->value)+1)=0;
		(*(chars+i))->leaf=NULL;
		(*(chars+i))->howmanyinthecircle=0;
	}

	/* Set the right next elements for characters */
	for (i=0;i<len-1;i++)
	{
		(*(chars+i))->next=(*(chars+i+1));
	}
	(*(chars+len-1))->next = *(chars) ;

	/* The value for the string element */
	
	string->isbasic=1;
	string->type=EL_STRING;
	string->value=NULL;
	string->leaf=*(chars);
	string->howmanyinthecircle=len;
	string->next=NULL;

	free(chars);

	return(string);
}

void destroy_element(element * el)
{
	int i;
	element * current;
	element * thenext;
	
	if (el->leaf!=NULL)
	{
		current=el->leaf;
		for (i=0;i<el->howmanyinthecircle;i++)
		{
			thenext=current->next;
			destroy_element(current);
			current=thenext;
		}
	}

	if (el->value!=NULL)
	{
		free(el->value);
	}
	free (el);
}
