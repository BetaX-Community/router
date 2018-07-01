/***************************************************************************
 *  Project:    betax-router
 *  File:       overpass-api.cpp
 *  Language:   C++
 *
 *  Author:     Andrianambinina Marius Rabenarivo 
 *  E-Mail:     marius@betax.mg 
 *
 *  Description:    Extracts an OSM XML file from the overpass API.
 *                  All functionality is contained in the header file;
 *                  this .c++ file just provides a stand-alone compilable wrapped.
 *
 *  Dependencies:       libboost
 *
 *  Compiler Options:   -std=c++11 -lboost_program_options 
 *
 *  Usage : ./overpass-api -b -19.0790 -a 47.2402 -d -18.7464 -c 47.7569 -f antananarivo.xml
 *
 ***************************************************************************/

#include "overpass-api.h"

/************************************************************************
 ************************************************************************
 **                                                                    **
 **                                MAIN                                **
 **                                                                    **
 ************************************************************************
 ************************************************************************/

int main(int argc, char *argv[]) 
{
    float lonmin, latmin, lonmax, latmax;
    std::string file;

    try {
        boost::program_options::options_description generic("Generic options");
        generic.add_options()
            ("version,v", "print version std::string")
            ("help", "produce help message")    
            ;

        boost::program_options::options_description config("Configuration");
        config.add_options()
            ("file,f", boost::program_options::value <std::string> 
                (&file)->default_value ("xmldat.xml"), 
                "file name (.xml will be appended)")
            ("lonmin,a", boost::program_options::value <float> 
                (&lonmin)->default_value (-0.12), 
                "lonmin (ignored if file exists)")
            ("latmin,b", boost::program_options::value <float> 
                (&latmin)->default_value (51.515), "latmin (ditto)")
            ("lonmax,c", boost::program_options::value <float> 
                (&lonmax)->default_value (-0.115), "lonmax (ditto)")
            ("latmax,d", boost::program_options::value <float> 
                (&latmax)->default_value (51.52), "latmax (ditto)")
            ;

        boost::program_options::options_description cmdline_options;
        cmdline_options.add(generic).add(config);

        boost::program_options::options_description visible("Allowed options");
        visible.add(generic).add(config);

        boost::program_options::variables_map vm;
        store(boost::program_options::command_line_parser(argc, argv).
                options(cmdline_options).run(), vm);

        notify(vm);

        if (vm.count("help")) {
            std::cout << visible << std::endl;
            return 0;
        }

        if (vm.count("version")) {
            std::cout << "overpass-api, version 1.0" << std::endl;
            return 0;
        }

    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }    

    if ((int) file.find (".") < 0)
        file = file + ".xml";

    Xml xml (file, lonmin, latmin, lonmax, latmax);
    return 0;
};
