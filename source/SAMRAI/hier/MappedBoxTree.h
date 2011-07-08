/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   Binary tree of MappedBoxes for overlap searches. 
 *
 ************************************************************************/

#ifndef included_hier_MappedBoxTree
#define included_hier_MappedBoxTree

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/hier/Box.h"
#include "SAMRAI/hier/MappedBox.h"
#include "SAMRAI/hier/MappedBoxSet.h"
#include "SAMRAI/tbox/DescribedClass.h"
#include "SAMRAI/tbox/Pointer.h"
#include "SAMRAI/tbox/Timer.h"

#include <vector>
#include <map>

namespace SAMRAI {
namespace hier {

class BoxList;

/*!
 * @brief Utility sorting MappedBoxes into tree-like form for finding
 * box overlaps.
 *
 * This class recursively splits a set of MappedBoxes into tree-like
 * form and stores them for fast searches.  The recursive
 * splitting stops when the number of boxes in a leaf node of the tree
 * is less than a minimum number specified in the constructor.
 *
 * All mapped boxes in a MappedBoxTree must exist in the same index space.
 * This means that the must all have the same BlockId value.
 *
 * Overlap searches are done by
 * - hasOverlap()
 * - findOverlapMappedBoxes()
 *
 * Information about the boxes in the tree are given by
 * - getBoundingBox()
 * - getMappedBoxes()
 */

class MappedBoxTree:public tbox::DescribedClass
{
public:
   /*!
    * @brief Constructor building an uninitialized object.
    *
    * The object can be initialized using generateTree().
    *
    * @param[in] dim
    */
   explicit MappedBoxTree(
      const tbox::Dimension& dim);

   /*!
    * @brief Constructs a MappedBoxTree from set of MappedBoxes.
    *
    * @param[in] dim
    *
    * @param[in] mapped_boxes.  No empty boxes are allowed.  An assertion
    *                           failure will occur if the mapped boxes in this
    *                           input set do not all have the same BlockId.
    *
    * @param[in] min_number Split up sets of boxes while the number of
    * boxes in a subset is greater than this value.  Setting to a
    * larger value tends to make tree building faster but tree
    * searching slower, and vice versa.  @b Default: 10
    */
   explicit MappedBoxTree(
      const tbox::Dimension& dim,
      const MappedBoxSet& mapped_boxes,
      size_t min_number = 10);

   /*!
    * @brief Constructs a MappedBoxTree from vector of MappedBoxes.
    *
    * See MappedBoxTree( const tbox::Dimension& , const MappedBoxSet& , size_t min_number );
    *
    * @param[in] dim
    *
    * @param[in] mapped_boxes.  No empty boxes are allowed.
    *
    * @param[in] min_number.  @b Default: 10
    */
   explicit MappedBoxTree(
      const tbox::Dimension& dim,
      const std::vector<MappedBox>& mapped_boxes,
      size_t min_number = 10);

   /*!
    * @brief Constructs a MappedBoxTree from a list of Boxes.
    *
    * See MappedBoxTree( const tbox::Dimension& , const MappedBoxSet& , size_t min_number );
    *
    * @param[in] dim
    *
    * @param[in] boxes  No empty boxes are allowed.
    *
    * @param[in] block_id The BlockId that will be assigned to every
    *                     MappedBox in the tree.
    *
    * @param[in] min_number  @b Default: 10
    */
   explicit MappedBoxTree(
      const tbox::Dimension& dim,
      const BoxList& boxes,
      const BlockId& block_id,
      size_t min_number = 10);

   /*!
    * @brief Destructor.
    */
   ~MappedBoxTree();

   /*!
    * @brief Generates the tree from a MUTABLE vector of MappedBoxes.
    *
    * For efficiency reasons, mapped_boxes is changed in the process.
    * Its output state is undefined.  However, you can change
    * mapped_boxes after tree generation without invalidating the
    * tree.
    *
    * @param[in] mapped_boxes.  No empty boxes are allowed.
    *
    * @param[in] min_number
    */
   void
   generateTree(
      std::vector<MappedBox>& mapped_boxes,
      size_t min_number = 10);

   /*!
    * @brief Reset to uninitialized state.
    *
    * The dimension of boxes in the tree cannot be changed.
    *
    * Uninitialized trees can be initialized using generateTree().
    */
   void
   clear();

   /*!
    * @brief Check whether the tree has been initialized.
    *
    * Uninitialized trees can be initialized using generateTree().
    */
   bool
   isInitialized() const;

   //@{

   //! @name Access to box data

   /*!
    * @brief Get the MappedBoxes in the tree.
    *
    * @param[out] mapped_boxes
    */
   void
   getMappedBoxes(
      std::vector<MappedBox>& mapped_boxes) const;

