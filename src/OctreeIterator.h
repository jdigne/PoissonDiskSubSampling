/**
* This file is part of the PoissonDiskSubsampling project
* @file OctreeIterator.h
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
 * @file OctreeIterator.h
 * defines methods for getting range neighborhoods in the octree
 * @author Julie Digne
 * @date 2012/10/12
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

#ifndef OCTREE_ITERATOR_H
#define OCTREE_ITERATOR_H

#include<cstdlib>
#include <list>
#include <map>
#include "Point.h"
#include "Octree.h"
#include "OctreeNode.h"
#include <cmath>
#include<cassert>

/**@class TOctreeIterator
 * defines methods to access range neighbors of points
 */
template<class T>
class TOctreeIterator
{
   public : //some typedefs
    typedef std::list<T*> Neighbor_star_list;
    typedef std::set<T*> Exception_set;
    typedef std::map<double, T*> Neighbor_star_map;
    typedef std::list<double> Distance_list;

   private ://class members
    /**active depth*/
    unsigned int m_activeDepth;
    
    /**active adius*/
    double m_radius;
    
    /**square active radius*/
    double m_sqradius;
    
    /**TOctree the iterator refers to*/
    TOctree<T> *m_octree;
    
    public ://constructor+destructor
     /**constructor*/
     TOctreeIterator<T>();
     
     /**constructor*/
     TOctreeIterator<T>(TOctree<T> * octree);
     
     /**destructor*/
     ~TOctreeIterator<T>();
     
   public : //accessors modifiers
     
     /**set radius and active depth accordingly
      * @param radius radius to set
      * @return false if the radius was not coherent with the structure (> bounding box size)
      */
     bool setR(double radius);
     
     /**get active radius
      * @return active radius
      */
     double getR() const;
     
     /**get square active radius
      * @return square radius
      */
      double getSquareR() const;
     
     /**set depth and radius accordingly (smallest radius for this depth)
      * @param depth depth to set
      * @return false if the depth was not coherent with the octree depth
      */
     bool setDepth(unsigned int depth);
     
     /**get active depth
      * @return activeDepth
      */
     unsigned int getDepth() const;
     
   public : //getting neighbors

      
      /**get star-neighbors of a given point
       * @param query query point
       *@param[out] neighbors list of neighbors to be filled by the method
       *@return number of neighbors
       */
      unsigned int getNeighbors(const Point &query, Neighbor_star_list &neighbors) const;
      
      
      /**get star-neighbors of a given point
       * @param query query point
       *@param[out] neighbors list of neighbors to be filled by the method
       *@param[out] distances list of neighbors distances in the same order
       *@return number of neighbors
       */
      unsigned int getNeighbors(const Point &query, Neighbor_star_list &neighbors, Distance_list &distances) const;
     
      /**get star neighbors of a given point when the node containing that point is known
       * @param query query point
       *@param query_node node containing the query point
       *@param[out] neighbors list of neighbors to be filled by the method
       *@return number of neighbors
       */
      unsigned int getNeighbors(const Point &query, TOctreeNode<T> *query_node, Neighbor_star_list &neighbors) const;
     
      /**get star neighbors of a given point when the node containing that point is known
       * @param query query point
       *@param query_node node containing the query point
       *@param[out] neighbors list of neighbors to be filled by the method
       *@param[out] distances list of neighbors distances in the same order
       *@return number of neighbors
       */
      unsigned int getNeighbors(const Point &query, TOctreeNode<T> *query_node, Neighbor_star_list &neighbors, Distance_list &distances) const;
      
     /**get neighbors of a given point sorted by their distances 
       * @param query query point
       *@param[out] neighbors map of neighbors to be filled by the method
       *@return number of neighbors
       */
     unsigned int getSortedNeighbors(const Point &query, Neighbor_star_map &neighbors) const;
     
     /**get neighbors of a given point when the node containing that point is known
      * neighbors are sorted by their distances to the query
       * @param query query point
       *@param node node containing the query point
       *@param[out] neighbors map of neighbors to be filled by the method
       *@return number of neighbors
       */
     unsigned int getSortedNeighbors(const Point &query, TOctreeNode<T> *node, Neighbor_star_map &neighbors) const;
     
      /** Look in a ball centered at query point if there is any other point than those given in the parameter set
       * @param query center point
       * @param exceptions set of elements that are allowed in the neighborhood
       * @return false if the neighborhood contains other elements than those in the exception set
       */
      bool containsOnly(const Point &query, const Exception_set &exceptions) const;  
      
