/**
* This file is part of the PoissonDiskSubsampling project
* @file types.h
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
 *@file types.h
 * @author Julie Digne
 * @date 2012/10/25
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */


#ifndef TYPES_H
#define TYPES_H

#include<list>
#include<set>
#include<map>
#include "Point.h"

class Sample;

typedef std::list<Point> Point_list;
typedef std::list<Point*> Point_star_list;
typedef std::list<double> Distance_list;
typedef std::list<Point>::iterator Point_iterator;
typedef std::list<Point*>::iterator Point_star_iterator;


typedef std::list<Sample> Sample_list;
typedef std::list<Sample>::iterator  Sample_iterator;
typedef std::list<Sample*> Sample_star_list;
typedef std::list<Sample*>::iterator Sample_star_iterator;

typedef std::map<double, Sample*> Neighbor_star_map;
typedef Neighbor_star_map::iterator Neighbor_iterator;

#include "Octree.h"
typedef TOctree<Sample> Octree;

#include "OctreeNode.h"
typedef TOctreeNode<Sample> OctreeNode;
typedef std::vector<OctreeNode*> OctreeNode_vector;
typedef std::vector<OctreeNode_vector> OctreeNode_collection;


#include "OctreeIterator.h"
typedef TOctreeIterator<Sample> OctreeIterator;


#include "SampleSelection.h"
typedef TSampleSelection<Sample> SampleSelection;

#endif
