/**
* This file is part of the PoissonDiskSubsampling project
* @file Point.h
* @author Julie Digne
*
* Copyright (c) 2013-2021 Julie Digne
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/  
/**
 * @file Point.h
 * Declares a generic point class
 * @author Julie Digne
 * @date 2012/10/10
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */ 

#ifndef POINT_H
#define POINT_H

/**
*  Generic unoriented point
*/
class Point
{
    public :
    
    Point();

    Point(double x, double y, double z);

    ~Point();

    /**access x coordinate
     @return x
     */
    double x() const;

    /**access y coordinate
     @return y
     */
    double y() const;

    /**access y coordinate
     @return y
     */
    double z() const;

    private :

    /**3D coordinates*/
    double m_x,m_y,m_z;
};

#endif