      /** Look in a ball centered at query point if there is any other point than those given in the parameter set
       * @param query center point
       *@param query_node node containing the query point
       * @param exceptions set of elements that are allowed in the neighborhood
       * @return false if the neighborhood contains other elements than those in the exception set
       */
      bool containsOnly(const Point &query, TOctreeNode<T> *query_node, const Exception_set &exceptions) const;  
      
private :
    
    
     /**
      explore a node to find neighbors of a point.
      @param node (node to explore)
      @param query_point (center of the neighborhood)
      @param neighbors deque of points (at the end: neighbors of pt)
      */
     void explore(TOctreeNode<T> *node, const Point &query_point, Neighbor_star_list &neighbors) const;
      
     /**
      explore a node to look at neighbors of a point. Stops if one of those neighbors is not in the exception set
      @param node (node to explore)
      @param query_point (center of the neighborhood)
      @param check false if the neighborhood contains other elements
      * @param exceptions set of elements that are allowed in the neighborhood
      */
     void explore(TOctreeNode<T> *node, const Point &query_point, const Exception_set &exceptions, bool &check) const;
    
     /**
      explore a node to find neighbors of a point.
      @param node (node to explore)
      @param query_point (center of the neighborhood)
      @param neighbors deque of points (at the end: neighbors of pt)
      @param distances distance to the neigbors
      */
     void explore(TOctreeNode<T> *node, const Point &query_point, Neighbor_star_list &neighbors, Distance_list &distances) const;
     

     /**explore a node to find neighbors of a point and sort them according to their distance
      @param node (node to explore)
      @param query_point (center of the neighborhood)
      @param neighbors map of points sorted by their distances to the query point 
      */
     void exploreSort(TOctreeNode<T> *node, const Point &query_point, Neighbor_star_map &neighbors) const;
    
     
     /**
      follow the path given by loc-codes beginning at node
      @param node pointer to the node where the path begins (at the end is is the end of path node)
      @param xLocCode x locational code of the path
      @param yLocCode y locational code of the path
      @param zLocCode z locational code of the path
      @param k max level to look for
      */
     void traverseToLevel(TOctreeNode<T> **node,unsigned int xLocCode,unsigned int yLocCode, unsigned int zLocCode, unsigned int k) const;
     
     /**
      * get left neighbor code (along x-axis) of a cell
      * @param cell
      * @return unsigned int left neighbor xloc
      */
     unsigned int getXLeftCode(TOctreeNode<T> *cell) const ;
     
     /**
      * get left neighbor code (along y-axis) of a cell
      * @param cell
      * @return unsigned int left neighbor yloc
      */
     unsigned int getYLeftCode(TOctreeNode<T> *cell) const;
     
     /**
      * get left neighbor code (along z-axis) of a cell
      * @param cell
      * @return unsigned int left neighbor zloc
      */
     unsigned int getZLeftCode(TOctreeNode<T> *cell) const;
     
     
     /**
      * get right neighbor code (along x-axis) of a cell
      * @param cell
      * @return unsigned int right neighbor xloc
      */
     unsigned int getXRightCode(TOctreeNode<T> *cell) const;
     
     /**
      * get right neighbor code (along y-axis) of a cell
      * @param cell
      * @return unsigned int right neighbor yloc code
      */
     unsigned int getYRightCode(TOctreeNode<T> *cell) const;
     
     /**
      * get right neighbor code (along z-axis) of a cell
      * @param cell cell
      * @return unsigned int right neighbor zloc
      */
     unsigned int getZRightCode(TOctreeNode<T> *cell) const;
     
     /**compute the code of a given position (useful for locating points and traversing the octree
      @param point point to locate
      @param[out] xcode x locational code 
      @param[out] ycode y locational code 
      @param[out] zcode z locational code 
      */
     void computeCode(const Point &point, unsigned int &codx, unsigned int &cody, unsigned int &codz) const;
    
     /**return a cell containing the point at active depth
      @param point to locate
      @return the node containing the point at active depth
      */
     TOctreeNode<T>* locatePointNode(const Point &point) const; 
};

template<class T>
TOctreeIterator<T>::TOctreeIterator()
{
  m_octree = NULL;
}


