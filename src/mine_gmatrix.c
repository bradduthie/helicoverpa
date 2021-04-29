#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>
#include <Rmath.h>
#include <stdlib.h>


/* =============================================================================
 * This seeds a 4D array with standard random normal values; The array is used
 * to model an evolving population of 3D networks that move from loci values to
 * trait values. Each 3D subset of the 4D array is a potential solution to the
 * problem of how to map loci to traits in a way that is correlated according to
 * the correlation matrix in the mine_gmatrix function.
 *     netpop: The full 4D network of evolving 3D arrays
 *     npsize: The size of the population evolving in the evolutionary algorithm
 *     layers: The number of layers in an individual loci to trait network
 *     traits: The number of traits that an individual has
 * ========================================================================== */
void ea_net_ini(double ****netpop, int npsize, int layers, int traits){
  
  int k, l, i, j;
  
  for(k = 0; k < npsize; k++){
    for(l = 0; l < layers; l++){
      for(i = 0; i < traits; i++){
        for(j = 0; j < traits; j++){
          netpop[k][l][i][j] = rnorm(0, 1);
        }
      }
    }
  }
}


/* =============================================================================
 * This seeds a 3D array with standard random normal values; The array is used
 * to map loci to traits, and therefore get into the bigger network with many
 * layers. It also needs to go through the evolutionary algorithm, so we have
 * a population of 'npsize' loci to network matrices that can potentially evolve
 *     ltnpop: The 3D array that holds the population of loci to trait matrices
 *     npsize: The size of the population evolving in the evolutionary algorithm
 *     loci:   The number of loci in the model (rows in individual matrices)
 *     traits: The number of traits in the model (cols in individual matrices)
 * ========================================================================== */
void ea_ltn_ini(double ***ltnpop, int npsize, int loci, int traits){
  
  int k, i, j;
  
  for(k = 0; k < npsize; k++){
    for(i = 0; i < loci; i++){
      for(j = 0; j < traits; j++){
        ltnpop[k][i][j] = rnorm(0, 1);
      }
    }
  }
}


/* =============================================================================
 * This seeds a 2D array with standard random normal values; array rows include
 * individuals being modelled and array columns are individual loci
 *     inds:   A 2D array of individuals
 *     indivs: The number of individuals (rows of the array)
 *     loci:   The number of loci for an individual (columns of the array)
 * ========================================================================== */
void ea_pop_ini(double **inds, int indivs, int loci){
    
    int row, col;
    
    for(row = 0; row < indivs; row++){
        for(col = 0; col < loci; col++){
            inds[row][col] = rnorm(0, 1);
        }
    }
}

/* =============================================================================
 * This is a generic function to multiply two matrices together
 *     m1:      The first matrix to be multiplied
 *     m2:      The second matrix to be multiplied
 *     m1_rows: Number of rows in matrix m1
 *     m1_cols: Number of columns in matrix m1
 *     m2_rows: Number of rows in matrix m2
 *     m2_cols: Number of columns in matrix m2
 *     m_out:   The output product matrix
 * ========================================================================== */
void matrix_multiply(double **m1, double **m2, int m1_rows, int m1_cols,
                     int m2_rows, int m2_cols, double **m_out){
    
    /* Add break if non-conformable arrays? */
    int row, col, ele;
    double val; 

    for(row = 0; row < m1_rows; row++){
        for(col = 0; col < m2_cols; col++){
            val = 0;
            for(ele = 0; ele < m1_cols; ele++){
                val += (m1[row][ele] * m2[ele][col]);
            }
            m_out[row][col] = val;
        }
    }
}

/* =============================================================================
 * This function multiplies the square matrices that make up the layers of an
 * array (net) to produce a two dimensional matrix (net_out) product.
 *     traits:  Traits of an individual; also matrix rows and columns
 *     layers:  Layers of the network (i.e., how many matrices in the array)
 *     net:     The 3D array in which layers are square matrices
 *     net_out: The output matrix that is the product of the array layers
 * ========================================================================== */
