package xreal.server.game;

import javax.vecmath.AxisAngle4f;
import javax.vecmath.Quat4f;
import javax.vecmath.Vector3f;
import javax.vecmath.Vector4f;

import com.bulletphysics.collision.shapes.BoxShape;
import com.bulletphysics.collision.shapes.CollisionShape;
import com.bulletphysics.dynamics.RigidBody;
import com.bulletphysics.dynamics.RigidBodyConstructionInfo;
import com.bulletphysics.linearmath.DefaultMotionState;
import com.bulletphysics.linearmath.Transform;
import com.sun.org.apache.xml.internal.utils.StopParseException;

import xreal.Angle3f;
import xreal.CVars;
import xreal.Engine;
import xreal.Trajectory;
import xreal.TrajectoryType;
import xreal.common.EntityType;

public class TestBox extends GameEntity {
	
	long stopTime = 0;
	
	TestBox(final Vector3f start, final Vector3f dir)
	{
		//Engine.println("TestBox(start = " + start + ", dir = " + dir);
		
		setEntityState_origin(start);
		setEntityState_eType(EntityType.PHYSICS_BOX);
		//setEntityState_modelindex("models/powerups/ammo/rocketam.md3");
		
		Trajectory pos = new Trajectory();
	
		pos.trType = TrajectoryType.LINEAR;
		pos.trTime = Game.getLevelTime();
		pos.trBase = new Vector4f(start);
		pos.trDelta.scale(30, new Vector4f(dir));
		
		setEntityState_pos(pos);
		
		link();
		
		//new Thread(this).start();
		
		initPhysics(start, dir);
		
		nextThinkTime = (int)(1000.0f / CVars.sv_fps.getInteger());
		
		//start();
	}
	
	/*
	@Override
	public void run() {

		//Thread thisThread = Thread.currentThread();

		//while (ownThread == thisThread) {
		while(!stopThreadRequested) {
			
			try {
				
				long currentTime = (int) System.currentTimeMillis();
				
				//if(stopTime == 0)
				{
					updateEntityStateByPhysics();
				}
				
				if(!rigidBody.isActive()) {
				
					// end this thread in 1 second
					if(stopTime == 0) {
						stopTime = currentTime + nextThinkTime - 10;
					}
					
					if(currentTime >= stopTime) {
						
						stopTime = 0;
						requestStop();
						
						//stop();
						//wait();
						//interrupt();
					}
				}
				
				Thread.sleep(nextThinkTime);
			} catch (InterruptedException e) {
				//Engine.println("Interrupted Exception caught");
				Engine.println("TestBox.run(): Interrupted Exception caught in thread = " + Thread.currentThread().getId());
				//destroy();
			}			

			//Engine.println("TestBox.run() in thread = " + Thread.currentThread().getId());
		}
		
		Engine.println("TestBox.run(): stopped in thread = " + Thread.currentThread().getId());
	}
	*/
	
	protected void initPhysics(final Vector3f start, final Vector3f dir) {
		
		Vector3f maxs = new Vector3f(8, 8, 8);
		Vector3f mins = new Vector3f(-8, -8, -8);
		
		setEntityState_solid(mins, maxs);
		
		collisionShape = new BoxShape(maxs);
		// colShape = new SphereShape(1f);
		
		Game.getCollisionShapes().add(collisionShape);

		Transform startTransform = new Transform();
		startTransform.setIdentity();

		float mass = 1000.0f;

		// rigidbody is dynamic if and only if mass is non zero, otherwise static
		boolean isDynamic = (mass != 0f);

		Vector3f localInertia = new Vector3f(0, 0, 0);
		if (isDynamic) {
			collisionShape.calculateLocalInertia(mass, localInertia);
		}

		startTransform.origin.set(start);

		// using motionstate is recommended, it provides interpolation
		// capabilities, and only synchronizes 'active' objects
		DefaultMotionState myMotionState = new DefaultMotionState(startTransform);
		RigidBodyConstructionInfo rbInfo = new RigidBodyConstructionInfo(mass, myMotionState, collisionShape, localInertia);
		rigidBody = new RigidBody(rbInfo);
		
		if(CVars.g_threadPhysics.getBoolean())
		{
			RigidBodyMotionState gameCopyMotionState = Game.entityMotionStates.get(getEntityState_number());
			rigidBody.setUserPointer(gameCopyMotionState);
		}
		else
		{
			rigidBody.setUserPointer(this);
		}
		
		Vector3f vel = new Vector3f(dir);
		vel.scale(150);
		rigidBody.setLinearVelocity(vel);
		
		//enable CCD if the object moves more than 1 meter in one simulation frame
		rigidBody.setCcdMotionThreshold(10);
		
		//rigidBody.setCcdSweptSphereRadius(20);

		Game.getDynamicsWorld().addRigidBody(rigidBody);
	}
	
