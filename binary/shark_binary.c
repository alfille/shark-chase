/* shark-chase
 * find optimal path to escape a shark
 * see https://github.com/alfille/shark-chase
 * 
 * Paul H Alfille 2025
 * MIT license
 * */

/* Shark from DSK */

/* Use a binary search algorthm
 * D: start with a given delta
 *   T: try delta at each position (+ and - ) and select minimal
 *     if minimal is no change, halve delta and go to D
 *     if minimal is repeat at same location, double delta go to D
 *     else go to T
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

/* Uses Gnuplot for visulaization */

/* Compile:
 * gcc -o shark shark.c -lm
 * */

enum perturb_result { no_change, first_change, second_change } ;
struct trial {
    int index ; // element where change was made or NO_PATH (=PATH_PONTS+1) for none
    int last_index ; // for doubling
    double delta ; // change made to angle of index element
    double segment ; // calculated length of (altered) index element
    double length ; // calculated total length
    double penalty ; // penalty calculated, or PENALTY_UNCALCULATED flag 
} ;

double * radii = NULL ; // distance from center
double * angles = NULL ; // angle from baseline
double * segments = NULL ; // calculated distances between radii/angles
double * shark = NULL ; // smart sharks's angle around circumference 

/* function prototypees */
double angle_restrict( double angle ) ;
void allocate_arrays( void ) ;
void unallocate( void ) ;

void initial_path( struct trial * tr ) ;
void penalty_calc( struct trial * tr ) ;
void accept_trial( struct trial * tr ) ;
double score_calc( struct trial * tr ) ;
void perturb_calc( struct trial * tr ) ;
void copy_trial( struct trial * dest, struct trial * source ) ;
enum perturb_result test_delta( struct trial * tr ) ;
void iterate_delta( struct trial * tr ) ;
void run( void ) ;

void Graph(struct trial * tr) ;

/* Tunable parameters */
double PENALTY_MULT = 1.0 ; // penalty multiplier
char * add_text = NULL ; // for file names
int PATH_POINTS = 100 ; // Number of points (plus 1 for endpoints)
#define NO_PATH (PATH_POINTS + 1)
double SHARK_V = 4.0 ; // relative speed
#define SHARK_INIT_ANGLE M_PI // opposite side
int verbose = 0 ; // More progress reports
#define PENALTY_UNCALCULATED ( (double)-1.0 ) // Not yet calculated
int GENS = 25 ; // Number of halving times for delta

void Ptrial( struct trial * tr ) {
	printf( "ind=%d, del=%g, seg=%g, length=%g, penalty=%g %s\n",tr->index, tr->delta, tr->segment, tr->length, tr->penalty, (tr->penalty==PENALTY_UNCALCULATED) ? "UNC": "CAL" ) ;
}

double angle_restrict( double angle ) {
	static double PI2 = 2 * M_PI ;
	while ( angle <= -M_PI ) {
		angle += PI2 ;
	}
	while ( angle > M_PI ) {
		angle -= PI2 ;
	}
	return angle ;
}

void allocate_arrays( void ) {
	// fencepost allocation -- PATH_POINTS segments with 1 extra for the endpost 
    angles = calloc( PATH_POINTS+1, sizeof( double ) ) ;
    if ( angles==NULL ) {
        exit(1) ;
    }

    segments = calloc( PATH_POINTS+1, sizeof( double ) ) ;
    if ( segments==NULL ) {
        exit(1) ;
    }

    radii = calloc( PATH_POINTS+1, sizeof( double ) ) ;
    if ( radii==NULL ) {
        exit(1) ;
    }

    shark = calloc( PATH_POINTS+1, sizeof( double ) ) ;
    if ( shark==NULL ) {
        exit(1) ;
    }
}

void unallocate( void ) { // never called
    if ( angles ) free( angles ) ;
    if ( segments ) free( segments ) ;
    if ( radii ) free( radii ) ;
    if ( shark ) free( shark ) ;
}