void sum_network_layers(int traits, int layers, double ***net, 
                        double **net_out){
    
    int i, j, k;
    double ***net_temp;
    
    
    net_temp = malloc(layers * sizeof(double *));
    for(k = 0; k < layers; k++){
        net_temp[k] = malloc(traits * sizeof(double *));
        for(i = 0; i < traits; i++){
            net_temp[k][i] = malloc(traits * sizeof(double));   
        }
    }
    for(k = 0; k < layers; k++){
        for(i = 0; i < traits; i++){
            for(j = 0; j < traits; j++){
                net_temp[k][i][j] = net[k][i][j];
            }
        }
    }
    
    for(k = 1; k < layers; k++){
        matrix_multiply(net_temp[k-1], net_temp[k], traits, traits, traits, 
                        traits, net_out);
        if(k < layers == 1){
            for(i = 0; i < traits; i++){
                for(j = 0; j < traits; j++){
                    net_temp[k][i][j] = net_out[i][j]; 
                }
            }
        }
    }
    
    for(k = 0; k < layers; k++){
        for(i = 0; i < traits; i++){
            free(net_temp[k][i]);
        }
        free(net_temp[k]);        
    }
    free(net_temp); 
}

/* =============================================================================
 * This function seeds a matrix with standard random normal values the matrix
 * has a number of rows equal to individual's loci, and a number of columns
 * equal to an individual's traits.
 *    traits:         Traits an individual has, and columns of the matrix
 *    loci:           Loci an individual has, and the rows of the matrix
 *    loci_layer_one: The name of the matrix
 * ========================================================================== */
void initialise_loci_net(int traits, int loci, double **loci_layer_one){

    int row, col;
   
    for(row = 0; row < loci; row++){
        for(col = 0; col < traits; col++){
            loci_layer_one[row][col] = rnorm(0, 1); 
        }
    }
}

/* =============================================================================
 * This function seeds a 3D network array with standard random normal values.
 * Note that the array is made up of layers of square matrices, so the first two
 * dimensions of the array are always the same (traits).
 *     traits: The number of traits, and hence dimensions of each square matrix
 *     layers: The number of matrices layered on top of one another
 *     net:    The actual 3D network of dimension trait*trait*layers
 * ========================================================================== */
void initialise_net(int traits, int layers, double ***net){
    
    int k, i, j;

    for(k = 0; k < layers; k++){
        for(i = 0; i < traits; i++){
            for(j = 0; j < traits; j++){
                net[k][i][j] = rnorm(0, 1); 
            }
        }
    }   
}


/* =============================================================================
 * This function sets all elements in a matrix to a value of zero:
 *     rows: Rows within the matrix
 *     cols: Columns within the matrix
 *     mat:  The matrix itself
 * ========================================================================== */
void matrix_zeros(int rows, int cols, double **mat){
    
    int row, col;
    
    for(row = 0; row < rows; row++){
        for(col = 0; col < cols; col++){
            mat[row][col] = 0;
        }
    }
}


/* =============================================================================
 * MAIN RESOURCE FUNCTION:
 * ===========================================================================*/

/* =============================================================================
 * This is the outer function for mining the g-matrices
 *  Inputs include:
 *      PARAS:   Nothing yet, but will hold the paramters of interest
 *      GMATRIX: Holds the g-matrix that guides evolutionary algorithm fitness
 * ===========================================================================*/
