/**
* This file is part of the PoissonDiskSubsampling project
* @file main.cpp
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
#include <iostream>
#include <sstream>
#include <ctime>
#include <getopt.h>
#include <vector>

#include "Octree.h"
#include "OctreeIterator.h"
#include "utilities.h"
#include "FileIO.h"

#include "types.h"
#include "SampleSelection.h"


int main(int argc, char **argv) {
  
  //handling command line options
  int c;
  stringstream f;
  string infile, outfile;
  double radius = -1;
  int radius_flag = -1;
  int infile_flag = -1;
  int outfile_flag = -1;
  int ascii_flag = -1;
  
  while( (c = getopt(argc,argv, "i:o:r:a")) != -1)
  {
    switch(c)
    {
      case 'i':
      {
	f.clear();
      	f<<optarg;
	f>>infile;
	infile_flag = 1;
      	break;
      }
      case 'o':
      {
      	f.clear();
	f<<optarg;
	f>>outfile;
	outfile_flag = 1;
      	break;
      }
      case 'r': 
      {
	f.clear();
	f << optarg;
	f >> radius;
	radius_flag = 1;
	break;
      }
      case 'a':
      {
        ascii_flag = 1;
        break;
      }
    }    
  }

  if(infile_flag == -1)
  {
    std::cerr<<"No input file given (use the -i option)"<<std::endl;
    return EXIT_FAILURE;
  }
  
  if(outfile_flag == -1)
  {
    std::cerr<<"No output file given (use the -o option)"<<std::endl;
    return EXIT_FAILURE;
  }
  
  
  if(radius_flag == -1)
  {
    std::cerr<<"No radius given (use the -r option)"<<std::endl;
    return EXIT_FAILURE;
  }
  
  time_t start,end;
  
  Octree octree;
 
  std::time(&start);
  bool ok;
  ok = FileIO::readAndSortOrientedPoints(infile.c_str(),octree,radius); 
  
  if( !ok )
  {
      std::cerr<<"Pb opening the file; exiting."<<std::endl;
      return EXIT_FAILURE;
  }
    
  std::time(&end);
  
  std::cout<<"Octree with depth "<<octree.getDepth()<<" created."<<std::endl;
  std::cout<<"Octree contains "<<octree.getNpoints()<<" points. The bounding box size is "<<octree.getSize()<<std::endl;
  std::cout<<"Reading and sorting points in this octree took "<<difftime(end,start)<<" s."<<std::endl;
 
  std::cout<<"Octree statistics"<<std::endl;
  octree.printOctreeStat();

  OctreeIterator iterator(&octree);

  if(radius>0)
    iterator.setR(radius);
  
  std::time(&start);

  SampleSelection selection(radius, &octree, &iterator);
  selection.performDartThrowingSelection();

  std::time(&end);
  
  std::cout<<selection.getNSelected()<<" selected points."<<std::endl;
  std::cout<<"Selecting the points took "<<difftime(end,start)<<" s."<<std::endl;

  std::string output = outfile;
  if(ascii_flag == -1)
  {
    output.append("_seeds.off");
    if(! FileIO::savePointsOFF(output.c_str(), octree, selection.getNSelected()))
    {
        std::cerr<<"Pb saving the seeds (OFF); exiting."<<std::endl;
        return EXIT_FAILURE;
    }
  }
  else
  {
    output.append("_seeds.asc");
    if(! FileIO::savePoints(output.c_str(), octree))
    {
        std::cerr<<"Pb saving the seeds (ASCII); exiting."<<std::endl;
        return EXIT_FAILURE;
    }
  }
  
  return EXIT_SUCCESS;
}
