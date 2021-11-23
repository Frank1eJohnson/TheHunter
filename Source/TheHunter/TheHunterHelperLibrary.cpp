//==============================================================================
//
// Copyright 2016-2017 Windy Darian(Ruoyu Fan), Xueyin Wan, Menglu Wang
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//==============================================================================

#include "TheHunter.h"
#include "TheHunterHelperLibrary.h"

#include <limits>
#include <cmath>

FVector UTheHunterHelperLibrary::RotateTowardsRandomDirection(FVector Origin, float Angle)
{

	auto rotationAxis = FVector::CrossProduct(Origin, FMath::VRand());
	while (rotationAxis.IsNearlyZero())
	{
		rotationAxis = FVector::CrossProduct(Origin, FMath::VRand());
	}
	rotationAxis.Normalize();

	return Origin.RotateAngleAxis(Angle, rotationAxis);

}


float CalculateProjectileDirectionOneDimensionHelper(float Target, float Gravity, float Speed, bool * bWillHit, float * Time)
{
	constexpr auto InfFloat = std::numeric_limits<float>::max();
	// Helper function for 1D special case
	// Gravity should always be positive
	if (Target >= 0)
	{
		if (bWillHit) { *bWillHit = true; }
		if (Time)
		{
			*Time = (-Speed + FMath::Sqrt(Speed*Speed + 2 * Gravity * Target) )/ Gravity;
		}
		return 1.0f; // Same direction as Gravity
	}
	else
	{
		auto Delta = Speed*Speed + 2 * Gravity * Target;
		if (Delta < 0)
		{
			if (bWillHit) { *bWillHit = false; }
			if (Time)
			{
				*Time = InfFloat;
			}
		}
		else
		{
			if (bWillHit) { *bWillHit = true; }
			if (Time)
			{
				auto SqrtDelta = FMath::Sqrt(Delta);
				if (-SqrtDelta + Speed >= 0)
				{
					*Time = (-SqrtDelta + Speed) / Gravity;
				}
				else
				{
					*Time = (SqrtDelta + Speed) / Gravity;
				}
			}
		}
		return -1.0f;
	}

}

FVector CalculateProjectileDirectionHelper(FVector Target, FVector Origin, FVector Gravity, float Speed, bool * bWillHit, float * Time)
{
	constexpr auto InfFloat = std::numeric_limits<float>::max();

	if (Speed < 0)
	{
		Speed *= -1.0f;
	}

	auto RelativeTarget = Target - Origin;
	if (RelativeTarget.IsNearlyZero())
	{
		// Special case: same location
		if (bWillHit) { *bWillHit = true; }
		if (Time){*Time = 0.0f;}
		return FVector::ForwardVector;
	}
	else if (FMath::IsNearlyZero(Speed))
	{
		// Special case: zero speed
		if (bWillHit) { *bWillHit = false; }
		if (Time) { *Time = InfFloat; }
		return FVector::ForwardVector;
	}
	else if (Gravity.IsNearlyZero())
	{
		// Special case: no gravity
		FVector Direction; float Length;
		RelativeTarget.ToDirectionAndLength(Direction, Length);
		if (bWillHit) { *bWillHit = true; }
		if (Time)
		{
			*Time = Length / Speed;
		}
		return Direction;
	}

	FVector GravityAxis;
	float GravitySize;
	Gravity.ToDirectionAndLength(GravityAxis, GravitySize);
	auto FrontAxis = FVector::CrossProduct(Gravity, FVector::CrossProduct(RelativeTarget, Gravity));
	
	bool bIsDifferentDirectionFromGravity = FrontAxis.Normalize();
	if (!bIsDifferentDirectionFromGravity)
	{
		// Special case: X and gravity facing the same direction
		auto DirectionFactor = CalculateProjectileDirectionOneDimensionHelper(
			FVector::DotProduct(GravityAxis, RelativeTarget),
			GravitySize,
			Speed,
			bWillHit,
			Time
		);
		return DirectionFactor * GravityAxis;
	}

	auto Distance_X = FVector::DotProduct(FrontAxis, RelativeTarget);
	auto Distance_Y = FVector::DotProduct(GravityAxis, RelativeTarget);

	auto Distance_X_2 = Distance_X * Distance_X;
	auto Distance = RelativeTarget.Size();
	auto GravitySize_2 = GravitySize * GravitySize;
	auto Speed_2 = Speed * Speed;
	auto Speed_4 = Speed_2 * Speed_2;
	
	auto DeltaA = -Distance_X_2 * GravitySize_2 
		+ 2 * Distance_Y * GravitySize * Speed_2 
		+ Speed_4;

	// To make sure archers can fire to a close position even if cannot hit;
	auto AlteredSqrtDeltaA = DeltaA >= 0 ? FMath::Sqrt(DeltaA) : 0;
	auto DeltaB = Distance_Y * GravitySize + Speed_2 + AlteredSqrtDeltaA;

	constexpr auto OneOverSqrtTwo = 0.707107f;
	auto X = (DeltaB > 0)? (FMath::Sqrt(DeltaB) * Distance_X / Distance * OneOverSqrtTwo) : 0;
	auto Y = (DeltaB > 0)? (Distance_Y * X / Distance_X - 0.5f * Distance_X * GravitySize / X) : -Speed;

	if (DeltaB < 0 || DeltaA < 0)
	{
		if (bWillHit) { *bWillHit = false; }
		if (Time) { *Time = InfFloat; }
	}
	else
	{
		if (bWillHit) { *bWillHit = true; }
		if (Time) { *Time = Distance_X / X; }
	}

	auto result = GravityAxis * Y + FrontAxis * X;
	result.Normalize();

	return result;
}

FVector UTheHunterHelperLibrary::CalculateProjectileDirection(FVector Target, FVector Origin, FVector Gravity, float Speed, bool & bWillHit, float & Time)
{
	return CalculateProjectileDirectionHelper(Target, Origin, Gravity, Speed, &bWillHit, &Time);
}