template<class T>
TOctreeIterator<T>::TOctreeIterator(TOctree<T>* octree)
{
  m_octree = octree;
  m_activeDepth = m_octree->getDepth();
  m_radius = m_octree->getSize()/((double)pow2(m_activeDepth));
  m_sqradius = m_radius * m_radius;
}


template<class T>
TOctreeIterator<T>::~TOctreeIterator()
{
  m_octree = NULL;
}

template<class T>
bool TOctreeIterator<T>::setDepth(unsigned int depth)
{
  if(depth<= m_octree->getDepth())
  {
    m_activeDepth = depth;
    m_radius = m_octree->getSize()/((double)pow2(depth));
    m_sqradius = m_radius * m_radius;
    return true;
  }
  return false;
}

template<class T>
unsigned int TOctreeIterator<T>::getDepth() const
{
  return m_activeDepth;
}


template<class T>
double TOctreeIterator<T>::getR() const
{
  return m_radius;
}

template<class T>
bool TOctreeIterator<T>::setR(double radius)
{
  if(radius < m_octree->getSize())
  {
     m_radius = radius;
     m_sqradius = m_radius * m_radius;
     m_activeDepth = (unsigned int)(m_octree->getDepth() - floor( log2( m_octree->getSize() / (2.0*m_radius) )));
     return true;
  }
  return false;
}

template<class T>
double TOctreeIterator<T>::getSquareR() const
{
  return m_sqradius;
}


template<class T>
unsigned int TOctreeIterator<T>::getNeighbors(const Point& query, Neighbor_star_list& neighbors) const
{
	TOctreeNode<T> *node = locatePointNode(query);
	unsigned int n = getNeighbors(query, node, neighbors);
	return n;
}

template<class T>
unsigned int TOctreeIterator<T>::getNeighbors(const Point& query, Neighbor_star_list& neighbors, Distance_list &distances) const
{
	TOctreeNode<T> *node = locatePointNode(query);
	unsigned int n = getNeighbors(query, node, neighbors, distances);
	return n;
}



template<class T>
unsigned int TOctreeIterator<T>::getNeighbors(const Point& query, TOctreeNode<T>* query_node, Neighbor_star_list& neighbors) const
{	
  Point octree_origin = m_octree->getOrigin();
  Point node_origin = query_node->getOrigin();
  double node_size = query_node->getSize();
  double octree_size = m_octree->getSize();
  
  if(query_node->getDepth() ==  m_activeDepth)
  {
    //find neighboring nodes
    list<unsigned int> xloc, yloc, zloc;
    xloc.push_back(query_node->getXLoc());
    yloc.push_back(query_node->getYLoc());
    zloc.push_back(query_node->getZLoc());
    
    if((query.x() - m_radius  < node_origin.x())&&(query.x() - m_radius > octree_origin.x()))
      xloc.push_back(getXLeftCode(query_node));
    if((query.x() + m_radius > node_origin.x() + node_size) && (query.x() + m_radius <octree_origin.x() + octree_size))
      xloc.push_back(getXRightCode(query_node));
    
    if((query.y() - m_radius < node_origin.y())&&(query.y() - m_radius >octree_origin.y()))
      yloc.push_back(getYLeftCode(query_node));
    if((query.y() + m_radius > node_origin.y() + node_size) && (query.y() + m_radius < octree_origin.y() + octree_size))
      yloc.push_back(getYRightCode(query_node));
    
    if((query.z() - m_radius  < node_origin.z())&&(query.z() - m_radius >octree_origin.z()))
      zloc.push_back(getZLeftCode(query_node));
    if((query.z() + m_radius > node_origin.z() +node_size) && (query.z() + m_radius <octree_origin.z() + octree_size))
      zloc.push_back(getZRightCode(query_node));
    
    //look inside neighboring node
      list<unsigned int>::iterator xi,yi,zi;
      xi = xloc.begin();
      while(xi != xloc.end())
      {
	yi = yloc.begin();
	while(yi != yloc.end())
	{
	  zi = zloc.begin();
	  while(zi != zloc.end())
	  {
	    TOctreeNode<T> *node=m_octree->getRoot();
	    traverseToLevel(&node, *xi, *yi, *zi, m_activeDepth);
	    if((node!=NULL)&&(node->getDepth() == m_activeDepth))
	      explore(node, query, neighbors);
	    ++zi;
	  }
	  ++yi;
	}
	++xi;
      }
  }
  else
  {
    unsigned int s = query_node->getDepth();
    list<unsigned int> xloc, yloc, zloc;
    xloc.push_back(query_node->getXLoc());
    yloc.push_back(query_node->getYLoc());
    zloc.push_back(query_node->getZLoc());
    if((query.x() - m_radius  < node_origin.x() )&&(query.x() - m_radius > octree_origin.x() ))
      xloc.push_back(getXLeftCode(query_node));
    if((query.x() + m_radius > node_origin.x() + node_size) && (query.x() + m_radius < octree_origin.x() +octree_size))
      xloc.push_back(getXRightCode(query_node));
    
    if((query.y() - m_radius  < node_origin.y() )&&(query.y() - m_radius >octree_origin.y() ))
      yloc.push_back(getYLeftCode(query_node));
    if((query.y() + m_radius > node_origin.y() + node_size) && (query.y() + m_radius < octree_origin.y()+octree_size))
      yloc.push_back(getYRightCode(query_node));
    
    if((query.z() - m_radius  < node_origin.z() )&&(query.z() - m_radius >octree_origin.z()))
      zloc.push_back(getZLeftCode(query_node));
    if((query.z() + m_radius > node_origin.z() + node_size) && (query.z() + m_radius <octree_origin.z() + octree_size))
      zloc.push_back(getZRightCode(query_node));
    
    
    //look inside neighboring nodes
      list<unsigned int>::iterator xi,yi,zi;
      xi = xloc.begin();
      while(xi != xloc.end())
      {
	yi = yloc.begin();
	while(yi != yloc.end())
	{
	  zi = zloc.begin();
	  while(zi != zloc.end())
	  {
	    TOctreeNode<T> *node=m_octree->getRoot();
	    traverseToLevel(&node,*xi,*yi,*zi,s);
	    if( (node != NULL )&&( node->getDepth() == s))
	      explore(node,query,neighbors);
	    ++zi;
	  }
	  ++yi;
	}
	++xi;
      }
  }
  return (int)neighbors.size();
}


