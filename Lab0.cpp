// CLab0.c : C Program to convert to C++
//

#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

///////////////////////////////////////////////////////
//
// Container type
//
///////////////////////////////////////////////////////

typedef struct container
{
	// 2d array
	char** strings;
	// the number of strings
	int rows;
} container;

// count the number of lines in the data file
//
//	input:		filename
//	output:		the number of lines in the file
//					note:  each line corresponds to a single passenger
//
int countLines( FILE* in )
{
	int count = 0;
	char line[80];
	if ( in != NULL)
	{
		while ( !feof( in ) )
			if ( fgets( line, 80, in ) ) count++;
		rewind( in );	
	}
	return count;
}

// opens the file and stores the strings
//
//	input:		string of passenger data
//				container to store strings
//
int processFile( container* cp, char* fn )
{
	char line[80];
	FILE* in = fopen( fn,"r" );
	int count = 0;
	if ( in != NULL )
	{
		cp->rows = countLines(in);
		cp->strings = (char**)malloc(sizeof(char*)*cp->rows);
		while ( !feof(in) )
		{
			if ( fgets( line, 80, in ) )
			{
				cp->strings[count] = (char*)malloc(sizeof(char)*strlen(line)+1);
				strcpy(cp->strings[count],line);
				count++;
			}
		}
	}
	else
	{
		printf("Unable to open file %s\n",fn);
		exit(0);
	}
	fclose(in);
	return count;
}

// deletes the strings in the container
//
//	input:		string of passenger data
//	output:		initialized passenger pointer
//
void deleteContainer( container* cp )
{
	int x = 0;
	if (cp->strings)
	{
		for (x=0; x<cp->rows; x++)
			free(cp->strings[x]);
		free(cp->strings);
	}
}

///////////////////////////////////////////////////////
//
// Passenger type
//
///////////////////////////////////////////////////////

typedef struct passenger
{
	// name of passenger
	char* plast;
	char* pfirst;
	// row of passenger
	int  row;
	// seat number in row
	char seatno;
	// flight number
	int flightno;
} passenger;

// initializes a single passenger
//
//	input:		string of passenger data
//	output:		initialized passenger pointer
//
void initPassenger( passenger* p, char* line )
{
	// strtok modifies the line, so make a temporary 
	char tstring[80];
	strcpy(tstring,line);
	char* pl = strtok(tstring,",");
	char* pf = strtok(NULL,",");
	char* pr = strtok(NULL,",");
	char* ps = strtok(NULL,",");
	char* pflight = strtok(NULL,",");
	p->plast = (char *) malloc( strlen( pl )+1 );
	p->pfirst = (char *) malloc( strlen( pf )+1 );
	strcpy( p->plast, pl );
	strcpy( p->pfirst, pf );
	p->row = atoi(pr);
	p->seatno = *ps;
	p->flightno = atoi(pflight);
}

// releases passenger memory
//
//	input:		a passenger
//	output:		none
//
void deletePassenger( passenger *p )
{
	if ( p->seatno != 0 )
	{
		if ( p->plast )
		{
			free( p->plast );
			p->plast = NULL;
		}
		if ( p->pfirst )
		{
			free( p->pfirst );
			p->pfirst = NULL;
		}
	}
}

// displays a passenger
//
//	input:		a passenger
//	output:		none
//
void showPassenger( passenger p )
{
	// note:  passenger flight number is not displayed
	printf( "%s,%s,%d%c\n", p.plast, p.pfirst, p.row, p.seatno );
}

///////////////////////////////////////////////////////
//
// Airline type
//
///////////////////////////////////////////////////////

typedef struct airline
{
	// dynamic array of passengers
	passenger *persons;
	// number of passengers in airline
	int npassengers;
} airline;

// initializes an airline
//
//	input:		airline pointer
//				number of passengers
//	output:		none
//
void initAirline( airline *a, int np )
{
	a->npassengers = 0;
	a->persons = (passenger *) malloc( sizeof( passenger )*np );
}

// releases airline memory
//
//	input:		airline pointer
//	output:		none
//
void deleteAirline( airline *a )
{
	if ( a->persons ) 
		free( a->persons );
}

// reads a passenger data file
//
//	input:		airline pointer
//					file name
//	output:		number of lines read
//
int processData( airline *a, container* cp )
{
	int x = 0;
	int index = 0;
	if ( cp != NULL)
	{
		for (x=0; x<cp->rows; x++)
		{
			initPassenger( &(a->persons[index]), cp->strings[x] );
			index++;
		}
		a->npassengers = index;
	}
	return index;
}

// displays the passengers in an airline
//
//	input:		airline
//	output:		none
//
void showAirline( airline a )
{
	int x = 0;
	for (x=0; x<a.npassengers; x++)
		showPassenger( a.persons[x] );
}


///////////////////////////////////////////////////////
//
// Main program
//
///////////////////////////////////////////////////////

// drives the program flow
void main()
{
		container strings;  // make an object 
		airline* myairline = (airline *) malloc(sizeof(airline));  // make a pointer
		processFile(&strings, "airline0.csv"); // open and store the strings
		initAirline( myairline, strings.rows ); // initialize the airline
		processData( myairline, &strings );  // process the stored data from the file
		showAirline( *myairline ); // display the processed passenger data
		deleteAirline( myairline ); // free any memory allocated by the airline
		deleteContainer( &strings );  // free any memory allocated by the container
		free( myairline ); // free the pointer declared in main
}
