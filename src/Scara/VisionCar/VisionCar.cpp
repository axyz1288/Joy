#include "VisionCar.h"

VisionCar *VisionCar::inst_ = nullptr;

/**
 * Alternatively new scara arm object
 * To ensure we only have one scara arm object alive at a time
 * @retval inst_ - pointer of VisionCar object
 */
VisionCar *VisionCar::getVisionCar()
{
	if (inst_ == nullptr)
		inst_ = new VisionCar();
	return inst_;
}

/**
 * Vision Car constructor
 */
VisionCar::VisionCar()
	: MotorUnion({4, 5, 6}, {"Pro200", "Pro20", "Pro20"}),
	  FIRST_MOTOR_ID(0)
{
	Start();
	SetMotor_Operating_Mode(FIRST_MOTOR_ID  , 3);	    // Car angle control motor set to position mode
	SetMotor_Operating_Mode(FIRST_MOTOR_ID+1, 1);	    // Screw control motor set to velocity mode
	SetMotor_Operating_Mode(FIRST_MOTOR_ID+2, 3);	    // Cam angle control motor set to position mode
	cout << "\t\tClass constructed: VisionCar" << endl;
}

/**
 * Enable all motors, and set up the speed
 */
void VisionCar::Start()
{
	// Car angle motor
	SetMotor_Velocity(FIRST_MOTOR_ID, 100);
	SetMotor_Accel(FIRST_MOTOR_ID, 50);

	// Screw motor
	SetMotor_Accel(FIRST_MOTOR_ID + 1, 50);

	// Cam angle motor
	SetMotor_Velocity(FIRST_MOTOR_ID + 2, 200);
	SetMotor_Accel(FIRST_MOTOR_ID + 2, 100);

	SetAllMotorsTorqueEnable(true);
}

/**
 * Disable all motors
 */
void VisionCar::Stop()
{
	SetAllMotorsTorqueEnable(false);
}

/**
 * Move car, camera and camera height to desired position.
 * Before moving the car, the camera will be moved inside and the screw will be moved down for sake of safety
 * @param oz - int, vision car angle
 * @param h  - int, screw high (1) or low (0), pos_state enum is better
 * @param oc - int, camera outside (1) or inside (0), pos_state enum is better
 */
void VisionCar::GotoPosition(const int &oz, const int &h,  const int &oc)
{

	// Move camera inside if camera is now outside or undefined (undefined mean initial)
	if(current_cam_io != VisionCar::INSIDE){
		GoCameraIO(VisionCar::INSIDE);
		WaitMotorArrival(2);
	}

	// Screw down if screw is now upside or undefined (undefined mean initial)
	if (current_screw_io != VisionCar::DOWN){
		GoScrewHeight(VisionCar::DOWN);
	}

	// Set car angle
	GoCarAngle(oz);
	WaitMotorArrival(0);

	// Screw up or down
	if (current_screw_io != h){
		GoScrewHeight(h);
	}

	// Camera out or in
	if(current_cam_io != oc){
		GoCameraIO(oc);
		WaitMotorArrival(2);
	}
}

/**
 * Move vision car to goal angle
 * ##  No checking if already in position or not
 * @param goal_angle - float, target angle to go
 */
void VisionCar::GoCarAngle(const int &goal_angle)
{
	SetMotor_Angle(FIRST_MOTOR_ID, float(goal_angle));
}

/**
 * Move camera to desired height
 * @param dir - move screw up or down
 * @retval - bool, current screw position
 */
int VisionCar::GoScrewHeight(const int &dir)
{
	/*Moving*/

	printf("Not yet\n");
	current_screw_io = dir;
	return current_screw_io;
}

/**
 * Move camera out or in
 * ##  No checking if already in position or not
 * @param IO - bool, inside (true), outside (false)
 * @retval current_cam_io - int, current camera position
 */
int VisionCar::GoCameraIO(const int &IO)
{
	if (IO == VisionCar::INSIDE){
		SetMotor_Angle(FIRST_MOTOR_ID+2, CamInDegree);
	}
	else{
		SetMotor_Angle(FIRST_MOTOR_ID+2, CamOutDegree);
	}
	current_cam_io = IO;
	return current_cam_io;
}

/**
 * Get the Camera position data
 * @retval - bool, inside (true), outside (false)
 */
int VisionCar::GetCamPos()
{
	return current_cam_io;
}

/**
 * Get the Screw position data
 * @retval - bool, up (true), down (false)
 */
int VisionCar::GetScrewPos()
{
	return current_screw_io;
}

/**
 * Move scara arm to initial point
 * Camera in, Screw down and car to 0.
 */
void VisionCar::Reset()
{
	// camera in
	GoCameraIO(VisionCar::INSIDE);
	WaitMotorArrival(2);
	// // Screw down
	// GoScrewHeight(VisionCar::DOWN);
	// WaitAllMotorsArrival();
	// Car to 0 degree
	GoCarAngle(0);
	WaitMotorArrival(0);
}
