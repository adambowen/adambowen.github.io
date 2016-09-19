// Copyright (c) Pattern Analytics Research Ltd.
// Used with permission

#include <sstream>
#include <iostream>
#include "par_fusion.h"

int main( int argc, char *argv[] )
{
    try
    {
        par::fusion::fuser fuser;
        par::fusion::fusion_information algorithm(PAR_WARWICK_WARP_LIVESCAN_3_0_0);

        for( int i=1; i<argc; ++i )
        {
            std::istringstream ss(argv[i]);
            ss.unsetf(std::ios::skipws);

            double score = 0;
            if( ss >> score )
            {
                fuser.add_score( algorithm, score );
            }
            else
            {
                std::cerr << "Invalid score: " << argv[i] << ".\n";
                return 1;
            }
        }

        std::cout << fuser.fused_score() << "\n";

        return 0;
    }
    catch( const par::exception& e )
    {
        std::cerr << "Error: " << e.what() << ".\n";
        return 1;
    }
}