SEXP mine_gmatrix(SEXP PARAS, SEXP GMATRIX){
 
    /* SOME STANDARD DECLARATIONS OF KEY VARIABLES AND POINTERS               */
    /* ====================================================================== */
    int    i, j, k;
    int    row;
    int    col;
    int    vec_pos;
    int    protected_n;    /* Number of protected R objects */
    int    len_PARAS;      /* Length of the parameters vector */
    int    *dim_GMATRIX;   /* Dimensions of the G-matrix */
    double val;            /* Value of matrix elements */
    double *paras;         /* parameter values read into R */
    double *paras_ptr;     /* pointer to the parameters read into C */
    double *paras_ptr_new; /* Pointer to new paras (interface R and C) */
    double *network_ptr;   /* Pointer to network output (interface R and C) */
    double *loci_net_ptr;  /* Pointer to the loci to net (interface R and C) */
    double *loci_eff_ptr;  /* Pointer to the loci effects (interface R and C) */
    double *G_ptr;         /* Pointer to GMATRIX (interface R and C) */
    
    int loci;
    int traits;
    int layers;
    int indivs;   /* Seeded individuals in evolutionary algorithm */
    int npsize;   /* Number of arrays in the evolutionary algorithm */
    
    double **gmatrix;
    double **loci_layer_one;
    double **net_sum;
    double ***net;
    double **loci_to_traits;
    double **inds;
    double ***ltnpop;
    double ****netpop;

    /* First take care of all the reading in of code from R to C */
    /* ====================================================================== */

    protected_n = 0;
    
    PROTECT( PARAS = AS_NUMERIC(PARAS) );
    protected_n++;
    paras_ptr = REAL(PARAS);

    PROTECT( GMATRIX = AS_NUMERIC(GMATRIX) );
    protected_n++;
    G_ptr = REAL(GMATRIX);
    
    len_PARAS   = GET_LENGTH(PARAS);
    dim_GMATRIX = INTEGER( GET_DIM(GMATRIX)  );

    /* The C code for the model itself falls under here */
    /* ====================================================================== */
    
    paras   = malloc(len_PARAS * sizeof(double *));
    vec_pos = 0;
    for(i = 0; i < len_PARAS; i++){
        paras[i] = paras_ptr[vec_pos];
        vec_pos++;
    } /* The parameters vector is now copied into C */

    /* Code below remakes the GMATRIX matrix for easier use */
    traits   = dim_GMATRIX[0];
    gmatrix  = malloc(traits * sizeof(double *));
    for(row = 0; row < traits; row++){
        gmatrix[row] = malloc(traits * sizeof(double));   
    } 
    vec_pos = 0;
    for(col = 0; col < traits; col++){
        for(row = 0; row < traits; row++){
            gmatrix[row][col] = G_ptr[vec_pos]; 
            vec_pos++;
        }
    }
    
    /* Do the biology here now */
    /* ====================================================================== */
    
    /** Parameter values as defined in R **/
    loci   = paras[0]; /* Number of loci for an individual */
    layers = paras[1]; /* Layers in the network from loci to trait */
    indivs = paras[2]; /* Individuals in the population */
    npsize = paras[3]; /* Size of the strategy population */
    
    /* Allocate memory for the appropriate loci array, 3D network, sum net,
     * and loci_to_trait values
     */ 
    loci_layer_one  = malloc(loci * sizeof(double *));
    for(row = 0; row < loci; row++){
        loci_layer_one[row] = malloc(traits * sizeof(double));   
    }

    net   = malloc(layers * sizeof(double *));
    for(k = 0; k < layers; k++){
        net[k] = malloc(traits * sizeof(double *));
        for(i = 0; i < traits; i++){
            net[k][i] = malloc(traits * sizeof(double));   
        }
    } 

    net_sum = malloc(traits * sizeof(double *));
    for(row = 0; row < traits; row++){
        net_sum[row] = malloc(traits * sizeof(double));   
    } 
    
    loci_to_traits  = malloc(loci * sizeof(double *));
    for(row = 0; row < loci; row++){
        loci_to_traits[row] = malloc(traits * sizeof(double));   
    } 
    
    inds = malloc(indivs * sizeof(double *));
    for(row = 0; row < indivs; row++){
        inds[row] = malloc(loci * sizeof(double));
    }

    ltnpop = malloc(npsize * sizeof(double *));
    for(k = 0; k < npsize; k++){
        ltnpop[k] = malloc(loci * sizeof(double *));
        for(i = 0; i < loci; i++){
            ltnpop[k][i] = malloc(traits * sizeof(double));   
        }
    } 

    netpop = malloc(npsize * sizeof(double *));
    for(k = 0; k < npsize; k++){
        netpop[k] = malloc(layers * sizeof(double *));
        for(j = 0; j < layers; j++){
            netpop[k][j] = malloc(traits * sizeof(double *));
            for(i = 0; i < traits; i++){
                netpop[k][j][i] = malloc(traits * sizeof(double *));
            }
        }
    } 
 
    
    /* Initialise values of matrices to zero */
    matrix_zeros(traits, traits, net_sum);
    matrix_zeros(loci, traits, loci_to_traits);

    /* Now populate the networks with random values to initialise */    
    initialise_loci_net(traits, loci, loci_layer_one);

    initialise_net(traits, layers, net);
    
    /* Gets the summed effects of network by multiplying matrices */
    sum_network_layers(traits, layers, net, net_sum);
    
    /* Matrix that gets the final phenotype from the genotype */
    matrix_multiply(loci_layer_one, net_sum, loci, traits, traits, traits,
                    loci_to_traits);
    
    /* We now need an evolutionary algorithm that takes all the values from 
     * loci_layer_one and net and goes through the algorithm until we go from
     * random normal loci values to traits that match the covariance matrix
     * input into the R function
     */
    
    ea_pop_ini(inds, indivs, loci);
    ea_ltn_ini(ltnpop, npsize, loci, traits);
    ea_net_ini(netpop, npsize, layers, traits);
    
    /* This code switches from C back to R */
    /* ====================================================================== */        
    
    SEXP PARAMETERS_NEW;
    PROTECT( PARAMETERS_NEW = allocVector(REALSXP, len_PARAS) );
    protected_n++;
    
    paras_ptr_new = REAL(PARAMETERS_NEW);
    
    vec_pos = 0;
    for(i = 0; i < len_PARAS; i++){
        paras_ptr_new[vec_pos] = paras[i];
        vec_pos++;
    }
    
    
    SEXP LOCI_TO_NET;
    PROTECT( LOCI_TO_NET = allocMatrix(REALSXP, loci, traits) );
    protected_n++;
    
    loci_net_ptr = REAL(LOCI_TO_NET);
    
    vec_pos = 0;
    for(j = 0; j < traits; j++){
        for(i = 0; i < loci; i++){
            loci_net_ptr[vec_pos] = loci_layer_one[i][j];
                vec_pos++;
        }
    }
    
    
    SEXP NETWORK;
    PROTECT( NETWORK = alloc3DArray(REALSXP, traits, traits, layers) );
    protected_n++;
    
    network_ptr = REAL(NETWORK);
    
    vec_pos = 0;
    for(k = 0; k < layers; k++){
        for(i = 0; i < traits; i++){
            for(j = 0; j < traits; j++){
                network_ptr[vec_pos] = net[k][j][i];
                vec_pos++;
            }
        }
    }
    
    SEXP LOCI_EFFECTS;
    PROTECT( LOCI_EFFECTS = allocMatrix(REALSXP, loci, traits) );
    protected_n++;
    
    loci_eff_ptr = REAL(LOCI_EFFECTS);
    
    vec_pos = 0;
    for(j = 0; j < traits; j++){
        for(i = 0; i < loci; i++){
            loci_eff_ptr[vec_pos] = loci_to_traits[i][j];
            vec_pos++;
        }
    }
    
    
    SEXP GOUT;
    GOUT = PROTECT( allocVector(VECSXP, 5) );
    protected_n++;
    SET_VECTOR_ELT(GOUT, 0, PARAMETERS_NEW);
    SET_VECTOR_ELT(GOUT, 1, GMATRIX);
    SET_VECTOR_ELT(GOUT, 2, LOCI_TO_NET);
    SET_VECTOR_ELT(GOUT, 3, NETWORK);
    SET_VECTOR_ELT(GOUT, 4, LOCI_EFFECTS);
    
    UNPROTECT(protected_n);
    
    /* Free all of the allocated memory used in arrays */

    for(k = 0; k < npsize; k++){
        for(i = 0; i < layers; i++){
            for(j = 0; j < traits; j++){
                free(netpop[k][i][j]);
            }
            free(netpop[k][i]);
        }
        free(netpop[k]);
    }
    free(netpop);
 
    for(k = 0; k < npsize; k++){
        for(i = 0; i < loci; i++){
            free(ltnpop[k][i]);
        }
        free(ltnpop[k]);        
    }
    free(ltnpop); 

    for(row = 0; row < indivs; row++){
        free(inds[row]);
    }
    free(inds);
    
    for(row = 0; row < traits; row++){
        free(gmatrix[row]);
    }
    free(gmatrix);
    
    for(row = 0; row < loci; row++){
        free(loci_to_traits[row]);
    }
    free(loci_to_traits);
    
    for(k = 0; k < layers; k++){
        for(i = 0; i < traits; i++){
            free(net[k][i]);
        }
        free(net[k]);        
    }
    free(net); 
    
    for(row = 0; row < loci; row++){
        free(loci_layer_one[row]);
    }
    free(loci_layer_one);
    
    for(row = 0; row < traits; row++){
        free(net_sum[row]);
    }
    free(net_sum);

    
    free(paras);


    return(GOUT); 
}
/* ===========================================================================*/