template<class T>
unsigned int TOctreeIterator<T>::getNeighbors(const Point& query, TOctreeNode<T>* query_node, Neighbor_star_list& neighbors, Distance_list &distances) const
{	
  Point octree_origin = m_octree->getOrigin();
  Point node_origin = query_node->getOrigin();
  double node_size = query_node->getSize();
  double octree_size = m_octree->getSize();
  
  if(query_node->getDepth() ==  m_activeDepth)
  {
    //find neighboring nodes
    list<unsigned int> xloc, yloc, zloc;
    xloc.push_back(query_node->getXLoc());
    yloc.push_back(query_node->getYLoc());
    zloc.push_back(query_node->getZLoc());
    
    if((query.x() - m_radius  < node_origin.x())&&(query.x() - m_radius > octree_origin.x()))
      xloc.push_back(getXLeftCode(query_node));
    if((query.x() + m_radius > node_origin.x() + node_size) && (query.x() + m_radius <octree_origin.x() + octree_size))
      xloc.push_back(getXRightCode(query_node));
    
    if((query.y() - m_radius < node_origin.y())&&(query.y() - m_radius >octree_origin.y()))
      yloc.push_back(getYLeftCode(query_node));
    if((query.y() + m_radius > node_origin.y() + node_size) && (query.y() + m_radius < octree_origin.y() + octree_size))
      yloc.push_back(getYRightCode(query_node));
   
    if((query.z() - m_radius  < node_origin.z())&&(query.z() - m_radius >octree_origin.z()))
      zloc.push_back(getZLeftCode(query_node));
    if((query.z() + m_radius > node_origin.z() +node_size) && (query.z() + m_radius <octree_origin.z() + octree_size))
      zloc.push_back(getZRightCode(query_node));
    
    //look inside neighboring node
      list<unsigned int>::iterator xi,yi,zi;
      xi = xloc.begin();
      while(xi != xloc.end())
      {
	yi = yloc.begin();
	while(yi != yloc.end())
	{
	  zi = zloc.begin();
	  while(zi != zloc.end())
	  {
	    TOctreeNode<T> *node=m_octree->getRoot();
	    traverseToLevel(&node, *xi, *yi, *zi, m_activeDepth);
	    if((node!=NULL)&&(node->getDepth() == m_activeDepth))
	      explore(node, query, neighbors, distances);
	    ++zi;
	  }
	  ++yi;
	}
	++xi;
      }
  }
  else
  {
    unsigned int s=query_node->getDepth();
    list<unsigned int> xloc, yloc, zloc;
    xloc.push_back(query_node->getXLoc());
    yloc.push_back(query_node->getYLoc());
    zloc.push_back(query_node->getZLoc());
    if((query.x() - m_radius  < node_origin.x())&&(query.x() - m_radius > octree_origin.x()))
      xloc.push_back(getXLeftCode(query_node));
    if((query.x() + m_radius > node_origin.x() + node_size) && (query.x() + m_radius < octree_origin.x() + octree_size))
      xloc.push_back(getXRightCode(query_node));
    
    if((query.y() - m_radius  < node_origin.y())&&(query.y() - m_radius >octree_origin.y() ))
      yloc.push_back(getYLeftCode(query_node));
    if((query.y() + m_radius > node_origin.y() + node_size) && (query.y() + m_radius < octree_origin.y() + octree_size))
      yloc.push_back(getYRightCode(query_node));
    
    if((query.z() - m_radius  < node_origin.z() )&&(query.z() - m_radius >octree_origin.z()))
      zloc.push_back(getZLeftCode(query_node));
    if((query.z() + m_radius > node_origin.z() +node_size) && (query.z() + m_radius <octree_origin.z() + octree_size))
      zloc.push_back(getZRightCode(query_node));
    
    
    //look inside neighboring nodes
      list<unsigned int>::iterator xi,yi,zi;
      xi = xloc.begin();
      while(xi != xloc.end())
      {
	yi = yloc.begin();
	while(yi != yloc.end())
	{
	  zi = zloc.begin();
	  while(zi != zloc.end())
	  {
	    TOctreeNode<T> *node=m_octree->getRoot();
	    traverseToLevel(&node,*xi,*yi,*zi,s);
	    if( (node != NULL )&&( node->getDepth() == s))
	      explore(node,query,neighbors, distances);
	    ++zi;
	  }
	  ++yi;
	}
	++xi;
      }
  }
  return (int)neighbors.size();
}


