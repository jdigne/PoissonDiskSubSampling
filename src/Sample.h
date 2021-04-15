/**
* This file is part of the PoissonDiskSubsampling project
* @file Sample.h
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
#ifndef SAMPLE_H
#define SAMPLE_H

#include <cstdlib>
#include <iostream>
#include <cstdio>
#include "Point.h"


class Sample : public Point
{
  /**print the sample to an ostream
  @param output ostream to write the values to
  @param heightImg descriptor to print out
  @return modified ostream
  */
  friend std::ostream& operator<<(std::ostream& output, const Sample &p);
  
  private :
  double m_nx, m_ny, m_nz;

  double m_t1x, m_t1y, m_t1z;

  bool m_selected;
  
  bool m_covered;

  unsigned int m_ncovered;
  
  public :
  /**constructor*/
  Sample();

  /**constructor*/
  Sample(double x,double y, double z);
  
  /**constructor*/
  Sample(double x,double y, double z, double nx, double ny,double nz);
  
  public :

  double nx() const;
  double ny() const;
  double nz() const;
  
  double t1x() const;
  double t1y() const;
  double t1z() const;
  
  unsigned int getNCovered() const;
  
  void increaseNCovered();
  
  void decreaseNCovered();
  
  void set_nx(double nx);
  void set_ny(double ny);
  void set_nz(double nz);
 
  void set_t1x(double t1x);
  void set_t1y(double t1y);
  void set_t1z(double t1z); 
  
  bool isSelected() const;
  void setSelected(bool done);
  
  bool isCovered() const;
  void setCovered(bool done);
};


#endif