   /*!
    * @brief Return the bounding box of all the MappedBoxes in the
    * tree.
    */
   const Box&
   getBoundingBox() const;

   /*!
    * @brief Return the dimension of the boxes in the tree.
    */
   const tbox::Dimension&
   getDim() const;

   //@}


   //@{

   //! @name Overlap checks

   /*!
    * @brief Whether the given box has an overlap with MappedBoxes in the
    * tree.
    *
    * @param[in] box The box is assumed to be in same index space as
    * those in the tree.
    */
   bool
   hasOverlap(
      const Box& box) const;

   /*!
    * @brief Find all boxes that overlap the given \b box.
    *
    * To avoid unneeded work, the output @b overlap_mapped_boxes container
    * is not emptied.  Overlapping MappedBoxes are simply added.
    *
    * Output is sorted.
    *
    * @param[out] overlap_mapped_boxes MappedBoxes that overlap with box.
    *
    * @param[in] box the specified box whose overlaps are requested.
    * The box is assumed to be in same index space as those in the
    * tree.
    */
   void
   findOverlapMappedBoxes(
      MappedBoxSet& overlap_mapped_boxes,
      const Box& box) const;

   /*!
    * @brief Find all boxes that overlap the given \b box.
    *
    * To avoid unneeded work, the output @b overlap_mapped_boxes container
    * is not emptied.  Overlapping MappedBoxes are simply added.
    *
    * Output is unsorted.
    *
    * @param[out] overlap_mapped_boxes MappedBoxes that overlap with box.
    *
    * @param[in] box the specified box whose overlaps are requested.
    * The box is assumed to be in same index space as those in the
    * tree.
    */
   void
   findOverlapMappedBoxes(
      std::vector<MappedBox>& overlap_mapped_boxes,
      const Box& box) const;

   /*!
    * @brief Find all boxes that overlap the given \b box.
    *
    * To avoid unneeded work, the output @b overlap_mapped_boxes container
    * is not emptied.  Overlapping MappedBoxes are simply added.
    *
    * Output is unsorted.
    *
    * @param[out] overlap_mapped_boxes MappedBoxes that overlap with box.
    *
    * @param[in] box the specified box whose overlaps are requested.
    * The box is assumed to be in same index space as those in the
    * tree.
    */
   void
   findOverlapMappedBoxes(
      hier::BoxList & overlap_mapped_boxes,
      const Box& box) const;

   //@}

   /*!
    * @brief Create a similar tree with the boxes refined by a given
    * ratio.
    *
    * @param[in] ratio The boxes are refined by this ratio.
    *
    * Note that there is no coresponding version to create a coarsened
    * tree.  Coarsened trees cannot be trivially generated like
    * refined trees can.  To create a coarsened tree, you must
    * manually get the boxes, coarsen them and use them to build a new
    * tree.
    */
   tbox::Pointer<MappedBoxTree>
   createRefinedTree(
      const IntVector& ratio) const;

   /*!
    * @brief Get the BlockId.
    */
   const BlockId& getBlockId() const
   { return d_block_id; }

   /*!
    * @brief Assignment operator.
    *
    * @param[in] r
    */
   MappedBoxTree&
   operator = (
      const MappedBoxTree& r);

   /*!
    * @brief Print statistics on number of constructor calls, tree
    * builds, tree searches, etc.
    *
    * This method is for developers to analyze performance.
    */
   static void
   printStatistics(
      const tbox::Dimension& dim);

   /*!
    * @brief Reset statistics on number of constructor calls, tree
    * builds, tree searches, etc.
    *
    * This method is for developers to analyze performance.
    */
   static void
   resetStatistics(
      const tbox::Dimension& dim);

   /*!
    * @brief To satisfy requirements of STL vectors, the vector is
    * allowed to use this constructor.
    */
   friend class std::vector<MappedBoxTree>;

   /*!
    * @brief To satisfy requirements of STL maps, the map is
    * allowed to use this constructor.
    */
   friend class std::map<BlockId,MappedBoxTree>;

private:

   /*!
    * @brief Default constructor is private to disallow user access.
    * Objects are normally constructed with at least a dimension.
    */
   MappedBoxTree();

   /*!
    * @brief Private recursive function for generating the search tree.
    *
    * mapped_boxes is changed in the process (for efficiency reasons).
    * Its output state is undefined.
    *
    * The object is not cleared in this method.  If the object has
    * been initialized, it should be cleared before calling this
    * method.  @see clear().
    *
    * @param[in] mapped_boxes
    *
    * @param min_number.  @b Default: 10
    */
   void
   privateGenerateTree(
      std::vector<MappedBox>& mapped_boxes,
      size_t min_number = 10);