template<class T>
void TOctreeIterator<T>::traverseToLevel(TOctreeNode<T>** node, unsigned int xLocCode, unsigned int yLocCode, unsigned int zLocCode, unsigned int k)
const
{
  int l=(*node)->getDepth()-1;
  
  while((*node)->getDepth()>k)
  {
    unsigned int childBranchBit=1<<l;
    unsigned int childIndex=((((xLocCode) & (childBranchBit)) >> l) << 2)
    + (((yLocCode & childBranchBit) >> l) << 1)
    + ( (zLocCode & childBranchBit) >> l);
    
    if((*node)->getChild(childIndex) != NULL)
    {
      *node=(*node)->getChild(childIndex);
      l--;
    }
    else
      break;
  }
}



template<class T>
void TOctreeIterator<T>::explore(TOctreeNode<T>* node, const Point& query_point, Neighbor_star_list &neighbors)
const
{
  	if(node->getDepth() != 0)
	{
		for(unsigned int i=0;i<8;i++)
			if(node->getChild(i) != NULL)
				explore(node->getChild(i), query_point, neighbors);
	
	}
	else if(node->getNpts() != 0)
	{
		typename std::list<T>::iterator iter;
		for(iter = node->points_begin(); iter != node->points_end(); ++iter)
		{
			double dist = dist2( query_point, *iter);
			if(dist < m_sqradius)
				neighbors.push_back(&(*iter));
		}
	}
}


template<class T>
void TOctreeIterator<T>::explore(TOctreeNode<T>* node, const Point& query_point, Neighbor_star_list &neighbors, Distance_list &distances)
const
{
  	if(node->getDepth() != 0)
	{
		for(unsigned int i=0;i<8;i++)
			if(node->getChild(i) != NULL)
				explore(node->getChild(i), query_point, neighbors, distances);
	
	}
	else if(node->getNpts() != 0)
	{
		typename std::list<T>::iterator iter;
		for(iter = node->points_begin(); iter != node->points_end(); ++iter)
		{
			double dist = dist2( query_point, *iter);
			if(dist < m_sqradius)
			{
				neighbors.push_back(&(*iter));
				distances.push_back(dist);
			}
		}
	}
}

