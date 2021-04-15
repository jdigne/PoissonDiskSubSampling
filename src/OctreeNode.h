/**
* This file is part of the PoissonDiskSubsampling project
* @file OctreeNode.h
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
* @file OctreeNode.h 
* define an octree node
* @author Julie Digne
* @date 2012/10/10
* This program is free software: you can use, modify and/or
* redistribute it under the terms of the simplified BSD
* License. You should have received a copy of this license along
* this program. If not, see
* <http://www.opensource.org/licenses/bsd-license.html>.
*/

#ifndef OCTREENODE_H
#define OCTREENODE_H

#include <cstdlib>
#include <list>

#include "Point.h"
#include <iostream>
#include <fstream>
#include <cassert>

/**
 * @class TOctreeNode
 * implements a generic node for a generic octree
 */
template<class T>
class TOctreeNode
{
	protected :
	  
	/**
	TOctreeNode* pointer to the parent of the node
	*/
	TOctreeNode<T> *m_parent;
	
	/**
	pointer to the eight children of the node
	*/
	TOctreeNode<T> *m_child[8];
	
	/**
	* child number of the node (depends on the relative location of the node to the middle of its parent)
	* \verbatim	
	   0-------4
	  /|      /|
	 2-------6 |
	 | 1-----|-5
	 |/      |/
	 3-------7
	 
	 axis:
	 x: along direction 0->4
	 y: along direction 0->2
	 z: along direction 0->1
	 \endverbatim
	* REMARK: this convention is not important because we are dealing with a cube. It will not affect neither representation nor computations
	*/
	unsigned int m_nchild;
	
	/**number of points included in the node or in the node's children 
	 */
	unsigned int m_npts;
	
	/**origin of the node*/
	Point m_origin;
	
	/**level of the node*/
	unsigned int m_depth;
	
		/**
	x locational code
	*/
	unsigned int m_xloc;
	
	/**
	y locational code
	*/
	unsigned int m_yloc;
	
	/**
	z locational code
	*/
	unsigned int m_zloc;
	
	/**size of the node side*/
	double m_size;
	
	/**list of points contained in the node
	 (empty if the node is not a leaf)
	 */
	list<T> m_points;
	
	  
	public :
	/**
	Default constructor initializes all variables
	*/
	TOctreeNode();
	
	/**
	constructor initializes size, depth and origin
	@param size;
	@param origin
	@param depth
	*/
	TOctreeNode(Point & origin, double size,  unsigned int depth);
	
	/**
	Destructor
	*/
	~TOctreeNode();
	
	/**
	set node size
	@param size desired node size
	*/
	void setSize ( double size );
	
	/**
	 * get node size
	 * @return size of the node
	 */
	double getSize() const;
	
	/**returns the number of points contained in the node
	@return number of points contained in the node (if leaf) or node's children
	*/
	const unsigned int& getNpts() const;

	/**
	set child number (depends on the relative location of the node to the middle of its parent)
	@param a child number
	*/
	void setNchild(unsigned int a);

	/**get the child number
	 *@return child number 
	 */
	const unsigned int& getNChild() const;

	/**
	set the origin of the node by a point structure
	@param pt Origin point
	*/
	void setOrigin (Point &pt);
	
	/**get the origin of a given node
	 *@return origin
	 */
	Point getOrigin() const;

	/**
	set parent of a node
	@param node pointer to the parent node
	*/
	void setParent ( TOctreeNode<T> *node );
	
	/**
	get parent of a node
	@return TOctreeNode* pointer to the parent node
	*/
	TOctreeNode<T>* getParent() const;
	
	/**get child of a node
	 * @param index of the child
	 * @return child node
	 */
	TOctreeNode<T>* getChild(unsigned int index);
	
	/**
	set level of a node
	@param l level
	*/
	void setDepth ( unsigned int l );
	
	/**get depth of the node
	@return depth
	*/
	unsigned int getDepth() const;
	