   /*!
    * @brief Set up the child branches.
    *
    * This method is called after splitting the MappedBoxes into the
    * left_mapped_boxes and right_mapped_boxes, with boxes straddling
    * the divider stored in d_mapped_boxes.  It generates
    * d_left_child, d_right_child and, if needed, d_center_child.
    *
    * @param[in] min_number
    *
    * @param[in,out] left_mapped_boxes
    *
    * @param[in,out] right_mapped_boxes
    */
   void setupChildren(
      const size_t min_number,
      std::vector<MappedBox> &left_mapped_boxes,
      std::vector<MappedBox> &right_mapped_boxes);

   /*!
    * @brief Check whether given box has any ovarlap with the tree.
    *
    * @param[in] box
    */
   bool
   privateHasOverlap(
      const Box& box) const;

   /*!
    * @brief Find all boxes that overlap the given \b box.
    *
    * Output is sorted.
    *
    * @param[out] overlap_boxes Output, mapped_boxes that overlap with box.
    *
    * @param[in] box the specified box whose overlaps are requested.
    */
   void
   privateFindOverlapMappedBoxes(
      MappedBoxSet& overlap_mapped_boxes,
      const Box& box) const;

   /*!
    * @brief Find all boxes that overlap the given \b box.
    *
    * Output is unsorted.
    *
    * @param[out] overlap_boxes Output, mapped_boxes that overlap with box.
    *
    * @param[in] box the specified box whose overlaps are requested.
    */
   void
   privateFindOverlapMappedBoxes(
      std::vector<MappedBox>& overlap_mapped_boxes,
      const Box& box) const;

   /*!
    * @brief Find all boxes that overlap the given \b box.
    *
    * Output is unsorted.
    *
    * @param[out] overlap_boxes Output, mapped_boxes that overlap with box.
    *
    * @param[in] box the specified box whose overlaps are requested.
    */
   void
   privateFindOverlapMappedBoxes(
      hier::BoxList& overlap_mapped_boxes,
      const Box& box) const;


   /*!
    * @brief Set up static class members.
    *
    * Only called by StartupShutdownManager.
    */
   static void
   initializeCallback();

   /*!
    * @brief Free static timers.
    *
    * Only called by StartupShutdownManager.
    */
   static void
   finalizeCallback();

   /*!
    * @brief Dimension corresponds to the dimension of boxes in the
    * tree.
    */
   const tbox::Dimension d_dim;

   /*!
    * @brief Bounding box of all the MappedBoxes in this tree.
    */
   Box d_bounding_box;

   /*!
    * @brief BlockId
    */
   BlockId d_block_id;

   /*!
    * Pointers to familial mapped_boxes.
    */
   tbox::Pointer<MappedBoxTree> d_left_child;
   tbox::Pointer<MappedBoxTree> d_right_child;

   /*!
    * @brief A tree for MappedBoxes that are not given to the left or
    * right children.
    */
   tbox::Pointer<MappedBoxTree> d_center_child;

   /*!
    * @brief MappedBoxes that are contained within the physical domain
    * that this tree represents.  When we have a small number of boxes
    * that do not warant the overhead of a child tree, the boxes go here.
    */
   std::vector<MappedBox> d_mapped_boxes;

   /*!
    * @brief Dimension along which the input box triples are
    * partitioned.
    */
   int d_partition_dim;

   /*
    * Timers are static to keep the objects light-weight.
    */
   static tbox::Pointer<tbox::Timer> t_build_tree[tbox::Dimension::
                                                  MAXIMUM_DIMENSION_VALUE];
   static tbox::Pointer<tbox::Timer> t_search[tbox::Dimension::
                                              MAXIMUM_DIMENSION_VALUE];

   static unsigned int s_num_build[tbox::Dimension::MAXIMUM_DIMENSION_VALUE];
   static unsigned int s_num_generate[tbox::Dimension::MAXIMUM_DIMENSION_VALUE];
   static unsigned int s_num_duplicate[tbox::Dimension::MAXIMUM_DIMENSION_VALUE];
   static unsigned int s_num_search[tbox::Dimension::MAXIMUM_DIMENSION_VALUE];
   static unsigned int s_num_sorted_box[tbox::Dimension::MAXIMUM_DIMENSION_VALUE];
   static unsigned int s_num_found_box[tbox::Dimension::MAXIMUM_DIMENSION_VALUE];
   static unsigned int s_max_sorted_box[tbox::Dimension::MAXIMUM_DIMENSION_VALUE];
   static unsigned int s_max_found_box[tbox::Dimension::MAXIMUM_DIMENSION_VALUE];
   static unsigned int s_max_lin_search[tbox::Dimension::MAXIMUM_DIMENSION_VALUE];

   static tbox::StartupShutdownManager::Handler
   s_initialize_finalize_handler;

};

}
}

#endif