template<class T>
unsigned int TOctreeIterator<T>::getSortedNeighbors(const Point &query, Neighbor_star_map &neighbors) const
{
	TOctreeNode<T> *node = locatePointNode(query);
	unsigned int n = getSortedNeighbors(query, node, neighbors);
	return n;
  
}

template<class T>
unsigned int TOctreeIterator<T>::getSortedNeighbors(const Point& query, TOctreeNode<T>* query_node, Neighbor_star_map &neighbors) const
{	
  Point octree_origin = m_octree->getOrigin();
  Point node_origin = query_node->getOrigin();
  double node_size = query_node->getSize();
  double octree_size = m_octree->getSize();
  
  if(query_node->getDepth() ==  m_activeDepth)
  {
    //find neighboring nodes
    list<unsigned int> xloc, yloc, zloc;
    xloc.push_back(query_node->getXLoc());
    yloc.push_back(query_node->getYLoc());
    zloc.push_back(query_node->getZLoc());
    
    if((query.x() - m_radius  < node_origin.x())&&(query.x() - m_radius > octree_origin.x()))
      xloc.push_back(getXLeftCode(query_node));
    if((query.x() + m_radius > node_origin.x() + node_size) && (query.x() + m_radius <octree_origin.x() + octree_size))
      xloc.push_back(getXRightCode(query_node));
    
    if((query.y() - m_radius < node_origin.y())&&(query.y() - m_radius >octree_origin.y()))
      yloc.push_back(getYLeftCode(query_node));
    if((query.y() + m_radius > node_origin.y() + node_size) && (query.y() + m_radius < octree_origin.y() + octree_size))
      yloc.push_back(getYRightCode(query_node));
   
    if((query.z() - m_radius  < node_origin.z())&&(query.z() - m_radius >octree_origin.z()))
      zloc.push_back(getZLeftCode(query_node));
    if((query.z() + m_radius > node_origin.z() +node_size) && (query.z() + m_radius <octree_origin.z() + octree_size))
      zloc.push_back(getZRightCode(query_node));
    
    //look inside neighboring node
      list<unsigned int>::iterator xi,yi,zi;
      xi = xloc.begin();
      while(xi != xloc.end())
      {
	yi = yloc.begin();
	while(yi != yloc.end())
	{
	  zi = zloc.begin();
	  while(zi != zloc.end())
	  {
	    TOctreeNode<T> *node=m_octree->getRoot();
	    traverseToLevel(&node, *xi, *yi, *zi, m_activeDepth);
	    if((node!=NULL)&&(node->getDepth() == m_activeDepth))
	      exploreSort(node, query, neighbors);
	    ++zi;
	  }
	  ++yi;
	}
	++xi;
      }
  }
  else
  {
    unsigned int s=query_node->getDepth();
    list<unsigned int> xloc, yloc, zloc;
    xloc.push_back(query_node->getXLoc());
    yloc.push_back(query_node->getYLoc());
    zloc.push_back(query_node->getZLoc());
    if((query.x() - m_radius  < node_origin.x())&&(query.x() - m_radius > octree_origin.x()))
      xloc.push_back(getXLeftCode(query_node));
    if((query.x() + m_radius > node_origin.x() + node_size) && (query.x() + m_radius < octree_origin.x() + octree_size))
      xloc.push_back(getXRightCode(query_node));
    
    if((query.y() - m_radius  < node_origin.y())&&(query.y() - m_radius >octree_origin.y() ))
      yloc.push_back(getYLeftCode(query_node));
    if((query.y() + m_radius > node_origin.y() + node_size) && (query.y() + m_radius < octree_origin.y() + octree_size))
      yloc.push_back(getYRightCode(query_node));
    
    if((query.z() - m_radius  < node_origin.z() )&&(query.z() - m_radius >octree_origin.z()))
      zloc.push_back(getZLeftCode(query_node));
    if((query.z() + m_radius > node_origin.z() +node_size) && (query.z() + m_radius <octree_origin.z() + octree_size))
      zloc.push_back(getZRightCode(query_node));
    
    
    //look inside neighboring nodes
      list<unsigned int>::iterator xi,yi,zi;
      xi = xloc.begin();
      while(xi != xloc.end())
      {
	yi = yloc.begin();
	while(yi != yloc.end())
	{
	  zi = zloc.begin();
	  while(zi != zloc.end())
	  {
	    TOctreeNode<T> *node=m_octree->getRoot();
	    traverseToLevel(&node,*xi,*yi,*zi,s);
	    if( (node != NULL )&&( node->getDepth() == s))
	      exploreSort(node, query, neighbors);
	    ++zi;
	  }
	  ++yi;
	}
	++xi;
      }
  }
  return (int)neighbors.size();
}

