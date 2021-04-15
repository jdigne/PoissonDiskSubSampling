/**
* This file is part of the PoissonDiskSubsampling project
* @file SampleSelection.h
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
#ifndef SAMPLE_SELECTION_H
#define SAMPLE_SELECTION_H

#include "Octree.h"
#include "OctreeNode.h"
#include "OctreeIterator.h"
#include <cmath>

template<class T>
class TSampleSelection
{
  typedef std::list<T*> Sample_star_list;
  typedef std::list<T> Sample_list;
  public :
  /**constructor*/
  TSampleSelection();

  /**constructor
   @param radius selection radius
   @param octree octree
   @param iterator iterator over the octree
   */
  TSampleSelection(double radius, TOctree<T> *octree, TOctreeIterator<T> *iterator);
  
  /**destructor*/
  ~TSampleSelection();

  public : //accessors

  /**get radius
   @return radius
   */
  double getRadius();
  
  /**set radius
   *@param radius  
   */
  void setRadius(double radius);
  
  /**get the number of selected points
   @return number of points
   */
  unsigned int getNSelected() const;
  
public : //selection methods
  
  /**select points according to a covering criterium*/
  void performSelection();
  
  /**select points according to a covering criterium (dartthrowing)
   */
  void performDartThrowingSelection();
  
  private :
  
  double m_radius;
  
  double m_sqradius;
  
  double m_sigma;
  
  unsigned int m_nselected;
  
  TOctree<T> *m_octree;
  
  TOctreeIterator<T> *m_iterator;
  
  Sample_star_list m_selected_samples;
  
  
  /**select points according to a covering criterium
   @param cell constrain selection to a given cell
   */
  void performSelection(TOctreeNode<T> *cell);
   
 
  
  /**select points according to a covering criterium
   @param cell constrain selection to a given cell
   @param par parent at the right scale
   */
  void performSelection(TOctreeNode<T> *cell, TOctreeNode<T> *par);

  /**select points according to a covering criterium
   @param cell constrain selection to a given cell
   @param par parent at the right scale
   */
  void performDartThrowingSelection(TOctreeNode< T >* cell,
                               std::list<T*> &cell_selected_samples);
};


template<class T>
TSampleSelection<T>::TSampleSelection()
{
    m_octree = NULL;
    m_iterator = NULL;
    m_nselected = 0;
    setRadius(0);
}

template<class T>
TSampleSelection<T>::TSampleSelection(double radius, TOctree<T> *octree, TOctreeIterator<T> *iterator)
{
   m_octree = octree;
   m_iterator = iterator;
    m_nselected = 0;
   setRadius(radius);
   m_iterator->setR(radius);
}

template<class T>
TSampleSelection<T>::~TSampleSelection()
{
    m_octree = NULL;
    m_iterator = NULL;
    m_nselected = 0;
    setRadius(0);
}

template<class T>
void TSampleSelection<T>::setRadius(double radius)
{
    m_radius = radius;
    m_sqradius = radius *radius;
    
    if(radius > 0)
      m_sigma = 0.5/m_radius;
}

template<class T>
double TSampleSelection<T>::getRadius()
{
  return m_radius;
}


template<class T>
unsigned int TSampleSelection<T>::getNSelected() const
{
  return m_nselected;
}


template<class T>
void TSampleSelection<T>::performSelection()
{
  std::cout<<"Selecting points with radius "<<getRadius()<<std::endl;
  TOctreeNode<T>* cell = m_octree->getRoot();
  performSelection(cell);
}

template<class T>
void TSampleSelection<T>::performSelection(TOctreeNode<T> *cell)
{
	if(cell->getDepth() > m_iterator->getDepth())
	{
		for(int i=0;i<8;i++)
			if(cell->getChild(i)!=NULL)
				performSelection(cell->getChild(i));
	}
	else
	{
		performSelection(cell,cell);
	}
}

