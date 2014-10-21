         /* ------------------------------------------------ 

          * Display Binary Search Tree HW3
          
          * Class: CS 251, Fall 2014.

          * System: Ubuntu 12.04 compiled with g++

          * Author: Basheer Subei

          * This program displays a binary tree (including edges) in ASCII.
          * A max screen size of 80 columns wide and 50 rows high is assumed.
          * Elements in the tree are assumed to be limited to 4-digit width.
          *
          * Instructions can be found at:
          * https://sites.google.com/site/uiccs251/programming-assignments/prog-3-display-tree

          * -------------------------------------------------

          */



/* General procedure taken from Mike Dunlavey on Stackoverflow
 * http://stackoverflow.com/questions/2242097/how-to-print-out-a-bst-in-c
 *
 * Here's approximate pseudo-code to do it. The basic idea is walk the tree layer-by-layer,
 * printing all the node in each layer on one line. Each node is separated by twice as much
 * space as the nodes below it. Since the tree is not all of uniform depth, it is artificially
 * padded with virtual nodes to take up the blank spaces where nodes don't exist.
 *
 * Psueducode:
     measure the depth of the tree, call that D
     have two queues, called Q1 and Q2
     enque the top node of the tree in Q1
     for (i = D; --i>=0; ){
       foreach node in Q1 {

         on first iteration of this inner loop, print 2^i - 1 spaces,
         else print 2^(i+1) - 1 spaces.

         if node == null print blank
         else print node.value

         if node.left exists enque node.left in Q2
         else enque null in Q2

         if node.right exists enque node.right in Q2
         else enque null in Q2
       }
       copy Q2 to Q1
       clear Q2
       print end-of-line
     }
 *
 */