	/**
	check if a point given by its coordinates is inside a node
	@param x coordinates of the point
	@param y
	@param z
	@return true if the point is inside
	*/
	bool isInside(double x,double y,double z) const;
	
	
	/**
	check if a point given by its coordinates is inside a node
	@param p  point
	@return true if the point is inside
	*/
	bool isInside(const Point &p) const;

	/**
	check if a point given by its coordinates is inside or in a band around the node
	@param p  point
	@param d width of the band around the node
	@return true if the point is inside
	*/
	bool isInside(const Point &p, double d) const;
	
	
	/**
	locationnal code method
	@return unsigned int x locationnal code of the node
	*/
	unsigned int getXLoc() const;
	
	/**
	locationnal code method
	@return unsigned int y locationnal code of the node
	*/
	unsigned int getYLoc() const;
	
	/**
	locationnal code method
	@return unsigned int z locationnal code of the node
	*/
	unsigned int getZLoc() const;
	
	/**
	locationnal code method
	@param Xloc locationnal code of the node
	*/
	void setXLoc(unsigned int Xloc);
	
	/**
	locationnal code method
	@param Yloc locationnal code of the node
	*/
	void setYLoc(unsigned int Yloc);
	
	/**
	locationnal code method
	@param Zloc locationnal code of the node
	*/
	void setZLoc(unsigned int Zloc);
	
	/**get a pointer to the list of points
	 * @return pointer to the beginning of the list
	 */
	typename std::list<T>::iterator points_begin();
	
	/**get a pointer to the end of the list of points
	 * @return pointer to the end of 'points'
	 */
	typename std::list<T>::iterator points_end();
		
	/**get a const pointer to the list of points
	 * @return const pointer to the beginning of the list
	 */
	typename std::list<T>::const_iterator points_begin() const;
	
	/**get a const pointer to the end of the list of points
	 * @return const pointer to the end of 'points'
	 */
	typename std::list<T>::const_iterator points_end() const;
	
	/**add a point to the list of points included in the cell
	 * PREREQUISITE: the node is a leaf in the octree
	 * @param pt point to add
	 */
	void addPoint(T &pt);
	
	/**build the i^th child of the node
	 * @param index child index
	 * @param origin origin of the node
	 * @return pointer to the created node 
	 */
	TOctreeNode<T>* initializeChild(unsigned int index, Point origin);
};


template<class T>
TOctreeNode<T>::TOctreeNode()
{
	for(int i = 0 ; i <8 ; i++)
	  m_child[i] = NULL;
	m_parent = NULL;
	m_xloc = m_yloc = m_zloc =0;
	m_depth = 0;
	m_npts = 0;
	m_origin = Point();
	m_size = 0.0;
}

template<class T>
TOctreeNode<T>::TOctreeNode(Point& origin, double size, unsigned int depth)
{
	for(int i = 0 ; i <8 ; i++)
	  m_child[i] = NULL;
	m_parent = NULL;
	m_xloc = m_yloc = m_zloc =0;
	m_depth = depth;
	m_npts = 0;
	m_origin = origin;
	m_size = size;
}

template<class T>
TOctreeNode<T>::~TOctreeNode()
{
        m_points.clear();
	m_xloc = m_yloc = m_zloc =0;
	m_depth = 0;
	m_npts = 0;
	for(int i = 0; i<8 ; i++)
	  delete m_child[i];
	m_parent = NULL;
	m_origin = Point();
	m_size = 0.0;
}


template<class T>
unsigned int TOctreeNode<T>::getDepth() const
{
    return m_depth;
}

template<class T>
void TOctreeNode<T>::setDepth(unsigned int l)
{
    m_depth = l;
}

template<class T>
double TOctreeNode<T>::getSize() const
{
  return m_size;
}

template<class T>
void TOctreeNode<T>::setSize(double size)
{
    m_size = size;
}


