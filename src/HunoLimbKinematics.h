#ifndef HUNOLIMBKINEMATICS_H
#define HUNOLIMBKINEMATICS_H

#include "ros/ros.h"
#include "geometry_msgs/Pose.h"
#include "huno_globals.h"

#include <string>
#include <Eigen/Dense>

/********************************************************************
* Huno Limb Kinematics Class.
* 
* author: E. Sasagawa
*
* description:
*   Declare limb class that will be used for all limbs.
*   Contains kinematic functions.
*   
* conventions:
*   M : Mathematical Units (radians, meters/sec, etc.)
*   C : Converted Units (degrees, mm/sec, etc.)
********************************************************************/

class C_HunoLimbKinematics
{
private:
  /* MEMBER VARIABLES */
  /* Joint type */
  const hunolimb_t m_limbType;

  /* Number of joints in this limb */
  const int m_numJoints;

  /* Offset angles for each joint (radians) */
  Eigen::Matrix<double, Eigen::Dynamic, 1> m_thetasRef_M;

  /* Joint twist axes */
  Eigen::Matrix<double, 3, Eigen::Dynamic> m_omegas_M;

  /* 3D points on each twist axis */
  Eigen::Matrix<double, 3, Eigen::Dynamic> m_qs_M;

  /* Twist vectors for each joint */
  Eigen::Matrix<double, 3, Eigen::Dynamic> m_zetas_M;

  /* Reference configuration matrix for this limb */
  Eigen::Matrix4d m_g0Mat_M;

  /* Lock boolean on the Jacobian matrix */
  bool m_isJacobianLocked;

  /* Jacobian matrix for this limb */
  Eigen::MatrixXd m_jacobian_M;

  /* Max angular velocity limits for each joint */
  Eigen::VectorXd m_jointVelLimits_M;

  /* PRIVATE FUNCTIONS */
  /* Calculate screw matrix due to rotation */
  Eigen::Matrix4d ExpXihatTheta(int jointIdx, double &theta_M);

  /* Generate 6x6 adjoint matrix */
  Eigen::Matrix6d AdjointMatrix(const Eigen::Ref<const Eigen::Matrix4d> &expXihatTheta);

  /* Check joint velocities for limits and directions */
  void BoundJointVels( Eigen::Ref<Eigen::VectorXd> &thetaDots_M );

public:
  /* Constructor */
  C_HunoLimbKinematics(hunolimb_t limb, int numJoints, const std::string param_namespace);

  /* Forward kinematics and Jacobian generation*/
  geometry_msgs::Pose ForwardKinematics(const double *thetas_C);

  /* Inverse kinematics */
  bool InverseKinematics(const double *p_inEEVel_C, double *p_outThetasDot_C);
}; // end class HunoLimbKinematics

#endif
