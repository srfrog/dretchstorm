package xreal.client.game;

import javax.vecmath.Matrix3f;
import javax.vecmath.Matrix4f;
import javax.vecmath.Vector3f;

import xreal.Angle3f;
import xreal.CVars;
import xreal.Color;
import xreal.client.EntityState;
import xreal.client.PlayerState;
import xreal.client.game.util.RenderUtils;
import xreal.common.Config;

import com.bulletphysics.collision.shapes.CapsuleShapeZ;
import com.bulletphysics.collision.shapes.ConvexShape;
import com.bulletphysics.collision.shapes.CylinderShapeZ;
import com.bulletphysics.collision.shapes.SphereShape;
import com.bulletphysics.linearmath.Transform;

public class CEntity_Player extends CEntity {
	
//	private int				muzzleFlashTime;
	
//	playerEntity_t 			pe;
	public PlayerState		ps;
	
	public CEntity_Player(EntityState es) {
		super(es);
		
		//Engine.println("CEntity_Player(entity number = " + es.getNumber() + ")");
	}

	/**
	 * A player just came into view or teleported, so reset all animation info.
	 */
	@Override
	public void reset() {
		
		super.reset();
		
		errorTime = -99999;	// guarantee no error decay added
		extrapolated = false;
	
		//CG_ClearLerpFrame(&cgs.clientinfo[cent->currentState.clientNum], &cent->pe.legs, cent->currentState.legsAnim);
		//CG_ClearLerpFrame(&cgs.clientinfo[cent->currentState.clientNum], &cent->pe.torso, cent->currentState.torsoAnim);
	
		lerpOrigin = currentState.pos.evaluatePosition(ClientGame.getTime());
		lerpAngles = new Angle3f(currentState.apos.evaluatePosition(ClientGame.getTime()));
	
		rawOrigin = new Vector3f(lerpOrigin);
		rawAngles = new Angle3f(lerpAngles);
		
		/*	
		memset(&cent->pe.legs, 0, sizeof(cent->pe.legs));
		cent->pe.legs.yawAngle = cent->rawAngles[YAW];
		cent->pe.legs.yawing = qfalse;
		cent->pe.legs.pitchAngle = 0;
		cent->pe.legs.pitching = qfalse;
	
		memset(&cent->pe.torso, 0, sizeof(cent->pe.legs));
		cent->pe.torso.yawAngle = cent->rawAngles[YAW];
		cent->pe.torso.yawing = qfalse;
		cent->pe.torso.pitchAngle = cent->rawAngles[PITCH];
		cent->pe.torso.pitching = qfalse;
	
		if(cg_debugPosition.integer)
		{
			CG_Printf("%i ResetPlayerEntity yaw=%i\n", cent->currentState.number, cent->pe.torso.yawAngle);
		}
		 */
	}
	
	@Override
	public void addToRenderer() throws Exception 
	{
		super.addToRenderer();
		
		// get the player model information
		/*
		renderfx = 0;
		if(currentState.number == cg.snap->ps.clientNum)
		{
			if(!cg.renderingThirdPerson)
			{
				renderfx = RF_THIRD_PERSON;	// only draw in mirrors
			}
			else
			{
				if(cg_cameraMode.integer)
				{
					return;
				}
			}
		}
		*/
		
		if(CVars.cg_drawPlayerCollision.getBoolean()) {
			renderCollisonShape();
		}
	}
	
