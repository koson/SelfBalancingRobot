#include "application_pid.h"
#include "tm_stm32_usart.h"
#include "arm_math.h"




#define PID_PARAM_KP		12//10//2.5/2  	/* Proportional */
#define PID_PARAM_KI		0.01//1.2/2		/* Integral */
#define PID_PARAM_KD		0.8	/* Derivative */


#define PID_PARAM_KP2		0.08*1000  	/* Proportional */
#define PID_PARAM_KI2		0.000015*1000		/* Integral */
#define PID_PARAM_KD2		1.22*1000
	/* Derivative */

#define MINANGLE 10000
double positionError, angleError;
double positionOutput, position1, position2, position3;
int32_t force, forceL1, forceL2, forceL3, forceL4, forceR, forceL;
int32_t force, forceR1, forceR2, forceR3, forceR4;
arm_pid_instance_f32 anglePID;
arm_pid_instance_f32 positionPID;
double scaleFactor, scaleFactor2, scaleFactor3;
uint16_t positionCounter = 0;
int32_t angleAdjust;
double averagePosition = 0;
double angle1, angle2, angle3, angle4, angle5, angle6, setpointAngle;
void application_pid(int32_t angle){
   
    
    
    
    angle2 = angle1;
    angle1 = angle;
    angle = (angle1+angle2)/2;
    angle-=getZeroAngle();
    setpointAngle = angle - (positionOutput*scaleFactor/100);

    
    
    angleError = -setpointAngle;// + positionOutput;    
            
    //positionError = stepperCurrentPosition();
    
    averagePosition = (stepperCurrentPositionR()+stepperCurrentPositionL())/2;
    force = -arm_pid_f32(&anglePID, angleError);
    if(positionCounter >= 4){
        position3 = position2;
        position2 = position1;
        position1 = averagePosition;
        positionError = (position1+position2+position3)/3;
        positionOutput = (-arm_pid_f32(&positionPID, positionError)/1000);
        positionCounter=0;
        setStepperCurrentPositionR(stepperCurrentPositionR()+10);
        setStepperCurrentPositionL(stepperCurrentPositionR()-10);
    }
    positionCounter++;
    
    if((positionOutput>200 || positionOutput<-200)||(force > 10000||force <-10000))resetPID();
    
    
    scaleFactor = 1;
    if(averagePosition>0) scaleFactor = (abs(200 + force)/2);
    if(averagePosition<0) scaleFactor = (abs(200 - force)/2);
    if(scaleFactor>150)scaleFactor = 150;
    scaleFactor3 = scaleFactor2;
    scaleFactor2 = scaleFactor;
    
    scaleFactor = (scaleFactor2+scaleFactor3)/2;
    

    //motor output
    forceL = 0;
    forceR = 0;
    if(stepperCurrentPositionR()<stepperCurrentPositionL())forceR=500;
    if(stepperCurrentPositionR()>stepperCurrentPositionL())forceL=500;
    
    
    forceR4=forceR3;
    forceR3=forceR2;
    forceR2=forceR1;
    forceR1=force+forceR;
    forceL4=forceL3;
    forceL3=forceL2;
    forceL2=forceL1;
    forceL1=force+forceL;
    forceR = (forceR2+forceR1+forceR3)/3;
    forceL = (forceL2+forceL1+forceL3)/3;
    
    
    setStepperSpeedR((float)(forceR));
    setStepperSpeedL((float)(forceL));
    
    writeLog("setpointAngle", setpointAngle);
    writeLog("Angle:",          angle);
    writeLog("CurrentPosition:",positionError);
    writeLog("PosOutput",       positionOutput);
    writeLog("AngleError",      angleError);
    writeLog("Speed",           force);
    writeLog("scaleFactor", scaleFactor);
    
}

void resetPID(void){
    arm_pid_reset_f32(&anglePID);
    arm_pid_reset_f32(&positionPID);
    positionOutput = 0;
    setStepperCurrentPosition(0);
    
}


void init_pid(void){
    anglePID.Kp = PID_PARAM_KP;        /* Proporcional */
    anglePID.Ki = PID_PARAM_KI;        /* Integral */
    anglePID.Kd = PID_PARAM_KD;        /* Derivative */
    arm_pid_init_f32(&anglePID, 1);
    
    
    //pid2
    positionPID.Kp = PID_PARAM_KP2;        /* Proporcional */
    positionPID.Ki = PID_PARAM_KI2;        /* Integral */
    positionPID.Kd = PID_PARAM_KD2;        /* Derivative */
    arm_pid_init_f32(&positionPID, 1);
    
}
