//
// File:	LocallyActiveDataFillBoxSet.h
// Package:	SAMRAI transfer
// Copyright:	(c) 1997-2005 The Regents of the University of California
// Revision:	$Revision: 481 $
// Modified:	$Date: 2005-07-21 13:50:43 -0700 (Thu, 21 Jul 2005) $
// Description:	Routines for "smart" boxlist ops in locally-active comm schedules
//

#ifndef included_xfer_LocallyActiveDataFillBoxSet
#define included_xfer_LocallyActiveDataFillBoxSet

#ifndef included_SAMRAI_config
#include "SAMRAI_config.h"
#endif
#ifndef included_hier_Box
#include "Box.h"
#endif
#ifndef included_hier_BoxList
#include "BoxList.h"
#endif
#ifndef included_tbox_List
#include "tbox/List.h"
#endif
#ifndef included_tbox_PIO
#include "tbox/PIO.h"
#endif
#ifndef included_xfer_FillBoxSet
#include "FillBoxSet.h"
#endif
#ifndef included_xfer_LocallyActiveDataFillBox
#include "LocallyActiveDataFillBox.h"
#endif
#ifndef included_xfer_CoarsenClasses
#include "CoarsenClasses.h"
#endif
#ifndef included_xfer_RefineClasses
#include "RefineClasses.h"
#endif
#ifndef included_iostream
#include <iostream>
using namespace std;
#define included_iostream
#endif

namespace SAMRAI {
   namespace xfer {

/**
 * Class LocallyActiveDataFillBoxSet is a utility class that provides "smart" 
 * boxlist operations in communication schedules that operate on "locally-active" 
 * data; i.e., where each data item may live on a different set of patches.  This 
 * class is derived from the FillBoxSet class and extends the functionality of that 
 * base class for locally-active patch data.   Specifically, this box maintains
 * a colleciton of LocallyActiveDataFillBox objects, each of which contains a box and
 * an associated list of either CoarseClass or RefineClass items, but not both.
 *
 * @see xfer::FillBoxSet
 * @see xfer::LocallyActiveDataFillBox
 */
template<int DIM> 
class LocallyActiveDataFillBoxSet : public FillBoxSet<DIM>
{
public:
   /*!
    * @brief Construct a new locally-active fill box set and copy the information 
    * from the argument locally-active fill box set.
    * 
    * @param fill_box_set  Constant reference to fill box set to be copied.
    */
   LocallyActiveDataFillBoxSet(
      const LocallyActiveDataFillBoxSet<DIM>& fill_box_set);

   /*!
    * @brief Default constructor creates a new fill box set with an empty 
    * box set and active patch data information initialized to an unusable state.  
    * The box and the active patch must be set by calling resetLocallyActiveFillBoxes() 
    * or addLocallyActiveFillBox() functions on the constructed object.
    */
   LocallyActiveDataFillBoxSet();

   /*!
    * The destructor releases all box and locally-active data storage.
    */
   virtual ~LocallyActiveDataFillBoxSet<DIM>();

   /*!
    * Clears all existing box and locally-active data information for calling object 
    * and sets it to the state of the argument fill box set.
    *
    * Note that this is essentially the same as an assignment opertor, but this
    * implementation was chosen to avoid warnings with some compilers.
    */
   void setTo(const LocallyActiveDataFillBoxSet<DIM>& fill_box_set);

   /*!
    * Return number of boxes maintained by this locally-active fill box set.
    */
   int getNumberOfBoxes() const;

   /*!
    * Return a const reference to the list of locally-active fill boxes owned 
    * by this object.
    */
   const tbox::List< xfer::LocallyActiveDataFillBox<DIM> >& getLocallyActiveDataBoxes() const;

   /*!
    * Return const reference to non-redundant list of refine items representing the
    * union of all locally-active fill boxes owned by this object.
    * 
    * Note that if this object manages coarsen item data, an unrecoverable
    * assertion will result.
    */
   const tbox::List<const typename xfer::RefineClasses<DIM>::Data*>&
      getUnionActiveRefineVarData() const;

   /*!
    * Return const reference to non-redundant list of coarsen items representing the
    * union of all locally-active fill boxes owned by this object.
    *
    * Note that if this object manages refine item data, an unrecoverable
    * assertion will result.
    */
   const tbox::List<const typename xfer::CoarsenClasses<DIM>::Data*>&
      getUnionActiveCoarsenVarData() const;
  