template<class T>
void TOctreeIterator<T>::exploreSort(TOctreeNode<T>* node, const Point& query_point, Neighbor_star_map &neighbors)
const
{
  	if(node->getDepth() != 0)
	{
		for(unsigned int i=0;i<8;i++)
			if(node->getChild(i) != NULL)
				exploreSort(node->getChild(i), query_point, neighbors);
	
	}
	else if(node->getNpts() != 0)
	{
		typename std::list<T>::iterator iter;
		for(iter = node->points_begin(); iter != node->points_end(); ++iter)
		{
			double dist = dist2( query_point, *iter);
			if(dist < m_sqradius)
				neighbors.insert( pair<double, T*>(dist, &(*iter)) );
		}
	}
}



template<class T>
unsigned int TOctreeIterator<T>::getXLeftCode(TOctreeNode<T>* node)
const
{
    return node->getXLoc()-0x00000001;
}

template<class T>
unsigned int TOctreeIterator<T>::getXRightCode(TOctreeNode<T>* node)
const
{
    return node->getXLoc()+(unsigned int)pow2(node->getDepth());
}

template<class T>
unsigned int TOctreeIterator<T>::getYLeftCode(TOctreeNode<T>* node)
const
{
    return node->getYLoc()-0x00000001;

}

template<class T>
unsigned int TOctreeIterator<T>::getYRightCode(TOctreeNode<T>* node)
const
{
    return node->getYLoc()+(unsigned int)pow2(node->getDepth());
}

template<class T>
unsigned int TOctreeIterator<T>::getZLeftCode(TOctreeNode<T>* node)
const
{
    return node->getZLoc()-0x00000001;
}

template<class T>
unsigned int TOctreeIterator<T>::getZRightCode(TOctreeNode<T>* node)
const
{
    return node->getZLoc()+(unsigned int)pow2(node->getDepth());
}

template<class T>
void TOctreeIterator<T>::computeCode(const Point& point, unsigned int& codx, unsigned int& cody, unsigned int& codz) const
{
    double multiplier = 1.0 / (m_octree->getSize()) * m_octree->getBinSize();
    const Point &octree_origin = m_octree->getOrigin();
    codx=(unsigned int)((point.x() - octree_origin.x()) * multiplier);
    cody=(unsigned int)((point.y() - octree_origin.y()) * multiplier);
    codz=(unsigned int)((point.z() - octree_origin.z()) * multiplier);
}

template<class T>
TOctreeNode<T>* TOctreeIterator<T>::locatePointNode(const Point& point) const
{
  unsigned int codx,cody,codz;
  computeCode(point, codx, cody, codz);
  
  TOctreeNode<T> *node = m_octree->getRoot();
  traverseToLevel(&node, codx, cody, codz, m_activeDepth);
  
  return node;
}

template<class T>
bool TOctreeIterator<T>::containsOnly(const Point& query, const Exception_set& exceptions) const
{
	TOctreeNode<T> *node = locatePointNode(query);
	return containsOnly(query, node, exceptions);
}

