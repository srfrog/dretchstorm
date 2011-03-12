package xreal.client.game;

import javax.vecmath.Matrix3f;
import javax.vecmath.Matrix4f;
import javax.vecmath.Point3f;
import javax.vecmath.Vector3f;

import com.bulletphysics.collision.dispatch.CollisionObject;

import xreal.Color;
import xreal.Engine;
import xreal.client.EntityState;
import xreal.client.game.util.RenderUtils;
import xreal.client.renderer.Polygon;
import xreal.client.renderer.Renderer;
import xreal.client.renderer.Vertex;

public class CEntity_PhysicsBox extends CEntity {
	
	public CEntity_PhysicsBox(EntityState es) {
		super(es);
	}

	
	@Override
	public void addToRenderer() throws Exception 
	{
		super.addToRenderer();
		
		Vector3f        mins = new Vector3f();
		Vector3f        maxs = new Vector3f();

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
		}

		// set the polygon's vertex colors
		Color color;
		switch (currentState.generic1)
		{
			case CollisionObject.ACTIVE_TAG:
				color = Color.White;
				break;

			case CollisionObject.ISLAND_SLEEPING:
				color = Color.Green;
				break;

			case CollisionObject.WANTS_DEACTIVATION:
				color = Color.Cyan;
				break;

			case CollisionObject.DISABLE_DEACTIVATION:
				color = Color.Red;
				break;

			case CollisionObject.DISABLE_SIMULATION:
				color = Color.Yellow;
				break;

			default:
				color = Color.Magenta;
				break;
		}

		Matrix3f rotation = new Matrix3f();
		rotation.setIdentity();
		rotation.set(lerpQuat);
		//lerpAngles.get(rotation);
		
		//MatrixFromAngles(rotation, cent->lerpAngles[PITCH], cent->lerpAngles[YAW], cent->lerpAngles[ROLL]);
		//MatrixSetupTransformFromRotation(transform, rotation, cent->lerpOrigin);
		
		Matrix4f transform = new Matrix4f(rotation, lerpOrigin, 1);

		RenderUtils.renderBox(transform, mins, maxs, color, ClientGame.getMedia().debugPlayerAABB_twoSided);
	}
}
