#ifndef __Node
#define __Node

#include <vector>
#include <algorithm>
#include "GenVector.h"
#include "Ray.h"
#include "Shape.h"
#include "Hitpoint.h"
#include "AABB.h"
#define MAXI 99999999
struct TreeNode {
	AABB bb;
	TreeNode *left;
	TreeNode *right;

	TreeNode(AABB b) {
		bb = b;
		left = NULL;
		right = NULL;
	}
};

	float traverseSceneTree( Ray r )
	{/*
		int didChildHit = 0;
		if(!null){
			if(bb.list.size()>0){
				float rootParam = bb.intersect(r);
				bool didItHit =  rootParam >= 0 && rootParam < MAXI;
				if( didItHit )
				{
					if(left->root->null && right->root->null)
						return bb.list[0]->intersect(r);
					if(!left->root->null){
						float leftParam = left->traverseSceneTree(r);
						didChildHit =  leftParam >= 0 && leftParam < MAXI;
						if( didChildHit )
							return leftParam;
					}
					if(!right->root->null){
						float rightParam = right->traverseSceneTree(r);
						didChildHit =  rightParam >= 0 && rightParam < MAXI;
						if( didChildHit )
							return rightParam;
					}

				}
			}
		}*/
		return -1;

	}

#endif
