/**
* This file is part of the PoissonDiskSubsampling project
* @file Sample.cpp 
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
#include "Sample.h"

Sample::Sample() :Point()
{
  m_nx = m_ny = m_nz = 0.0;
  m_selected = true;
  m_covered = false;
  m_ncovered =0;
}

Sample::Sample(double x, double y, double z): Point(x, y, z)
{
  m_nx = m_ny = m_nz = 0.0;
  m_t1x = m_t1y = m_t1z = 0.0;
  m_selected = true;
  m_covered = false;
  m_ncovered =0;
}

Sample::Sample(double x, double y, double z, double nx, double ny, double nz): Point(x, y, z)
{
  m_nx = nx;
  m_ny = ny;
  m_nz = nz;
  m_t1x = m_t1y = m_t1z = 0.0;
  m_selected = true;
  m_covered = false;
  m_ncovered =0;
}

double Sample::nx() const
{
    return m_nx;
}

double Sample::ny() const
{
    return m_ny;
}

double Sample::nz() const
{
    return m_nz;
}

double Sample::t1x() const
{
  return m_t1x;
}

double Sample::t1y() const
{
  return m_t1y;
}

double Sample::t1z() const
{
  return m_t1z;
}


void Sample::set_nx(double nx)
{
  m_nx = nx;
}

void Sample::set_ny(double ny)
{
  m_ny = ny;
}

void Sample::set_nz(double nz)
{
  m_nz = nz;
}

void Sample::set_t1x(double t1x)
{
  m_t1x = t1x;
}

void Sample::set_t1y(double t1y)
{
  m_t1y = t1y;
}

void Sample::set_t1z(double t1z)
{
  m_t1z = t1z;
}


bool Sample::isCovered() const
{
    return m_covered;
}

void Sample::setCovered(bool covered)
{
  m_covered = covered;
}

bool Sample::isSelected() const
{
  return m_selected;
}

void Sample::setSelected(bool selected)
{
  m_selected = selected;
}


unsigned int Sample::getNCovered() const
{
    return m_ncovered;
}


void Sample::increaseNCovered()
{
  m_ncovered++; 
}

void Sample::decreaseNCovered()
{
   if(m_ncovered != 0)
     m_ncovered--;
}




std::ostream& operator<<(std::ostream& output, const Sample& p) {
  output <<p.x()<<"\t"<<p.y()<<"\t"<<p.z()<<"\t"
	    <<p.nx()<<"\t"<<p.ny()<<"\t"<<p.nz()<<std::endl;
	      
  return output; 
}

