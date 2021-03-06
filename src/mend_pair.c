#include <R.h>
#include <stdio.h>
#include <math.h>
#include "solidmon.h"

#define G(L,s,H,nS) (2*nS)*L + s*2 + H 

/*this function goes through all possible pairings of Sires and Kids and returns
relevant information about each pair such as number of mendelian compatible loci,
the log-likelihood ratio of parental versus unrelated computed for compatible loci, etc.

The fields of the output file are as follows: 
     - index of sire (starting with 1)
     - index of kid (starting w/ 1)
     - pairing indicators for each locus:
          -1  : missing data, 
          0   : not sharing any allele
          1   : sharing at least 1 allele
     - number of Mendelian compatible loci in the pair
     - log probability of pair's genotype given they are parent-offspring of mendalian prob
     - log probability of pair's genotyped given they are unrelated
     - log likelihood-ratio (parental versus unrelated)   
*/

int Pair_filter(int *max_incompat, int *S, int *K, double *P,  double *excP, int *L, int *nS, int *nK, int *ps)
{
	int num_S = *nS;
	int num_K = *nK;
	int num_L = *L;
	int max_incom = *max_incompat;
	int s, k, j;
	int a, b, c, d;
	
	FILE *out;
	out=fopen("out/Mend_pair_sel.tbl", "w");
	
	fprintf(out, "SIR_INDX\tKIDS_INDX\t");
	for (j=0; j<num_L; j++)
	    fprintf (out, "LOCUS_%i\t",j+1);
	fprintf(out, "NUM_MATCH\tLOG_PROB_MEND\tLOG_SHARED\tLOG_LIKELIHOOD\n");
	
	//iterate for each dad 
	for (s = 0; s < num_S; s++) { //iterate for each dad 
		for (k = 0; k < num_K; k++) { //iterate thru each kids
			double prob_pair = 0;
			double unrel_prob = 0;
			int match = 0;
			
			fprintf(out, "%i\t%i\t" ,s+1, k+1);
			for (j = 0, match=0; j < num_L ; j++) { //iterate thru each locus
				a = S[G(j, s, 0, num_S)]; 
				b = S[G(j, s, 1, num_S)];
				c = K[G(j, k, 0, num_K)]; 
				d = K[G(j, k, 1, num_K)];
				double *p =  P + ps[j];
				
				fprintf(out, "%i\t", (MISSING)*-1 + (!MISSING)*(SHARES_1_OR_2));
				
				if (MISSING) 
				    continue;
				
				if(SHARES_1_OR_2) {
					match++;
					prob_pair += log(PO_PROB);
					unrel_prob += log(UN_PROB);
				}				
				
			}
			fprintf(out, "%i\t%lf\t%lf\t%lf\n" ,match, prob_pair, unrel_prob, prob_pair-unrel_prob);
		}
	}
	fclose(out);
	return 1;
}