void initial_path( struct trial * tr ) {
	// run straight to the edge
	for ( int i = 0 ; i <= PATH_POINTS ; ++i ) {
		radii[i] = (double) i / PATH_POINTS ;
		angles[i] = 0. ;
	}
	segments[0] = 0 ;
	for ( int i = 1 ; i <= PATH_POINTS ; ++i ) {
		segments[i] = radii[i] - radii[i-1] ;
	}
	
	tr->index = NO_PATH ; // no index ;
	tr->last_index = NO_PATH ;
	tr->delta = M_PI / 4.0 ; // initial delta
	tr-> length = 1.0 ; // straight path radius 1
	tr->penalty = PENALTY_UNCALCULATED ;
}

void penalty_calc( struct trial * tr ) {
	// also computes shark array
	int transition = PATH_POINTS / SHARK_V ;
	double s = SHARK_INIT_ANGLE ;
	shark[0] = s ;
	double p = 0 ; // penalty
	// adjust everything after trial index
	for ( int i = 1 ; i <= PATH_POINTS ; ++i ) {
		// adjusted angle and segment length
		double a ;
		double l = segments[i] ;
		if ( i < tr->index ) {
			a = angles[i] ;
		} else {
			a = angles[i] + tr->delta ;			
			if ( i == tr->index ) {
				l = tr->segment ;
			}
		}
		
		// Shark goes to man's angle upto max speed
		double rem = angle_restrict( a - s ) ; // corrected angular difference
		double max_shark = l * SHARK_V ; // max distance can travel
		s += (rem>=0) ? fmin( rem, max_shark ) : fmax( rem, -max_shark ) ;
		if ( ( i > transition ) && ( fabs( rem ) < .001 ) ) {
			// penalty
			p += l + (i==PATH_POINTS) ;
		}
		shark[i] = s ;
		//printf("angle %g, shark %g, max_shark %g, rem %g, p %g\n",a,s,max_shark,rem,p);
	}
	tr->penalty = p ;
	//printf("Penalty ");
	//Ptrial( tr ) ;
}

double score_calc( struct trial * tr ) {
	// calculate score for minimizing
	if ( tr->penalty == PENALTY_UNCALCULATED ) {
		// calculate penalty if not yet computed
		penalty_calc( tr ) ;
	}
	return tr->length + PENALTY_MULT * tr->penalty ;
}

void accept_trial( struct trial * tr ) {
	// perturbed version will now be baseline
	// leaves delta and penalty unchanged
	if ( tr->index != NO_PATH ) {
		segments[tr->index] = tr->segment ;
		
		for ( int i = tr->index ; i <= PATH_POINTS ; ++i ) {
			angles[i] += tr->delta ;
		}
	}
	
	tr->segment = 0. ;
	tr->index = NO_PATH ;
}

void perturb_calc( struct trial * tr ) {
	// use law of cosines to calculate segment length from altered angle
	int i = tr->index ; // segment start
	double r = radii[i] ; 
	double last_r = radii[i-1] ;
	tr->segment = sqrt( r*r + last_r*last_r -2*r*last_r*cos( angles[i] + (tr->delta) - angles[i-1] ) );
	tr->length += tr->segment - segments[i] ; // update length
	tr->penalty = PENALTY_UNCALCULATED ; // penalty no longer accurate
}			

void copy_trial( struct trial * dest, struct trial * source ) {
	memcpy( dest, source, sizeof( struct trial ) ) ;
}

enum perturb_result test_delta( struct trial * tr ) {
	// return 1 if a change
	// return 0 if no change ( need to halve the delta )
	struct trial test ;
	double old_length = tr->length ;
	copy_trial( &test, tr ) ;
	
	// set index case as best (to test against)
	double best_score = score_calc( tr ) ;
	//printf("Best score %f ",best_score ) ;
	//Ptrial( tr ) ;
	
	for ( int i= 1 ; i <= PATH_POINTS ; ++i ) {
		test.index = i ;
		test.length = old_length ;
		// positive pertubation
		test.delta = tr->delta ;
		perturb_calc( &test ) ;
		if ( test.length < best_score ) {
			//printf("test1 ");
			double score = score_calc( &test ) ;
			if ( score < best_score ) {
				copy_trial( tr, &test ) ;
				best_score = score ;
				//printf("New best %f | ",best_score);
				//Ptrial(tr) ;
			}
		}
		test.length = old_length ;
		// negative pertubation
		test.delta = -(tr->delta) ;
		perturb_calc( &test ) ;
		if ( test.length < best_score ) {
			//printf("test1 ");
			double score = score_calc( &test ) ;
			if ( score < best_score ) {
				copy_trial( tr, &test ) ;
				best_score = score ;
				//printf("New best %f | ",best_score);
				//Ptrial(tr) ;
			}
		}
	}
	
