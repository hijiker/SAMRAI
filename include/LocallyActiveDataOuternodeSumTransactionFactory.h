//
// File:	LocallyActiveDataOuternodeSumTransactionFactory.h
// Package:	SAMRAI algorithms
// Copyright:	(c) 1997-2005 The Regents of the University of California
// Revision:	$Revision: 696 $
// Modified:	$Date: 2005-11-03 12:27:01 -0800 (Thu, 03 Nov 2005) $
// Description:	Concrete factory for creating outernode sum transaction objects
//              for locally-active data refine schedules.
//

#ifndef included_algs_LocallyActiveDataOuternodeSumTransactionFactory
#define included_algs_LocallyActiveDataOuternodeSumTransactionFactory

#ifndef included_SAMRAI_config
#include "SAMRAI_config.h"
#endif
#ifndef included_tbox_Arena
#include "tbox/Arena.h"
#endif
#ifndef included_tbox_Pointer
#include "tbox/Pointer.h"
#endif
#ifndef included_hier_Box
#include "Box.h"
#endif
#ifndef included_hier_BoxOverlap
#include "BoxOverlap.h"
#endif
#ifndef included_hier_LocallyActiveDataPatchLevelManager
#include "LocallyActiveDataPatchLevelManager.h"
#endif
#ifndef included_hier_PatchLevel
#include "PatchLevel.h"
#endif
#ifndef included_xfer_RefineClasses
#include "RefineClasses.h"
#endif
#ifndef included_xfer_LocallyActiveDataRefineTransactionFactory
#include "LocallyActiveDataRefineTransactionFactory.h"
#endif

#ifndef NULL
#define NULL (0)
#endif

namespace SAMRAI {
    namespace algs {

/*!
 * @brief Concrete subclass of the xfer::LocallyActiveDataRefineTransactionFactory<DIM> 
 * base class that allocates transaction outernode sum objects for a 
 * xfer::LocallyActiveDataRefineSchedule<DIM> object.
 *
 * @see xfer::LocallyActiveDataRefineTransactionFactory
 * @see xfer::OuternodeSumTransaction
 */

template<int DIM> 
class LocallyActiveDataOuternodeSumTransactionFactory 
: public xfer::LocallyActiveDataRefineTransactionFactory<DIM>
{
public:
   /*!
     @brief Default constructor.
   */
   LocallyActiveDataOuternodeSumTransactionFactory();

   /*!
     @brief Virtual destructor for base class.
   */
   virtual ~LocallyActiveDataOuternodeSumTransactionFactory();

   /*!
    * @brief Set the array of xfer::RefineClass<DIM>::Data items used by the transactions.
    */
   void setRefineItems(const typename xfer::RefineClasses<DIM>::Data** refine_items,
                       int num_refine_items);

   /*!
    * @brief Clear the array of xfer::RefineClass<DIM>::Data items used by the transactions.
    */
   void unsetRefineItems();

   /*!
    * @brief Allocate an OuternodeSumTransaction<DIM> object.  
    *
    * @param dst_level      tbox::Pointer to destination patch level.
    * @param src_level      tbox::Pointer to source patch level.
    * @param overlap        tbox::Pointer to overlap region between patches.
    * @param dst_patch      Integer index of destination patch in destination
    *                       patch level.
    * @param src_patch      Integer index of source patch in source patch level.
    * @param ritem_id       Integer index of xfer::RefineClass<DIM>::Data item 
    *                       associated with transaction.
    * @param box            Optional const reference to box defining region of
    *                       refine transaction.  Default is an empty box.
    * @param use_time_interpolation  Optional boolean flag indicating whether the
    *                       refine transaction involves time interpolation.
    *                       Default is false.
    * @param pool           Optional pointer to memory pool from which the
    *                       refine transaction may be allocated. Default is null.
    */
   tbox::Pointer<tbox::Transaction>
   allocate(tbox::Pointer< hier::PatchLevel<DIM> > dst_level,
            tbox::Pointer< hier::PatchLevel<DIM> > src_level,
            tbox::Pointer< hier::BoxOverlap<DIM> > overlap,
            int dst_patch_id,
            int src_patch_id,
            int ritem_id,
            const hier::Box<DIM>& box = hier::Box<DIM>(),
            bool use_time_interpolation = false,
            tbox::Pointer<tbox::Arena> pool = (tbox::Arena *) NULL) const;

   /*!
    * @brief Function to initialize scratch space data for the sum transactions
    * (patch data components indicated by the patch level manager) to zero.
    *
    * @param level        tbox::Pointer to patch level holding scratch data.
    * @param fill_time    Double value of simulation time at which preprocess
    *                     operation is called.
    * @param preprocess_mgr Const reference to hier::LocallyActiveDataPatchLevelManager<DIM>
    *                     object that indicates patch data array indices of which scratch
    *                     patch data objects on each patch to preprocess.
    */
   void preprocessScratchSpace(
      tbox::Pointer<hier::PatchLevel<DIM> > level,
      double fill_time,
      const hier::LocallyActiveDataPatchLevelManager<DIM>& preprocess_mgr) const;

private:
   // The following two functions are not implemented
   LocallyActiveDataOuternodeSumTransactionFactory(
      const LocallyActiveDataOuternodeSumTransactionFactory<DIM>&);
   void operator=(const LocallyActiveDataOuternodeSumTransactionFactory<DIM>&);

   const typename xfer::RefineClasses<DIM>::Data** d_refine_items;
   int d_number_refine_items;

};

}
}
#endif

#ifdef INCLUDE_TEMPLATE_IMPLEMENTATION
#include "LocallyActiveDataOuternodeSumTransactionFactory.C"
#endif