template<class T>
bool TOctreeIterator<T>::containsOnly(const Point& query, TOctreeNode< T >* query_node, const Exception_set& exceptions) const
{
  Point octree_origin = m_octree->getOrigin();
  Point node_origin = query_node->getOrigin();
  double node_size = query_node->getSize();
  double octree_size = m_octree->getSize();
  
  if(query_node->getDepth() ==  m_activeDepth)
  {
    //find neighboring nodes
    list<unsigned int> xloc, yloc, zloc;
    xloc.push_back(query_node->getXLoc());
    yloc.push_back(query_node->getYLoc());
    zloc.push_back(query_node->getZLoc());
    
    if((query.x() - m_radius  < node_origin.x())&&(query.x() - m_radius > octree_origin.x()))
      xloc.push_back(getXLeftCode(query_node));
    if((query.x() + m_radius > node_origin.x() + node_size) && (query.x() + m_radius <octree_origin.x() + octree_size))
      xloc.push_back(getXRightCode(query_node));
    
    if((query.y() - m_radius < node_origin.y())&&(query.y() - m_radius >octree_origin.y()))
      yloc.push_back(getYLeftCode(query_node));
    if((query.y() + m_radius > node_origin.y() + node_size) && (query.y() + m_radius < octree_origin.y() + octree_size))
      yloc.push_back(getYRightCode(query_node));
    
    if((query.z() - m_radius  < node_origin.z())&&(query.z() - m_radius >octree_origin.z()))
      zloc.push_back(getZLeftCode(query_node));
    if((query.z() + m_radius > node_origin.z() +node_size) && (query.z() + m_radius <octree_origin.z() + octree_size))
      zloc.push_back(getZRightCode(query_node));
    
    //look inside neighboring node
      list<unsigned int>::iterator xi,yi,zi;
      xi = xloc.begin();
      while (xi != xloc.end())
      {
	yi = yloc.begin();
	while (yi != yloc.end())
	{
	  zi = zloc.begin();
	  while (zi != zloc.end())
	  {
	    TOctreeNode<T> *node=m_octree->getRoot();
	    traverseToLevel(&node, *xi, *yi, *zi, m_activeDepth);
	    bool ok = true;
	    if((node!=NULL)&&( node->getDepth() == m_activeDepth))
	    {
	      explore(node, query, exceptions, ok);
	      if(!ok)
		return false;
	    }
	    ++zi;
	  }
	  ++yi;
	}
	++xi;
      }
  }
  else
  {
    unsigned int s=query_node->getDepth();
    list<unsigned int> xloc, yloc, zloc;
    xloc.push_back(query_node->getXLoc());
    yloc.push_back(query_node->getYLoc());
    zloc.push_back(query_node->getZLoc());
    if((query.x() - m_radius  < node_origin.x())&&(query.x() - m_radius > octree_origin.x()))
      xloc.push_back(getXLeftCode(query_node));
    if((query.x() + m_radius > node_origin.x() + node_size) && (query.x() + m_radius < octree_origin.x() + octree_size))
      xloc.push_back(getXRightCode(query_node));
    
    if((query.y() - m_radius  < node_origin.y())&&(query.y() - m_radius >octree_origin.y() ))
      yloc.push_back(getYLeftCode(query_node));
    if((query.y() + m_radius > node_origin.y() + node_size) && (query.y() + m_radius < octree_origin.y() + octree_size))
      yloc.push_back(getYRightCode(query_node));
    
    if((query.z() - m_radius  < node_origin.z() )&&(query.z() - m_radius >octree_origin.z()))
      zloc.push_back(getZLeftCode(query_node));
    if((query.z() + m_radius > node_origin.z() +node_size) && (query.z() + m_radius <octree_origin.z() + octree_size))
      zloc.push_back(getZRightCode(query_node));
    
    
    //look inside neighboring nodes
      list<unsigned int>::iterator xi,yi,zi;
      xi = xloc.begin();
      while(xi != xloc.end())
      {
	yi = yloc.begin();
	while(yi != yloc.end())
	{
	  zi = zloc.begin();
	  while(zi != zloc.end())
	  {
	    TOctreeNode<T> *node=m_octree->getRoot();
	    traverseToLevel(&node,*xi,*yi,*zi,s);
	    bool ok = true;
	    if( (node != NULL )&&( node->getDepth() == s))
	      explore(node, query, exceptions, ok);
	    if(!ok)
		return false;
	    ++zi;
	  }
	  ++yi;
	}
	++xi;
      }
  }
  return true;
}



template<class T>
void TOctreeIterator<T>::explore(TOctreeNode<T> *node, const Point &query_point, const Exception_set &exceptions, bool &check)
const
{
	if(!check)
	  return;

	if( node->getDepth() != 0)
	{
		unsigned int i = 0;
		while( (i<8) && (check))
		{
			if(node->getChild(i) != NULL)
				explore(node->getChild(i), query_point, exceptions, check);
			i++;
		}
	}
	else if(node->getNpts() != 0)
	{
		typename std::list<T>::iterator iter;
		for(iter = node->points_begin(); iter != node->points_end(); ++iter)
		{
			double sqdist = dist2( query_point, *iter);
			if((sqdist < m_sqradius) && (exceptions.find(&(*iter)) == exceptions.end()))
			{
			  check = false;
			  return;
			}
		}
	}
}


#endif