	// choose best
	if ( tr->index == NO_PATH ) {
		// no change
		tr->last_index = NO_PATH ; // turn off collection 2 for doubling
		return no_change ;
	} else if ( tr->last_index == tr->index ) {
		// yes change
		accept_trial( tr ) ; // set as standard
		return second_change ;
	} else {
		tr->last_index = tr->index ;
		accept_trial( tr ) ; // set as standard
		return first_change ;
	}
}

void iterate_delta( struct trial * tr ) {
	int halve = 0 ;
	while ( halve < GENS ) {
		switch ( test_delta( tr ) ) {
			case no_change:
				tr->delta *= .5 ;
				++ halve ;
				if ( verbose ) {
					printf( "Generation %d ",halve ) ;
					Ptrial( tr ) ;
				}
				break ;
			case first_change:
				break ;
			case second_change:
				tr->delta *= 2 ;
				-- halve ;
				if ( verbose ) {
					printf( "Generation %d ",halve ) ;
					Ptrial( tr ) ;
				}
				break ;
		}
		if ( verbose ) {
			printf( "Generation %d delta=%g",halve,tr->delta ) ;
			Ptrial( tr ) ;
		}
	}
}

void run( void ) {
	struct trial trial_struct ;
	allocate_arrays() ; // make space for arrays

	initial_path( &trial_struct ) ; // setup
	iterate_delta( &trial_struct ) ; // solve
	penalty_calc( &trial_struct ) ; // set shark
	Graph( &trial_struct ) ; // show
}	

// For printing
#define SHARK_RADIUS 1.5

void Graph(struct trial * tr) {
    char file_control[120] ;
    if ( add_text ) {
		sprintf( file_control, "controlB%d_%s.gplot", PATH_POINTS, add_text ) ;
	} else {
		sprintf( file_control, "controlB%d.gplot", PATH_POINTS ) ;
	}
    FILE * fcontrol = fopen( file_control, "w" ) ;
    if ( fcontrol == NULL ) {
        fprintf( stderr , "Cannot open file %s\n", file_control ) ;
        exit(1) ;
    }
    
    char file_data[120];
    if ( add_text ) {
		sprintf( file_data,    "dataB%d_%s.dat",      PATH_POINTS, add_text ) ;
	} else {
		sprintf( file_data,    "dataB%d.dat",      PATH_POINTS ) ;
	}
    FILE * fdata = fopen( file_data, "w" ) ;
    if ( fdata == NULL ) {
        fprintf( stderr , "Cannot open file %s\n", file_data ) ;
        fclose( fcontrol ) ;
        exit(1) ;
    }

    /* gnuplot commands */
    fprintf( fcontrol, "unset border\n" ) ;
    fprintf( fcontrol, "set polar\n" ) ;
    fprintf( fcontrol, "unset xtics\n" ) ;
    fprintf( fcontrol, "unset ytics\n" ) ;
    fprintf( fcontrol, "unset raxis\n" ) ;
    fprintf( fcontrol, "set ttics axis\n" ) ;
    fprintf( fcontrol, "set rtics axis\n" ) ;
    fprintf( fcontrol, "set grid polar\n" ) ;
    fprintf( fcontrol, "set size square\n" ) ;
    fprintf( fcontrol, "set rrange [0:%f]\n", SHARK_RADIUS ) ;
    fprintf( fcontrol, "set rtics (\"%.2f\" %.2f,\"%.2f\" %.2f 1,\"%.2f\" %.2f)\n",1./SHARK_V,1./SHARK_V,.5,.5,1.,1. ) ;
    fprintf( fcontrol, "set key autotitle columnheader\n" );
    fprintf( fcontrol, "plot for [i=0:1] \'%s\' using 1:2 index i with lines\n", file_data ) ;
//    fprintf( fcontrol, "pause mouse\n" ) ;
    
    fclose( fcontrol ) ;

    /* Show results */
    printf( "Calculation with %d points:\n", PATH_POINTS );
    printf( "\t%f\tTotal angle turned by man (degrees)\n",(angles[PATH_POINTS]-angles[0])*180/M_PI );
    printf( "\t%f\tTotal angle turned by shark (degrees)\n",(shark[PATH_POINTS]-shark[0])*180/M_PI );
    printf( "\t%f\tTotal length of man's run\n",tr->length );
    printf( "\t%f\tpenalty for shark bite\n", tr->penalty );

    /* Data file output */
    // man
    fprintf( fdata, "Man\n" ) ;
    for( int i = 0 ; i <= PATH_POINTS ; ++i ) {
        fprintf( fdata, "%g %g\n", angles[i], radii[i] ) ;
    }
    
    fprintf( fdata, "\n\n" ) ; // separator

    // shark
    fprintf( fdata, "Shark\n" ) ;
    for( int i = 0 ; i <= PATH_POINTS ; ++i ) {
        fprintf( fdata, "%g %g\n", shark[i], SHARK_RADIUS - i * (SHARK_RADIUS-1.0) / PATH_POINTS ) ;
    }
    
    fclose( fdata ) ;

    char system_command[250] ;
    sprintf( system_command, "gnuplot -p %s", file_control ); 
    
    system(system_command);
}