template<class T>
void TSampleSelection<T>::performSelection(TOctreeNode< T >* cell, TOctreeNode< T >* par)
{
        if(cell->getDepth() > 0)
	{
		for(int i=0; i<8; ++i)
			if(cell->getChild(i)!=NULL)
				performSelection(cell->getChild(i),par);

	}
	else if(cell->getNpts()>0)
	{
		typename Sample_list::iterator si=cell->points_begin();
		while(si!=cell->points_end())
		{
			T &s = *si;
			if(s.isCovered() == false)
			{
				Sample_star_list neighbors;
				m_iterator->getNeighbors(s, par, neighbors);
				if(neighbors.size()<3)
        {
				  s.setSelected(false);
          std::cout<<"removed one point"<<std::endl;
        }
				else
				{
				  typename Sample_star_list::iterator ni = neighbors.begin();
				  while(ni != neighbors.end())
				  {
				      (*ni)->setCovered(true);
				      (*ni)->setSelected(false);
				      (*ni)->increaseNCovered();
				      ++ni;
				  }
				  m_nselected ++;
				  s.setSelected(true);
				}
			}
			++si;
		}
	}

}

template<class T>
void TSampleSelection<T>::performDartThrowingSelection()
{
    std::cout<<"Dart Throwing Selection in parallel"<<std::endl;
    typedef typename std::vector< std::vector<TOctreeNode<T>* > >
                                                 OctreeNode_collection;
    TOctreeNode<T> *root = m_octree->getRoot();
    unsigned int depth = m_iterator->getDepth();

    const double d = 2.1 * m_radius;
    depth = (unsigned int)(m_octree->getDepth() - floor( log2(
                m_octree->getSize() / (1.5 * d) )));

    std::cout<<"Processing depth "<< depth <<" ; size "
             << m_octree->getSize()/(double)pow2(m_octree->getDepth()-depth)
             <<" ; dilatation radius "<<d<<std::endl;

    OctreeNode_collection node_collection;
    m_octree->getNodes(depth, root, node_collection);
    
    for(unsigned int i = 0; i < 8; ++i)
    {
       std::vector<std::list<T*> > cell_selected_samples;
       cell_selected_samples.assign(node_collection[i].size(),std::list<T*>());
       
#ifdef OMP
       #pragma omp parallel for default(shared)
#endif 
       for(int j = 0; j < (int)node_collection[i].size(); ++j)
       {
           TOctreeNode<T> *node = node_collection[i][j];
           performDartThrowingSelection(node, cell_selected_samples[j]);
       }
       
       //merge
       for(int j = 0; j < (int)node_collection[i].size(); ++j)
       {
           m_selected_samples.splice(m_selected_samples.end(),
                                   cell_selected_samples[j]);
       }
       m_nselected = m_selected_samples.size();
    }
}


template<class T>
void TSampleSelection<T>::performDartThrowingSelection(TOctreeNode< T >* cell,
                                          std::list<T*> &cell_selected_samples)
{
  
  std::vector<TOctreeNode<T>* > leaves;
  std::set<T*> m_all_samples;

  m_octree->getNodes(0, cell, leaves);
  TOctreeIterator<T> iterator(m_octree);
  iterator.setR(m_radius);

  //get all points stored in the octree
  typename std::vector<TOctreeNode<T>* >::iterator it_node;
  for(it_node = leaves.begin(); it_node != leaves.end(); ++it_node)
  {
    typename std::list<T>::iterator pi;
    for(pi = (*it_node)->points_begin(); pi != (*it_node)->points_end(); ++pi)
    {
      if(!pi->isCovered())
        m_all_samples.insert(&(*pi));
    }
  }
  srand (time(NULL));
  unsigned int  nuncovered = m_all_samples.size();
  while(nuncovered > 0)
  {
    unsigned int idx = std::rand() % nuncovered;
    typename std::set<T*>::iterator it = m_all_samples.begin();
    for(unsigned int i = 0;i <idx; ++i)
      ++it;

    T *s = *it;
    m_all_samples.erase(it);

    if(s->isCovered())
      continue;


    Sample_star_list neighbors;
    iterator.getNeighbors(*s, neighbors);
    typename Sample_star_list::iterator ni = neighbors.begin();
    while(ni != neighbors.end())
    {
        T* sample = *ni;
        m_all_samples.erase(sample);
        sample->setCovered(true);
        sample->setSelected(false);
        sample->increaseNCovered();
        ++ni;
    }
    
    s->setSelected(true);
    cell_selected_samples.push_back(s);
    
    nuncovered = m_all_samples.size();
  }
}






#endif

