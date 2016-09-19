// Copyright (c) Pattern Analytics Research Ltd.
// Used with permission

#include <stdio.h>
#include "par_fusion.h"

int main( int argc, char *argv[] )
{
    par_fusion_handle fuser = NULL;
    par_fusion_information_handle algorithm = NULL;
    int rv = 0;
    int i  = 0;
    float score = 0;
    double fused_score = 0;

    rv = par_fusion_init_fuser( &fuser );
    if( rv != PAR_OK )
    {
        fprintf(stderr,"Error: %s.\n", par_last_error_message());
        return 1;
    }

    rv = par_fusion_init_information( PAR_WARWICK_WARP_LIVESCAN_3_0_0, &algorithm );
    if( rv != PAR_OK )
    {
        fprintf(stderr,"Error: %s.\n", par_last_error_message());
        par_fusion_release_fuser(fuser);
        return 1;
    }

    for( i=1; i<argc; ++i )
    {
        if( sscanf(argv[i],"%f",&score) == 1 )
        {
            rv = par_fusion_add_score(fuser,algorithm,score);
            if( rv != PAR_OK )
            {
                fprintf(stderr,"Error: %s.\n", par_last_error_message());
                par_fusion_release_information(algorithm);
                par_fusion_release_fuser(fuser);
                return 1;
            }
        }
        else
        {
            fprintf(stderr,"Invalid score: %s.\n",argv[i]);
            par_fusion_release_information(algorithm);
            par_fusion_release_fuser(fuser);
            return 1;
        }
    }

    rv = par_fusion_fuse_scores(fuser,&fused_score);
    if( rv != PAR_OK )
    {
        fprintf(stderr,"Error: %s.\n", par_last_error_message());
        par_fusion_release_information(algorithm);
        par_fusion_release_fuser(fuser);
        return 1;
    }

    printf("%f\n",fused_score);

    par_fusion_release_information(algorithm);
    par_fusion_release_fuser(fuser);

    return 0;
}
