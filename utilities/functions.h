/*##############################################################################################
#                                                                                              #
#     #############       #############       #############       ####                ####     #
#    #             #     #             #     #             #     #    #              #    #    #
#    #    #####    #     #    #########      #    #####    #     #    #              #    #    #
#    #    #   #    #     #    #              #    #   #    #     #    #              #    #    #
#    #    #####    #     #    #              #    #####    #     #    #              #    #    #
#    #             #     #    #########      #             #     #    #              #    #    #
#    #             #     #             #     #             #     #    #              #    #    #
#    #    #####    #      #########    #     #    #####    #     #    #              #    #    #
#    #    #   #    #              #    #     #    #   #    #     #    #              #    #    #
#    #    #   #    #      #########    #     #    #   #    #     #    #########      #    #    #
#    #    #   #    #     #             #     #    #   #    #     #             #     #    #    #
#     ####     ####       #############       ####     ####       #############       ####     #
#                                                                                              #
#   Department of Energy                                                                       #
#   Politecnico di Milano                                                                      #
#   Author: Stefano Rebughini <stefano.rebughini@polimi.it>                                    #
#                                                                                              #
################################################################################################
#                                                                                              #
#   License                                                                                    #
#                                                                                              #
#   This file is part of ASALI.                                                                #
#                                                                                              #
#   ASALI is free software: you can redistribute it and/or modify                              #
#   it under the terms of the GNU General Public License as published by                       #
#   the Free Software Foundation, either version 3 of the License, or                          #
#   (at your option) any later version.                                                        #
#                                                                                              #
#   ASALI is distributed in the hope that it will be useful,                                   #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of                             #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                              #
#   GNU General Public License for more details.                                               #
#                                                                                              #
#   You should have received a copy of the GNU General Public License                          #
#   along with ASALI. If not, see <http://www.gnu.org/licenses/>.                              #
#                                                                                              #
##############################################################################################*/

namespace ASALI
{
    void checkDict(std::vector<std::string> dict, unsigned int dim, std::string name)
    {
        if ( dict.size() != dim )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> wrong dictionary format for || " << name << " ||" << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    
    double conversionToMeter(std::string dim)
    {
        double L = 0.;
        
             if(dim == "m")       {L=1.;}
        else if(dim == "micron")  {L=0.000001;}
        else if(dim == "mm")      {L=0.001;}
        else if(dim == "cm")      {L=0.01;}
        else if(dim == "dm")      {L=0.1;}
        else if(dim == "Km")      {L=1000.;}
        else if(dim == "inch")    {L=0.0254;}
        else
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> wrong || Unit dimensions || " << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        return L;
    }

    double conversionToMeterOverSeconds(std::string dim)
    {
        double L = 0.;

             if(dim == "m/s")       {L=1.;}
        else if(dim == "micron/s")  {L=0.000001;}
        else if(dim == "mm/s")      {L=0.001;}
        else if(dim == "cm/s")      {L=0.01;}
        else if(dim == "dm/s")      {L=0.1;}
        else if(dim == "Km/s")      {L=1000.;}
        else if(dim == "inch/s")    {L=0.0254;}
        else if(dim == "Km/h")      {L=1000./3600.;}
        else
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> wrong || Unit dimensions || " << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }
        return L;
    }
    
    std::string lookUpName( const std::vector<std::string> v)
    {
        std::string name = "none";

        if ( v.size() < 2 )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> Example:" << std::endl;
            std::cout << "-name <Field name>" << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }

        for ( unsigned int i=0;i<v.size();i++)
        {
            if ( v[i] == "-name" )
            {
                name = v[i+1];
            }
        }

        name.erase(std::remove(name.begin(), name.end(),'/'), name.end());

        if ( name == "none" )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> Example:" << std::endl;
            std::cout << "-name <Field name>" << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }

        return name;
    }

    std::string lookUpTime( const std::vector<std::string> v)
    {
        std::string name = "none";

        if ( v.size() < 2 )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> Example:" << std::endl;
            std::cout << "-time <Time folder>" << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }

        for ( unsigned int i=0;i<v.size();i++)
        {
            if ( v[i] == "-time" )
            {
                name = v[i+1];
            }
        }

        name.erase(std::remove(name.begin(), name.end(),'/'), name.end());

        if ( name == "none" )
        {
            std::cout << "\nASALI::FatalERROR" << std::endl;
            std::cout << "--> Example:" << std::endl;
            std::cout << "-time <Time folder>" << std::endl;
            std::cout << "\n" << std::endl;
            exit(EXIT_FAILURE);
        }

        return name;
    }
}
