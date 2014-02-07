#ifndef __Tree
#define __Tree

#include <vector>
#include <algorithm>
#include "GenVector.h"
#include "Ray.h"
#include "Shape.h"
#include "Hitpoint.h"
#include "AABB.h"
#define MAXI 9999999999999999
int longestDim;

int count;
struct TreeNode {
	AABB bb;
	TreeNode *left;
	TreeNode *right;

	TreeNode(AABB b) {
		bb = b;
		left = NULL;
		right = NULL;
	}
	TreeNode(){
		left = NULL;
		right = NULL;
	}
};
AABB makeBB( std::vector<Shape*> list);
void insert( struct TreeNode *&root, AABB bb );
int findMax( AABB bb );
bool compShapeElm0( Shape* a, Shape* b );
bool compShapeElm1( Shape* a, Shape* b );
bool compShapeElm2( Shape* a, Shape* b );
struct TreeNode *constructTree( std::vector<Shape*> list )
	{

		struct TreeNode *root;
		root = NULL;
		AABB currBB = makeBB(list);

		insert( root, currBB );
		if( list.size()> 1 ){

			longestDim = findMax(currBB);
			if( longestDim == 0 )
				std::sort(list.begin(), list.end(), compShapeElm0);
			else if( longestDim == 1 )
				std::sort(list.begin(), list.end(), compShapeElm1);
			else if( longestDim == 2 )
				std::sort(list.begin(), list.end(), compShapeElm2);

			std::size_t const middle = list.size() / 2;
			std::vector<Shape*> half1(list.begin(), list.begin()+middle);
			std::vector<Shape*> half2(list.begin()+middle, list.end());

			root->left  = constructTree(half1);
			root->right = constructTree(half2);
		}
		else if( list.size() < 1 ){
			root->left=NULL;
			root->right=NULL;
		}
		count = 0;
		return root;

	}
	AABB makeBB( std::vector<Shape*> list)
	{
		AABB* currBox = (AABB*) list [ 0 ]->getBB();
		int minX = currBox->bbMin[0];
		int maxX = currBox->bbMax[0];
		int minY = currBox->bbMin[1];
		int maxY = currBox->bbMax[1];
		int minZ = currBox->bbMin[2];
		int maxZ = currBox->bbMax[2];

		for( int i = 1; i < list.size(); i++ )
		{
			Shape* currShape =  list[ i ]->getBB();
			AABB* currBox = (AABB*) currShape;

			if( currBox->bbMin[0] < minX ){
				minX = currBox->bbMin[0];
			}
			if( currBox->bbMax[0] > maxX ){
				maxX = currBox->bbMax[0];
			}
			if( currBox->bbMin[1] < minY ){
				minY = currBox->bbMin[1];
			}
			if( currBox->bbMax[1] > maxY ){
				maxY = currBox->bbMax[1];
			}
			if( currBox->bbMin[2] < minZ ){
				minZ = currBox->bbMin[2];
			}
			if( currBox->bbMax[2] > maxZ ){
				maxZ = currBox->bbMax[2];
			}


		}
		int mid = currBox->matid;
		return AABB(Vector3(minX, minY, minZ), Vector3(maxX, maxY, maxZ), mid, list);
	}
	
	bool compShapeElm0( Shape* a, Shape* b ){ return ((AABB*)a->getBB())->bbMin[0] < ((AABB*)b->getBB())->bbMin[0]; }
	bool compShapeElm1( Shape* a, Shape* b ){ return ((AABB*)a->getBB())->bbMin[1] < ((AABB*)b->getBB())->bbMin[1]; }
	bool compShapeElm2( Shape* a, Shape* b ){ return ((AABB*)a->getBB())->bbMin[2] < ((AABB*)b->getBB())->bbMin[2]; }


	int findMax( AABB bb )
	{
		if(longestDim < 0){
			float xLen = bb.bbMax[0] - bb.bbMin[0];
			float yLen = bb.bbMax[1] - bb.bbMin[1];
			float zLen = bb.bbMax[2] - bb.bbMin[2];

			float lengestDim = std::max( std::max(xLen, yLen), zLen );

			if( longestDim == xLen )
				return 0;
			else if( longestDim == yLen )
				return 1;
			else if( longestDim == zLen )
				return 2;
			return 0;
		}
		return longestDim;

	}

	void insert( TreeNode *&root, AABB bb )
	{

		if( root == NULL )
		{

			root = new TreeNode( bb );

			return;
		}
		else if( bb.bbMin[longestDim] < (root->bb.bbMax[longestDim] - root->bb.bbMin[longestDim])/2)
		{

			insert( root->left, bb );

		}
		else
		{

			insert( root->right, bb);

		}


	}
		
	float traverseSceneTree( TreeNode *&root, Ray r )
	{
		count++;
		if(count == 22500){
			printf("bleh");
		}
		if(root != NULL){
			bool didLeftHit = false;
			bool didRightHit = false;
			float leftParam = MAXI;
			float rightParam = MAXI;
			if(root->bb.list.size()>0){
				float rootParam = root->bb.intersect(r);
				bool didItHit =  rootParam !=-1;
				//printf("%.2f",rootParam);
				if( didItHit )
				{
					if(root->left==NULL&&root->right==NULL)
					{
						printf("leaf");
						r.mid = root->bb.list[0]->getMatid();
						r.norm = root->bb.list[0]->getNormal(Vector3(0,0,0));
						return root->bb.list[0]->intersect(r);
					}
					if(root->left!=NULL)
					{
						//printf("Left");
						float leftParam = traverseSceneTree( root->left, r );
						didLeftHit =  leftParam >= 0;// && leftParam < MAXI;
					}
					if(root->right!=NULL)
					{
						//printf("right");
						float rightParam = traverseSceneTree(root->right, r);
						didRightHit =  rightParam >= 0;// && rightParam < MAXI;
					}
					if( leftParam > rightParam )
					{
						if(didRightHit)
						{
							r.mid = root->bb.list[0]->getMatid();
							r.norm = root->bb.list[0]->getNormal(Vector3(0,0,0));
							printf("jjjjjjjjjjjjjjjjjjjjjjjjj");
							return rightParam;
						}
					}
					if(didLeftHit)
					{
						r.mid = root->bb.list[0]->getMatid();
						r.norm = root->bb.list[0]->getNormal(Vector3(0,0,0));
						//printf("0000000000000000000000000");
						return leftParam;
					}

				}
			}

		return -1;

		}
	}
	
#endif