	private void renderCollisonShape() throws Exception 
	{
		//Engine.println("CEntity_Player.renderCollisionBox()");
		
		Vector3f        mins = new Vector3f();
		Vector3f        maxs = new Vector3f();

		//mins.set(Config.playerMins);
		//maxs.set(Config.playerMaxs);
		
		Matrix4f transform;
		Matrix3f rotation;

		// if it's us
		if(currentState.getNumber() == ClientGame.getClientNum())
		{
			if(!ClientGame.getCamera().isRenderingThirdPerson())
				return;
			
			// use the view height
			//maxs[2] = cg.predictedPlayerState.viewheight + 6;
			
			ConvexShape collisionShape;
			
			//collisionShape = new CapsuleShapeZ(Config.PLAYER_WIDTH / 2, Config.PLAYER_HEIGHT / 2);
			collisionShape = new CylinderShapeZ(new Vector3f(CVars.pm_bodyWidth.getValue() / 2, CVars.pm_bodyWidth.getValue() / 2, CVars.pm_normalHeight.getValue() / 2));
			//collisionShape = new SphereShape(Config.PLAYER_WIDTH / 2);
			
			//Matrix3f rotation = new Matrix3f();
			//lerpAngles.get(rotation);
		
			//collisionShape.getAabb(new Transform(new Matrix4f(rotation, lerpOrigin, 1)), mins, maxs);
		
			Transform t = new Transform();
			t.setIdentity();
			collisionShape.getAabb(t, mins, maxs);
			
			rotation = new Matrix3f();
			
			Angle3f angles = new Angle3f(0, ps.viewAngles.y, 0);
			angles.get(rotation);
			//ps.viewAngles.get(rotation);
			//currentState.angles.get(rotation);
			
			transform = new Matrix4f(rotation, lerpOrigin, 1);
		}
		else
		{
			int             x, zd, zu;

			// otherwise grab the encoded bounding box
			x = (currentState.solid & 255);
			zd = ((currentState.solid >> 8) & 255);
			zu = ((currentState.solid >> 16) & 255) - 32;

			mins.x = mins.y = -x;
			maxs.x = maxs.y = x;
			mins.z = -zd;
			maxs.z = zu;
			
			rotation = new Matrix3f();
			lerpAngles.get(rotation);
			
			transform = new Matrix4f(rotation, lerpOrigin, 1);
		}
		
		
		
		// draw outer collision box
		//RenderUtils.renderBox(transform, mins, maxs, Color.Green, ClientGame.getMedia().debugPlayerAABB_twoSided);
		RenderUtils.renderCylinderShapeZ(transform, mins, maxs, Color.Green, ClientGame.getMedia().debugPlayerAABB_twoSided);
		
		// draw player origin
		mins.set(-1, -1, -1);
		maxs.set(1, 1, 1);
		RenderUtils.renderBox(transform, mins, maxs, Color.Red, ClientGame.getMedia().debugPlayerAABB_twoSided);
		
		// draw legs
		if(false)
		{
			ConvexShape legsShape = new SphereShape(CVars.pm_bodyWidth.getInteger() / 2);
			
			Transform t = new Transform();
			t.setIdentity();
			legsShape.getAabb(t, mins, maxs);
			
			Vector3f gravityNormal = new Vector3f(CVars.g_gravityX.getValue(), CVars.g_gravityY.getValue(), CVars.g_gravityZ.getValue());
			gravityNormal.normalize();
			
			Vector3f legsPosition = new Vector3f();
			legsPosition.scaleAdd(CVars.pm_normalHeight.getInteger() / 2 - CVars.pm_bodyWidth.getInteger() / 2, gravityNormal, lerpOrigin);
			
			transform = new Matrix4f(rotation, legsPosition, 1);
			
			RenderUtils.renderBox(transform, mins, maxs, Color.Blue, ClientGame.getMedia().debugPlayerAABB_twoSided);
		}
		
		// draw player head
		if(ps != null)
		{
			mins.set(-6, -6, -6);
			maxs.set(6, 6, 6);
		
			Vector3f headPosition = new Vector3f(lerpOrigin);
			//headPosition.z += Config.playerMins.z;
			//headPosition.z += ps.viewHeight + 6;
			headPosition.z += ps.viewHeight;
			
			transform = new Matrix4f(rotation, headPosition, 1);
			
			RenderUtils.renderBox(transform, mins, maxs, Color.Blue, ClientGame.getMedia().debugPlayerAABB_twoSided);
		}
	}
}