   /*!
    * Set box and refine item information for this locally-active fill box set to
    * given arguments.
    * 
    * @param box       Input box.
    * @param var_data  Input list of refine class data pointers.
    *
    * Note that if this object currently manages coarsen item data, an unrecoverable
    * assertion will result.
    */
   void resetLocallyActiveFillBoxes(
      const hier::Box<DIM>& box,
      const tbox::List<const typename xfer::RefineClasses<DIM>::Data*>& var_data);

   /*!
    * Set box and refine item information for this locally-active fill box set to
    * given arguments.
    *
    * @param box       Input box.
    * @param var_data  Input list of coarsen class data pointers.
    *
    * Note that if this object currently manages coarsen item data, an unrecoverable
    * assertion will result.
    */
   void resetLocallyActiveFillBoxes(
      const hier::Box<DIM>& box,
      const tbox::List<const typename xfer::CoarsenClasses<DIM>::Data*>& var_data);

   /*!
    * Add box and refine item information to this locally-active fill box set.
    *
    * @param box       Input box.
    * @param var_data  Input list of refine class data pointers.
    *
    * Note that if this object currently manages coarsen item data, an unrecoverable
    * assertion will result.
    */
   void addLocallyActiveFillBox(
      const hier::Box<DIM>& box,
      const tbox::List<const typename xfer::RefineClasses<DIM>::Data*>& var_data);

   /*!
    * Add box and coarsen item information to this locally-active fill box set.
    *
    * @param box       Input box.
    * @param var_data  Input list of coarsen class data pointers.
    *
    * Note that if this object currently manages refine item data, an unrecoverable
    * assertion will result.
    */
   void addLocallyActiveFillBox(
      const hier::Box<DIM>& box,
      const tbox::List<const typename xfer::CoarsenClasses<DIM>::Data*>& var_data);

   /*!
    * Replace each box in the fill box set with its intersection with the argument box.
    * Empty fill boxes will be removed. 
    */
   void intersectBoxes(const hier::Box<DIM>& box);

   /*!
    * Replace each box in the fill box set with its intersection with the 
    * argument boxlist.  Empty fill boxes will be removed.
    */
   void intersectBoxes(const hier::BoxList<DIM>& boxes);

   /*!
    * Print all class member data for this locally-active fill box set object
    * to specified output stream  (default is plog).
    */
   void printClassData(ostream& os = tbox::plog) const;

private:
   /*
    * Private utility function to merge two lists of refine items sorted in increasing
    * order of their integer tag fields into a sorted, non-redundant list containing
    * all items in both lists.
    */
   void mergeLists(
      tbox::List<const typename xfer::RefineClasses<DIM>::Data*>& outlist,
      const tbox::List<const typename xfer::RefineClasses<DIM>::Data*>& inlist_a,
      const tbox::List<const typename xfer::RefineClasses<DIM>::Data*>& inlist_b) const;

   /*
    * Private utility function to merge two lists of coarsen items sorted in increasing
    * order of their integer tag fields into a sorted, non-redundant list containing
    * all items in both lists.
    */
   void mergeLists(
      tbox::List<const typename xfer::CoarsenClasses<DIM>::Data*>& outlist,
      const tbox::List<const typename xfer::CoarsenClasses<DIM>::Data*>& inlist_a,
      const tbox::List<const typename xfer::CoarsenClasses<DIM>::Data*>& inlist_b) const;

   /*
    * Private utility function to clear all locally-active fill box information.
    */
   void clearLocallyActiveFillBoxData();

   /*
    * Private utility functions to check unions and print lists of refine items.
    */
   bool checkUnion(ostream& os = tbox::plog) const;
   void printRefineVarListItems(
      const tbox::List<const typename xfer::RefineClasses<DIM>::Data*>& inlist, 
      ostream& os = tbox::plog) const;

   tbox::List< xfer::LocallyActiveDataFillBox<DIM> > d_locally_active_boxes;
   
   tbox::List<const typename xfer::RefineClasses<DIM>::Data*> d_union_refine_var_data;
   tbox::List<const typename xfer::CoarsenClasses<DIM>::Data*> d_union_coarsen_var_data;

   bool d_refine_data;
};

}
}

#endif

#ifdef INCLUDE_TEMPLATE_IMPLEMENTATION
#include "LocallyActiveDataFillBoxSet.C"
#endif