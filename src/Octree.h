/**
* This file is part of the PoissonDiskSubsampling project
* @file Octree.h
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
* @file Octree.h
* declares an octree storing structure 
* @author Julie Digne
* @date 2012/10/10
* This program is free software: you can use, modify and/or
* redistribute it under the terms of the simplified BSD
* License. You should have received a copy of this license along
* this program. If not, see
* <http://www.opensource.org/licenses/bsd-license.html>.
*/

#ifndef OCTREE_H
#define OCTREE_H

#include "utilities.h"
#include "Point.h"
#include "OctreeNode.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

template<class T>
class TOctree
{
  public : //constructors/destructors
	/**default constructor
	*/
	TOctree();
	
	/**initialize an octree with given parameters
	 * @param depth depth of the octree
	 */
	TOctree(unsigned int depth);
	
	/**initialize an octree with given parameter
	 * @param origin origin of the octree
	 * @param size  size of the loose bounding box
	 * @param depth depth of the octree
	 */
	TOctree(Point &origin, double size, unsigned int depth);
	
	/**Destructor
	*/
	~TOctree();
	
  public : //accessors and modifiers

	/**get octree depth
	 * @return depth
	 */
	unsigned int  getDepth() const;
	
	/**set octree depth
	 * @param depth
	 */
	void setDepth(unsigned int depth);
	
	/**get origign
	 *  @return origin of the octree
	 */
	const Point& getOrigin() const;
	
	/**get number of points
	 * @return number of points
	 */
	 unsigned int getNpoints() const;
	 
	 /**get side size of the octree
	  * @return size of the octree
	  */
	 double getSize() const;
	 
	 /**set size of the octree
	  * @param size
	  */
	 void setSize(double size);
	 
	 /**get the bin size
	  @return binsize
	  */
	 unsigned int getBinSize() const;
	 
	 
	 /**get root of the octree
	  * @return root of the octree
	  */
	 TOctreeNode<T>* getRoot() const;
	 

  public : //adding points
   
	/**initialize the octree with the origin and size
	 * @param origin origin of the octree
	 * @param size side size
	 **/
	void initialize(Point & origin, double size);


	/**Adding a point to the octree
	 * @param pt point to add
	 */
	void addPoint(T &pt);

	/**
	 * Adding a batch of points to the octree
	 * @param begin begin iterator of the batch
	 * @param end end iterator of the batch
	 * @return number of added points
	 */
	template<class Iterator>
	unsigned int addPoints(Iterator begin, Iterator end);
	
	/**print the mean number of points per non empty cell at each level*/
	void printOctreeStat();
	
	/**get all nodes at given depth
	 * @param depth input depth
	 * @param starting_node node to start from
	 * @param[out] nodes vector of nodes
	 * */
  void getNodes(unsigned int depth, TOctreeNode<T>* starting_node,
                std::vector<TOctreeNode<T>* > &nodes);
	
	/**get nodes in separate buckets according to the parity of the indices with respect to the three coordinates (8 buckets)
      process each node separately
	 * @param depth input depth
	 * @param starting_node node to start from
	 * @param[out] node_collection vector of vector of nodes (each vector of nodes can be processed separately)
      */
	void getNodes(unsigned int depth, TOctreeNode<T>* starting_node, std::vector< std::vector<TOctreeNode<T>* > > &node_collection); 
    
	
  protected :
	/**Maximum depth of the octree*/
	unsigned int m_depth;
	
	/**Number of points sorted in the octree*/
	unsigned int m_npoints;
	
	/**Origin of the octree*/
	Point m_origin;
	
	/**size of the side of the octree*/
	double m_size;
	
	/**BinSize
	 * the bin size is an octree parameter that determines the locational code of each node
	 */
	unsigned int m_binsize;
	
	/**root of the octree*/
	TOctreeNode<T> *m_root;
	
	/**number of non-empty cells per level*/
	std::vector<unsigned int> m_nb_non_empty_cells;
};

template<class T>
TOctree<T>::TOctree()
{
  m_size = 0;
  m_depth = 0;
  m_binsize = 0;
  m_npoints = 0;
  m_origin = Point();
  m_root = new TOctreeNode<T>();
  m_root->setDepth(0);
}


template<class T>
TOctree<T>::TOctree(unsigned int depth)
{
  m_size = 0;
  m_depth = depth;
  m_binsize = pow2(depth);
  m_npoints = 0;
  m_root = NULL;
  m_nb_non_empty_cells.assign(depth,0);
}


template<class T>
TOctree<T>::TOctree(Point& origin, double size, unsigned int depth)
{
  m_size = size;
  m_depth = depth;
  m_binsize = pow2(depth);
  m_origin = origin;
  m_npoints = 0;
  m_root = NULL;
  m_nb_non_empty_cells.assign(depth,0);
}

template<class T>
TOctree<T>::~TOctree()
{
  m_size = 0;
  m_depth = 0;
  m_binsize = 0;
  m_npoints = 0;
  m_origin = Point();
  
  if(m_root != NULL)
  {
    delete m_root;
    m_root = NULL;
  }
  m_nb_non_empty_cells.clear();
}