template<class T>
const unsigned int& TOctreeNode<T>::getNpts() const
{
    return m_npts;
}


template<class T>
const unsigned int& TOctreeNode<T>::getNChild() const
{
    return m_nchild;
}

template<class T>
void TOctreeNode<T>::setNchild(unsigned int a)
{
    m_nchild = a;
}


template<class T>
void TOctreeNode<T>::setParent(TOctreeNode *parent)
{
    m_parent = parent;
}

template<class T>
TOctreeNode<T>* TOctreeNode<T>::getParent() const
{
    return m_parent;
}

template<class T>
TOctreeNode<T>* TOctreeNode<T>::getChild(unsigned int index) 
{
    unsigned int i = index % 8;
    return m_child[i];
}


template<class T>
unsigned int TOctreeNode<T>::getXLoc() const
{
  return m_xloc;
}

template<class T>
void TOctreeNode<T>::setXLoc(unsigned int xloc)
{
    m_xloc = xloc;
}

template<class T>
unsigned int TOctreeNode<T>::getYLoc() const
{
    return m_yloc;
}

template<class T>
void TOctreeNode<T>::setYLoc(unsigned int yloc)
{
    m_yloc = yloc;
}

template<class T>
unsigned int TOctreeNode<T>::getZLoc() const
{
    return m_zloc;
}

template<class T>
void TOctreeNode<T>::setZLoc(unsigned int zloc)
{
    m_zloc = zloc;
}

template<class T>
bool TOctreeNode<T>::isInside(double x, double y, double z) const
{
    
	if ( ( x >= m_origin.x() )&&( x < m_origin.x() + m_size)
	  && ( y >= m_origin.y() )&&( y < m_origin.y() + m_size)
	  && ( z >= m_origin.z() )&&( z < m_origin.z() + m_size))
		return true;
	else
		return false;
}


template<class T>
bool TOctreeNode<T>::isInside(const Point &p) const
{
	if ( ( p.x() >= m_origin.x() )&&( p.x() < m_origin.x() + m_size)
	  && ( p.y() >= m_origin.y() )&&( p.y() < m_origin.y() + m_size)
	  && ( p.z() >= m_origin.z() )&&( p.z() < m_origin.z() + m_size))
		return true;
	else
		return false;
}

template<class T>
bool TOctreeNode<T>::isInside(const Point &p, double d) const
{
  double offset = m_size + d;
	if ( ( p.x() >= m_origin.x() - d )&&( p.x() < m_origin.x() + offset)
	  && ( p.y() >= m_origin.y() - d )&&( p.y() < m_origin.y() + offset)
	  && ( p.z() >= m_origin.z() - d )&&( p.z() < m_origin.z() + offset))
		return true;
	else
		return false;
}

template<class T>
void TOctreeNode<T>::setOrigin(Point& pt)
{
    m_origin = pt;
}


template<class T>
Point TOctreeNode<T>::getOrigin() const
{
  return m_origin;
}

template<class T>
typename std::list<T>::iterator TOctreeNode<T>::points_begin()
{
  return m_points.begin();
}

template<class T>
typename std::list<T>::iterator TOctreeNode<T>::points_end()
{
  return m_points.end();
}

template<class T>
typename std::list<T>::const_iterator TOctreeNode<T>::points_begin() const
{
  return m_points.begin();
}

template<class T>
typename std::list<T>::const_iterator TOctreeNode<T>::points_end() const
{
  return m_points.end();
}
template<class T>
void TOctreeNode<T>::addPoint(T &t)
{
    m_points.push_back(t);
    m_npts++;
}

template<class T>
TOctreeNode< T >* TOctreeNode<T>::initializeChild(unsigned int index, Point origin)
{
    double size = m_size/2.0;
    unsigned int depth= m_depth -1;
    m_child[index] = new TOctreeNode<T>(origin, size, depth);
    m_child[index]->setParent(this);
    m_child[index]->setNchild(index);

  return m_child[index];
}



#endif