	@Override
	public void updateEntityStateByPhysics() {
		//Engine.println("TestBox.updateEntityStateByPhysics()");
		
		if(CVars.g_threadPhysics.getBoolean())
		{
			// set entityState_t::pos
			RigidBodyMotionState gameCopyMotionState = Game.entityMotionStates.get(getEntityState_number());
			
			//trans.origin.snap();
			setEntityState_origin(gameCopyMotionState.origin);
			
			Trajectory pos = new Trajectory();
		
			pos.trType = rigidBody.isActive() ? TrajectoryType.LINEAR : TrajectoryType.STATIONARY;
			pos.trTime = Game.getLevelTime();
			pos.trBase = new Vector4f(gameCopyMotionState.origin);
			pos.trDelta.set(gameCopyMotionState.linearVelocity);
			
			
			setEntityState_pos(pos);
			
			
			
			// set entityState_t::apos 
			Trajectory apos = new Trajectory();
			apos.trType = rigidBody.isActive() ? TrajectoryType.LINEAR : TrajectoryType.STATIONARY;
			apos.trTime = Game.getLevelTime();
			apos.trBase.set(gameCopyMotionState.rotation);
			
			if(rigidBody.isActive()) {
				Vector3f angularVelocity = new Vector3f(gameCopyMotionState.angularVelocity);
				// convert angular velocity to something Q3A likes
				
				// The magnitude is the angular speed, 
				// and the direction describes the axis of rotation.
				float len = angularVelocity.length();
				angularVelocity.normalize();
				
				AxisAngle4f axisAngle = new AxisAngle4f(angularVelocity, len);
				//AxisAngle4f axisAngle = new AxisAngle4f(new Vector3f(0, 0, 1), 30);
				
				Quat4f angularVelocityQuat = new Quat4f();
				angularVelocityQuat.set(axisAngle);

				apos.trDelta.set(angularVelocityQuat);
				//apos.trDelta = new Vector3f(new Angle3f(quat));
			}
			
			//apos.trType = TrajectoryType.STATIONARY;

			setEntityState_apos(apos);
			
			//link();
			
			setEntityState_generic1(rigidBody.getActivationState());
		}
		else
		{
			if(rigidBody.getMotionState() != null)
			{
				// set entityState_t::pos
				Transform trans = new Transform();
				rigidBody.getMotionState().getWorldTransform(trans);
				
				//trans.origin.snap();
				setEntityState_origin(trans.origin);
				
				Trajectory pos = new Trajectory();
			
				pos.trType = rigidBody.isActive() ? TrajectoryType.LINEAR : TrajectoryType.STATIONARY;
				pos.trTime = Game.getLevelTime();
				pos.trBase = new Vector4f(trans.origin);
				
				Vector3f linearVelocity = new Vector3f();
				rigidBody.getLinearVelocity(linearVelocity);
				pos.trDelta.set(linearVelocity);
				
				
				setEntityState_pos(pos);
				
				
				
				// set entityState_t::apos 
				Trajectory apos = new Trajectory();
				apos.trType = rigidBody.isActive() ? TrajectoryType.LINEAR : TrajectoryType.STATIONARY;
				apos.trTime = Game.getLevelTime();
	
				Quat4f quat = new Quat4f();
				trans.getRotation(quat);
	
				//Angle3f angles = new Angle3f(quat);
				//apos.trBase.set(angles);
				apos.trBase.set(quat);
				
				if(rigidBody.isActive()) {
					Vector3f angularVelocity = new Vector3f();
					rigidBody.getAngularVelocity(angularVelocity);
					
					//http://www.euclideanspace.com/physics/kinematics/angularvelocity/
						
						/*
						 * Representing angular velocity using matrices
	
							For information about differentiating a matrix see this page.
							
							We have already seen that a 3D vector is sufficient to hold all the necessary information about angular velocity. 
							However there may be situations where we might want to hold this information in a matrix.
							In that case we can use the following matrix:
							
							[~w]= 	
							0 		-wz		wy
							wz 		0 		-wx
							-wy 	wx		0
						 */
					
					// convert angular velocity to something Q3A likes
					
					// The magnitude is the angular speed, 
					// and the direction describes the axis of rotation.
					float len = angularVelocity.length();
					angularVelocity.normalize();
					
					AxisAngle4f axisAngle = new AxisAngle4f(angularVelocity, len);
					//AxisAngle4f axisAngle = new AxisAngle4f(new Vector3f(0, 0, 1), 30);
					
					Quat4f angularVelocityQuat = new Quat4f();
					angularVelocityQuat.set(axisAngle);
	
					//apos.trDelta.set(angularVelocityQuat);
					apos.trDelta.set(axisAngle);
					
				}
				
				//apos.trType = TrajectoryType.STATIONARY;
	
				setEntityState_apos(apos);
				
				//link();
				
				setEntityState_generic1(rigidBody.getActivationState());
			}
		}
	}
}