template<class T>
void TOctree<T>::initialize(Point& origin, double size)
{
    m_size = size;
    m_origin = origin; 
    
    m_root = new TOctreeNode<T>(m_origin, m_size, m_depth);
    
    m_root->setXLoc(0);
    m_root->setYLoc(0);
    m_root->setZLoc(0);

    m_root->setParent(NULL);
}


template<class T>
unsigned int TOctree<T>::getDepth() const
{
  return m_depth;
}

template<class T>
void TOctree<T>::setDepth(unsigned int depth)
{
  m_depth = depth;
  m_binsize = pow2(depth);
  m_nb_non_empty_cells.clear();
  m_nb_non_empty_cells.assign(depth,0);
}

template<class T>
unsigned int TOctree<T>::getNpoints() const
{
  return m_npoints;
}


template<class T>
double TOctree<T>::getSize() const
{
  return m_size;
}

template<class T>
void TOctree<T>::setSize(double size)
{
  m_size = size;
}

template<class T>
unsigned int TOctree<T>::getBinSize()
const
{
  return m_binsize;
}


template<class T>
const Point& TOctree<T>::getOrigin() const
{
  return m_origin;
}

template<class T>
TOctreeNode<T>* TOctree<T>::getRoot() const
{
    return m_root;
}

template<class T>
template<class Iterator>
unsigned int TOctree<T>::addPoints(Iterator begin, Iterator end)
{
  Iterator it = begin;
  
  while(it != end)
  {
    T &a = *it;
    addPoint(a);
    ++it;
  }
  return m_npoints;
}


template<class T>
void TOctree<T>::addPoint(T& pt)
{
  unsigned int codx=(unsigned int)((pt.x() - m_origin.x()) / m_size * m_binsize);
  unsigned int cody=(unsigned int)((pt.y() - m_origin.y()) / m_size * m_binsize);
  unsigned int codz=(unsigned int)((pt.z() - m_origin.z()) / m_size * m_binsize);
  TOctreeNode<T> *node=getRoot();
  unsigned int l=node->getDepth()-1;
  
  //traverse the octree until we reach a leaf
  while(node->getDepth() != 0)
  {
    unsigned int childBranchBit=1<<l;
    unsigned int x = ( ( codx & childBranchBit) >> l );
    unsigned int y = ( ( cody & childBranchBit) >> l );
    unsigned int z = ( ( codz & childBranchBit) >> l );
    unsigned int childIndex = (x<<2) + (y<<1) + z;
    
    if(node->getChild(childIndex) == NULL)
    {
      double childSize = node->getSize()/2.0;
      unsigned int childDepth = node->getDepth() - 1; 
      Point origin = node->getOrigin();
      Point childOrigin = Point( origin.x()  + x * childSize,
				   origin.y() + y * childSize,
				   origin.z() + z * childSize);
      
     
      TOctreeNode<T> *child = node->initializeChild(childIndex, childOrigin); 
      
      child->setXLoc( node->getXLoc() + ( x<<(childDepth) ) );
      child->setYLoc( node->getYLoc() + ( y<<(childDepth) ) );
      child->setZLoc( node->getZLoc() + ( z<<(childDepth) ) );
      m_nb_non_empty_cells[childDepth] += 1;
    }
    node = node->getChild(childIndex);
    l--;
  }
  
  //add the point to the leaf.
  node->addPoint(pt);
  m_npoints++;
}


template<class T>
void TOctree<T>::getNodes(unsigned int depth, TOctreeNode<T> *starting_node, std::vector< TOctreeNode<T>* >& nodes)
{
    if(starting_node->getDepth() == depth)
	nodes.push_back(starting_node);
    else
    {
	for(int i=0;i<8;i++)
	  if(starting_node->getChild(i) != NULL)
	    getNodes(depth, starting_node->getChild(i), nodes);
    }
}

template<class T>
void TOctree<T>::getNodes(unsigned int depth, TOctreeNode<T> *starting_node, std::vector< std::vector<TOctreeNode<T>* > >& node_collection)
{
    std::vector< TOctreeNode<T>* > nodes;
    getNodes(depth, starting_node, nodes);
    
    for(int i = 0 ; i < 8; ++i)
    {
      std::vector<TOctreeNode<T>* > ov;
      node_collection.push_back(ov);
    }
   
    while(! nodes.empty())
    {
        TOctreeNode<T> *node = nodes.back();
	nodes.pop_back();
	unsigned int nchild = node->getNChild();
	node_collection[nchild].push_back(node); 
    }
}

template<class T>
void TOctree<T>::printOctreeStat()
{
    double size = m_size;
    for(int i = m_depth-1; i >= 0; i--)
    {
      std::cout<<"level "<<i<<" : size :"<<size<<" ; mean number of points: "<< (double)m_npoints/((double)m_nb_non_empty_cells[i])<<std::endl;
      size = size / 2.0;
    }
}

#endif
