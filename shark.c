/* shark-chase
 * find optimal path to escape a shark
 * see https://github.com/alfille/shark-chase
 * 
 * Paul H Alfille 2025
 * MIT license
 * */

/* Shark from DSK */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

/* Gnu Scientific Library for Simulated Annealing and Random */
#include <gsl/gsl_siman.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_sf_trig.h>

/* Also uses Gnuplot for visulaization */

/* Compile:
 * gcc -o shark shark.c -lgsl -lm
 * */

// Number of points (plus 1 for endpoints) */
int PATH_POINTS = 100 ;

int verbose = 0 ;

/* Simulated Annealing Parameters */
#define N_TRIES 10000 // points before stepping
#define ITERS_FIXED_T 10000 // points at temperature
#define K 1.0 // Boltzman const
#define T_INITIAL 0.008 // man_pos temp
#define MU_T 1.003 // damping factor
#define T_MIN 2.0e-6

/* Shark params */
#define SHARK_V 4.0 // relative speed
#define SHARK_INIT_ANGLE M_PI // opposite side
double  * shark_pos ;

// man's position
struct point {
    double angle ;
    double r ;
} * man_pos ;

// man_pos array -- straight run to the beach
void initial_setup( void ) {
    man_pos = (struct point *) calloc( PATH_POINTS + 1, sizeof( struct point ) ) ; 
    shark_pos = (double *) calloc( PATH_POINTS + 1, sizeof(double) ) ;
    for ( int i=0 ; i<=PATH_POINTS ; ++i ) {
        man_pos[i].r = (double) i / PATH_POINTS ;
        man_pos[i].angle = 0 ;
    }
}

// Calculate the run
// return length, update shark_pos and update penalty
double Calculation( void * xp, double * penalty ) {
    struct point * path = xp ;

    double length = 0. ;
    *penalty = 0.;

    double last_r = 0 ;
    double last_r2 = 0. ;
    double last_a = path[0].angle ;
    double shark = SHARK_INIT_ANGLE ;
    shark_pos[0] = shark ;
    
    for ( int i = 1 ; i <= PATH_POINTS ; ++i ) { // intervals, skip center
        double r = path[i].r ;
        double r2 = r*r;
        double a = path[i].angle ;

        double l = sqrt( r2 + last_r2 - 2 * r * last_r * cos( a-last_a ) ) ;
        length += l ;

        last_r = r ;
        last_r2 = r2 ;
        last_a = a ;

        double rem = gsl_sf_angle_restrict_symm( a - shark ) ;
        double max_shark = l * SHARK_V ;
        shark += (rem>=0) ? fmin( rem, max_shark ) : fmax( rem, -max_shark ) ;
        shark_pos[i] = shark ;

        if ( fabs( gsl_sf_angle_restrict_symm( shark - last_a ) ) < .00001 ) {
            if ( r * SHARK_V > 1 ) {
                // penalty for shark time waiting
                *penalty += l ;
            }
        }
    }

    if ( fabs( gsl_sf_angle_restrict_symm( shark - last_a ) ) > .0001 ) {
        *penalty += 1 ;
    }
    
    return length ;
}

// length (which we want to minimize) + square of beach distance
double Energy( void * xp ) {
    double penalty ;
    double length = Calculation( xp, &penalty ) ;
    return length + PATH_POINTS*penalty ;
}

double Metric( void * xp , void * yp ) {
    struct point * pathx = xp ;
    struct point * pathy = yp ;
    double metric = 0 ;
    for ( int i = 0 ; i < PATH_POINTS ; ++i ) { // ignore endpoints
        metric += fabs( pathx[i].angle - pathy[i].angle );
    }
    return metric ;
}

void Step( const gsl_rng * rng, void * xp, double step_size ) {
    struct point * path = xp ;
    int target = gsl_rng_uniform_int(rng, PATH_POINTS) + 1 ; // ignore center
    double delta = ( gsl_rng_uniform(rng) - .5 ) * step_size ;
    for ( int i = target; i <= PATH_POINTS ; ++i ) {
        path[i].angle += delta ;
    }
}

void Print( void * xp ) {
    struct point * path = xp ;
    for ( int i=0 ; i<=PATH_POINTS ; ++i ) {
        //printf( "%d: r=%g, a=%g\n", i, path[i].r, path[i].angle ) ;
    }
}

