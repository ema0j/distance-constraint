#include "Angel.h"
#include "mat.h"
#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXd;
using namespace std;

enum constraint
{
	DISTANCE_FIXED,
	DISTANCE,
	FIXED_ROTATION,
	JOINT
};

void applyConstraint(vec4 dst, vec4 vd, vec4 pos, vec4 vp,
					vec4* post_dst, vec4* vd2, vec4* post_pos, vec4* vp2);

//class system  
//{
//	int nbody;				// n, the number of rigid bodies (except fixed point)
//	int nground;			// m, the number of fixed point(ground)
//	int ncons;				// s, the number of constraints on system
//	float dt;				// delta time
//	float beta;				// bias to adjust
//
//	MatrixXd x;				// 6n*1 matrix, set of position of rigid bodies on system
//	MatrixXd v;				// 6n*1 matrix, set of velocity of rigid bodies on system
//	MatrixXd z;				// 6m*1 matrix, set of position of fixed points on system
//
//	MatrixXd Jsp;			// s*2 matrix, index part of Jacobian matrix
//	MatrixXd Jmap;			// s*12 matrix, constraint part of Jacobian matrix
//	MatrixXd M;				// 6n*6n matrix, mass of rigid bodies					
//	MatrixXd lambda;		// s*1 matrix, lambda 
//
//	vec3* cons_set;			// set of constraints defined on system
//};