void help() {
    printf("shark-chase\n");
    printf("\tfind fastest way to beach avoiding shark\n");
    printf("\tby Paul H Alfille 2023 -- MIT Licence\n");
    printf("\tSee https://github.com/alfille/shark-chase\n");
    printf("\n");
    printf("shark [options]\n");
    printf("\n");
    printf("Options\n");
    printf("\t-p%d\t--path\t\tnumber of steps (default %d)\n",PATH_POINTS,PATH_POINTS);    
    printf("\t-s%g\t--speed\t\tShark speed (default %g)\n",SHARK_V,SHARK_V);
    printf("\t-atext\t--add\t\tAdd text to end of control and data file names\n");    
    printf("\t-v\t--verbose\tshow progress during search\n");
    printf("\t-h\t--help\t\tthis help\n");
    printf("\n");
    printf("Obscure options\n");
    printf("\t-x%g\t--penalty\tPenalty multiplier (default %g)\n",PENALTY_MULT,PENALTY_MULT);
    printf("\t-g%d\t--generations\tNumber of halving error delta (default (%d)\n",GENS,GENS);
    exit(1);
}

struct option long_options[] =
{
    {"path"   ,   required_argument, 0, 'p'},
    {"speed"  ,   required_argument, 0, 's'},
	{"add"    ,   required_argument, 0, 'a'},
    {"verbose",   no_argument,       0, 'v'},
    {"help"   ,   no_argument,       0, 'h'},
	{"generations",   required_argument, 0, 'g'},
    {0        ,   0          ,       0,   0}
};

void ParseCommandLine( int argc, char * argv[] ) {
    // Parse command line
    int c;
    int option_index ;
    while ( (c = getopt_long( argc, argv, "p:s:a:vhx:g:", long_options, &option_index )) != -1 ) {
        //printf("opt=%c, index=%d, val=%s\n",c,option_index, long_options[option_index].name);
        switch (c) {
            case 0:
                break ;
            case 'p':
                PATH_POINTS = (int) atoi(optarg);
                break ;
            case 's':
                SHARK_V = (double) atof(optarg);
                break ;
            case 'a':
				add_text = (char *) optarg ;
				break ;
            case 'v':
                verbose = 1 ;
                break ;
            case 'h':
                help();
                break ;
             case 'x':
                PENALTY_MULT = (double) atof(optarg) ;
                break ;
             case 'g':
                GENS = (int) atoi(optarg) ;
                break ;
            default:
                help() ;
                break ;
            }
    }
}   

int main( int argc, char ** argv ) {
    ParseCommandLine( argc, argv ) ;
        
    printf( "Data points: %i\n",PATH_POINTS ) ;
    if ( verbose==0 ) {
        printf("\tcalculating -- may take a while\n");
    }
    run() ;
    
    return 0 ;
}