int pass( gsl_rng * rng ) {
    gsl_siman_params_t params = {
        N_TRIES, 
        ITERS_FIXED_T, 
        M_PI / PATH_POINTS , // step size
        K, 
        T_INITIAL, 
        MU_T, 
        T_MIN
        };
    
    gsl_siman_solve(
        rng , // gsl_rng *
        man_pos, // array
        Energy, // energy function
        Step, // step function
        Metric, // metric function
        verbose ? Print : NULL, //Print, // print position
        NULL, // copy
        NULL, // constructor
        NULL, // destructor
        (PATH_POINTS+1) * sizeof( struct point ), // array size 
        params
        );
}

// For printing
#define SHARK_RADIUS 1.5

void Graph(void) {
    char file_control[120] ;
    sprintf( file_control, "control%d.gplot", PATH_POINTS ) ;
    FILE * fcontrol = fopen( file_control, "w" ) ;
    if ( fcontrol == NULL ) {
        fprintf( stderr , "Cannot open file %s\n", file_control ) ;
        exit(1) ;
    }
    
    char file_data[120];
    sprintf( file_data,    "data%d.dat",      PATH_POINTS ) ;
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
    fprintf( fcontrol, "pause mouse\n" ) ;
    
    fclose( fcontrol ) ;

    /* Calculate of results */
    double penalty ;
    double length = Calculation( man_pos, &penalty ) ;
    
    printf( "Calculation with %d points:\n", PATH_POINTS );
    printf( "\t%f\tTotal angle turned by man (degrees)\n",(man_pos[PATH_POINTS].angle-0)*180/M_PI );
    printf( "\t%f\tTotal angle turned by shark (degrees)\n",(shark_pos[PATH_POINTS]-SHARK_INIT_ANGLE)*180/M_PI );
    printf( "\t%f\tTotal length of man's run\n",length );
    printf( "\t%f\tpenalty for shark bite\n", penalty );

    /* Data file output */
    // man
    fprintf( fdata, "Man\n" ) ;
    for( int i = 0 ; i <= PATH_POINTS ; ++i ) {
        fprintf( fdata, "%g %g\n", man_pos[i].angle, man_pos[i].r ) ;
    }
    
    fprintf( fdata, "\n\n" ) ; // separator

    // shark
    fprintf( fdata, "Shark\n" ) ;
    for( int i = 0 ; i <= PATH_POINTS ; ++i ) {
        fprintf( fdata, "%g %g\n", shark_pos[i], SHARK_RADIUS - i * (SHARK_RADIUS-1.0) / PATH_POINTS ) ;
    }
    
    fclose( fdata ) ;

    char system_command[250] ;
    sprintf( system_command, "gnuplot %s", file_control ); 
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
    printf("\t-v\t--verbose\tshow progress during search\n");
    printf("\t-h\t--help\t\tthis help\n");
    exit(1);
}

struct option long_options[] =
{
    {"path"   ,   required_argument, 0, 'p'},
    {"verbose",   no_argument,       0, 'v'},
    {"help"   ,   no_argument,       0, 'h'},
    {0        ,   0          ,       0,   0}
};

void ParseCommandLine( int argc, char * argv[] ) {
    // Parse command line
    int c;
    int option_index ;
    while ( (c = getopt_long( argc, argv, "p:vh", long_options, &option_index )) != -1 ) {
        //printf("opt=%c, index=%d, val=%s\n",c,option_index, long_options[option_index].name);
        switch (c) {
            case 0:
                break ;
            case 'h':
                help();
                break ;
            case 'p':
                PATH_POINTS = (int) atoi(optarg);
                break ;
            case 'v':
                verbose = 1 ;
                break ;
            default:
                help() ;
                break ;
            }
    }
        
    // test parameters
    if ( PATH_POINTS < 10 ) {
        PATH_POINTS = 10 ;
    }
    if ( PATH_POINTS > 1000000 ) {
        PATH_POINTS = 1000000 ;
    }
}   

int main( int argc, char ** argv ) {
    ParseCommandLine( argc, argv ) ;
    
    // random generator
    const gsl_rng_type * T ;
    gsl_rng * rng ;
    gsl_rng_env_setup() ;
    T = gsl_rng_default ;
    rng = gsl_rng_alloc( T ) ;
    
    initial_setup() ;
    printf( "Data points: %i\n",PATH_POINTS ) ;
    if ( verbose==0 ) {
        printf("\tcalculating -- may take a while\n");
    }
    pass(rng) ;
    Graph() ;
    
    return 0 ;
}
