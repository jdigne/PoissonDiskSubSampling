/**
* This file is part of the PoissonDiskSubsampling project
* @file FileIO.cpp
* @author Julie Digne
* file defining methods to read points from a file
* see FileIO.h for methods declaration
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

#include "FileIO.h"
#include<iostream>
#include<fstream>
#include<istream>
#include<ostream>
#include<sstream>
#include <iomanip>

using namespace std;


FileIO::FileIO()
{
}

FileIO::~FileIO()
{
}

bool FileIO::readAndSortPoints(const char* filename, Octree& octree, double min_radius)
{
      ifstream in;
      in.open(filename);
      
      if(!in)
      {
	std::cerr<<"File "<<filename<<" could not be opened"<<std::endl;
	return false;
      }
      
	string firstline;
	
	getline(in, firstline);
	
	 istringstream line_in(firstline);
	 string word;
	int nword = 0;
	 while (line_in>> word)
	   nword++;
	
	in.clear() ;
	in.seekg(0, ios::beg);
	
	double x,y,z;
	in >> x >> y >> z;

	list<Sample> input_samples;
	input_samples.push_back(Sample(x,y,z));
	
	double xmin, ymin, zmin, xmax, ymax, zmax;
	xmin = xmax = x;
	ymin = ymax = y;
	zmin = zmax = z;
	
	while( in >> x >> y >> z)
	{
	    input_samples.push_back(Sample(x,y,z));
	 
	    xmin = x < xmin ? x : xmin;
	    xmax = x > xmax ? x : xmax;
	    ymin = y < ymin ? y : ymin;
	    ymax = y > ymax ? y : ymax;
	    zmin = z < zmin ? z : zmin;
	    zmax = z > zmax ? z : zmax;
	}
	in.close();
	
	std::cout<<input_samples.size()<<" points read"<<std::endl;
	
	double lx = xmax - xmin;
	double ly = ymax - ymin;
	double lz = zmax - zmin;
	
	double size = lx > ly ? lx : ly;
	size = size > lz ? size : lz;

	
	size = 1.1 * size;
	double margin;
	
	
	if(min_radius > 0)
	{
	  unsigned int depth = (unsigned int)ceil( log2( size / (min_radius) ));
	  double adapted_size = pow2(depth) * min_radius;
	  margin = 0.5 * (adapted_size - size);
	  size = adapted_size;
	  octree.setDepth(depth);
	}
	else
	{
	  margin = 0.05 * size;
	}
	
	double ox = xmin - margin;
	double oy = ymin - margin;
	double oz = zmin - margin;
	Point origin(ox,oy,oz);

	octree.initialize(origin, size);

	octree.addPoints(input_samples.begin(), input_samples.end());
	
	return true;
}

bool FileIO::readAndSortOrientedPoints(const char* filename, Octree& octree, double min_radius)
{
      ifstream in;
      in.open(filename);
      
      if(!in)
      {
	std::cerr<<"File "<<filename<<" could not be opened"<<std::endl;
	return false;
      }
      
	string firstline;
	
	getline(in, firstline);
	
	 istringstream line_in(firstline);
	 string word;
	int nword = 0;
	 while (line_in>> word)
	   nword++;
	
	if(nword == 3)
	{
	  cerr<< "Only three doubles per line: unoriented points"<<endl;
        readAndSortPoints(filename, octree, min_radius);
        return true;
	}
	
//	if( nword != 6)
//	{
//	  cerr<<"each point must be given by 6 values (position + normal) : x y z nx 
//ny nz"<<endl;
//	  return false;
//	}
	
	in.clear() ;
	in.seekg(0, ios::beg);
	
	double x,y,z,nx,ny,nz;
	in >> x >> y >> z >> nx >> ny >> nz;

	list<Sample> input_samples;
	input_samples.push_back(Sample(x,y,z,nx,ny,nz));
	
	double xmin, ymin, zmin, xmax, ymax, zmax;
	xmin = xmax = x;
	ymin = ymax = y;
	zmin = zmax = z;
	
	while( in >> x >> y >> z >> nx >> ny >> nz)
	{
	    input_samples.push_back(Sample(x,y,z,nx,ny,nz));
	 
	    xmin = x < xmin ? x : xmin;
	    xmax = x > xmax ? x : xmax;
	    ymin = y < ymin ? y : ymin;
	    ymax = y > ymax ? y : ymax;
	    zmin = z < zmin ? z : zmin;
	    zmax = z > zmax ? z : zmax;
	}
	in.close();
	
	std::cout<<input_samples.size()<<" points read"<<std::endl;
	
	double lx = xmax - xmin;
	double ly = ymax - ymin;
	double lz = zmax - zmin;
	
	double size = lx > ly ? lx : ly;
	size = size > lz ? size : lz;

	
	size = 1.1 * size;
	double margin;
	
	
	if(min_radius > 0)
	{
	  unsigned int depth = (unsigned int)ceil( log2( size / (min_radius) ));
	  double adapted_size = pow2(depth) * min_radius;
	  margin = 0.5 * (adapted_size - size);
	  size = adapted_size;
	  octree.setDepth(depth);
	}
	else
	{
	  margin = 0.05 * size;
	}
	
	double ox = xmin - margin;
	double oy = ymin - margin;
	double oz = zmin - margin;
	Point origin(ox,oy,oz);

	octree.initialize(origin, size);

	octree.addPoints(input_samples.begin(), input_samples.end());
	
	return true;
}

 bool FileIO::savePoints(const char* filename, Octree& octree)
{ 
    unsigned int ncovered = 0;
      ofstream out;
      out.open(filename);
      
      if(!out)
	return false;
      
      OctreeNode *node = octree.getRoot();
      saveContent(node, out, ncovered);
 
      out.close();
      
      std::cout<<"Cover rate (average number of time a point is covered)"
               <<((double)ncovered)/((double)octree.getNpoints())<<std::endl;
      
      return true;
}



 bool FileIO::savePointsOFF(const char* filename, Octree& octree, int nselected)
{ 
    unsigned int ncovered = 0;
      ofstream out;
      out.open(filename);
      
      if(!out)
    return false;
      out<<"OFF"<<std::endl;
      out<<nselected<<"\t"<<0<<"\t"<<0<<std::endl;
      OctreeNode *node = octree.getRoot();
      saveContent(node, out, ncovered);
 
      out.close();
      
      std::cout<<"Cover rate (average number of time a point is covered)"
               <<((double)ncovered)/((double)octree.getNpoints())<<std::endl;
      
      return true;
}



void FileIO::saveContent(OctreeNode* node, ofstream& f,unsigned int &ncover)
{
      if(node->getDepth() != 0)
      {
	for(int i = 0; i < 8 ;i++)
	if(node->getChild(i) != NULL)
	  saveContent(node->getChild(i), f, ncover);
      }
      else if(node->getNpts() != 0)
	{
		Sample_list::const_iterator iter;
		for(iter = node->points_begin(); iter != node->points_end(); iter++)
		{
		  if(iter->isSelected())
		    f << std::fixed << std::setprecision(8)
          << iter->x() << "\t" << iter->y() << "\t" << iter->z() << "\t"
          << iter->nx() << "\t" << iter->ny() << "\t" << iter->nz()<<std::endl;
        ncover +=iter->getNCovered();
		}
	}
}

