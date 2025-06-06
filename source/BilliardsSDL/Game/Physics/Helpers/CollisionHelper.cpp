#include "CollisionHelper.h"

namespace CollisionHelper
{
	bool ComputeCirclesCollisionWithOutputData(const Circle& circleA, const Circle& circleB,
		Vector2<float>& outNormalForAB, float& outIntersectDistance)
	{
		const Vector2<float> ab = circleB.p_position - circleA.p_position;
		const float distanceBetweenCircles = ab.Length();		
		outNormalForAB = -ab / distanceBetweenCircles;

		const float radiusSum = circleA.GetRadius() + circleB.GetRadius();
		outIntersectDistance = radiusSum - distanceBetweenCircles;

		return distanceBetweenCircles < radiusSum;
	}


	bool ComputeCircleAARectCollisionWithOutputData(const Circle& circleA, const Rect<float>& aaRectB,
		Vector2<float>& outNormalForAB, float& outIntersectDistance)
	{
		Line<float> collisionLineEdge;
		Vector2<float> collisionPointOnEdge;
		float distanceEdgeToCircleCenter;
		const bool collidedWithEdge = Math::GetCollisiontRectEdgeLineWithCircle(circleA, aaRectB,
			collisionLineEdge, collisionPointOnEdge, distanceEdgeToCircleCenter);

		
		const Vector2<float> rectCenter = aaRectB.GetCenterPosition();
		const Vector2<float> ab = rectCenter - circleA.p_position;
		const float distanceBetweenRectAndCircle = ab.Length();

		const bool circleIsInside = Math::IsPointInsideRect(aaRectB, circleA.p_position);

		Vector2<float> perpendicularPointOnEdge = collisionLineEdge.GetPointAtRatio(0.5f);
		outNormalForAB = (perpendicularPointOnEdge - rectCenter).Normalized();

		if (circleIsInside)
		{
			outIntersectDistance = circleA.GetRadius() + distanceEdgeToCircleCenter;
			return true;
		}

		if (!collidedWithEdge)
		{
			return false;
		}

		
		outIntersectDistance = circleA.GetRadius() - distanceEdgeToCircleCenter;
		
		return true;
	}



	bool ComputeAARectsCollisionWithOutputData(const Rect<float>& aaRectA, const Rect<float>& aaRectB,
		Vector2<float>& outNormalForAB, float& outIntersectDistance)
	{
		if (!Math::AreAARectsIntersecting(aaRectA, aaRectB))
		{
			return false;
		}


		float distanceToEdge;
		const Line<float>& closestRectBEdgeLine = Math::GetClosestRectEdgeLineToPoint(aaRectA.GetCenterPosition(), aaRectB, distanceToEdge);
		const Vector2<float> rectaBCenter = aaRectB.GetCenterPosition();

		Vector2<float> perpendicularPointOnEdge = closestRectBEdgeLine.GetPointAtRatio(0.5f);

		outNormalForAB = (perpendicularPointOnEdge - rectaBCenter).Normalized();

		outIntersectDistance = distanceToEdge;

		return true;
	}



	void ApplyContactCollision(Rigidbody2D* rigidbody, const Vector2<float>& contactNormal, const float& intersectDistance)
	{
		// Position reflection
		// Xt+dt = X't+dt - (1+BounceCoef)*(n�X't+dt + d)*n
		// Xt+dt = X't+dt + (contactNormal * (2 + halfIntDist))
		Vector2<float> position = rigidbody->p_position + (contactNormal * (2 * intersectDistance));

		rigidbody->p_position = position;



		const float bounceE{ 1 + rigidbody->GetPhysicMaterial()->GetBounciness() };
		const float additionSign = Vector2<float>::Dot(contactNormal, rigidbody->GetVelocity()) > 0 ? 1 : -1;
		//printf("%s ---> sign: %f\n", rigidbody->GetGameObject()->GetName().c_str(), additionSign);


		// Velocity reflection
		// Vt+dt = V't+dt - (1+BounceCoef)*(n�V't+dt)*n
		Vector2<float> velocity = rigidbody->GetVelocity() +
			(contactNormal * (additionSign* bounceE * Vector2<float>::Dot(contactNormal, rigidbody->GetVelocity())));

		rigidbody->SetVelocity(velocity);

				
		// Acceleration reflection
		// At+dt = A't+dt - (1+BounceCoef)*(n�A't+dt)*n
		Vector2<float> acceleration = rigidbody->GetAcceleration() +
			(contactNormal * (additionSign* bounceE * Vector2<float>::Dot(contactNormal, rigidbody->GetAcceleration())));

		rigidbody->SetAcceleration(acceleration);				
	}


	void ApplyCollisionForceOnRestingBody(Rigidbody2D* restingRigidbody, const Vector2<float>& otherMotionForce, const Vector2<float>& contactNormal)
	{
		restingRigidbody->ApplyForce(contactNormal * Vector2<float>::Dot(otherMotionForce, contactNormal));
	}

}