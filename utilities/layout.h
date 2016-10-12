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
    void logo()
    {
        std::cout << "\033[2J\033[1;1H" << std::endl;
        std::cout << "################################################################################################" << std::endl;
        std::cout << "#                                                                                              #" << std::endl;
        std::cout << "#     #############       #############       #############       ####                ####     #" << std::endl;
        std::cout << "#    #             #     #             #     #             #     #    #              #    #    #" << std::endl;
        std::cout << "#    #     ###     #     #    #########      #     ###     #     #    #              #    #    #" << std::endl;
        std::cout << "#    #    #   #    #     #    #              #    #   #    #     #    #              #    #    #" << std::endl;
        std::cout << "#    #     ###     #     #    #              #     ###     #     #    #              #    #    #" << std::endl;
        std::cout << "#    #             #     #    #########      #             #     #    #              #    #    #" << std::endl;
        std::cout << "#    #             #     #             #     #             #     #    #              #    #    #" << std::endl;
        std::cout << "#    #    #####    #      #########    #     #    #####    #     #    #              #    #    #" << std::endl;
        std::cout << "#    #    #   #    #              #    #     #    #   #    #     #    #              #    #    #" << std::endl;
        std::cout << "#    #    #   #    #      #########    #     #    #   #    #     #    #########      #    #    #" << std::endl;
        std::cout << "#    #    #   #    #     #             #     #    #   #    #     #             #     #    #    #" << std::endl;
        std::cout << "#     ####     ####       #############       ####     ####       #############       ####     #" << std::endl;
        std::cout << "#                                                                                              #" << std::endl;
        std::cout << "#   Department of Energy                                                                       #" << std::endl;
        std::cout << "#   Politecnico di Milano                                                                      #" << std::endl;
        std::cout << "#   Author: Stefano Rebughini <stefano.rebughini@polimi.it>                                    #" << std::endl;
        std::cout << "#                                                                                              #" << std::endl;
        std::cout << "################################################################################################" << std::endl;
    }
    
    void end()
    {
        std::cout << "\nEnd\n" << std::endl;
    }
}
