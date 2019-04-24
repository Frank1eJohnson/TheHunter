//==============================================================================
//
// Copyright 2016 Windy Darian(Ruoyu Fan), Xueyin Wan, Menglu Wang
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


FVector UTheHunterHelperLibrary::RotateTowardsRandomDirection(FVector origin, float angle)
{

	auto rotationAxis = FVector::CrossProduct(origin, FMath::VRand());
	while (rotationAxis.IsNearlyZero())
		rotationAxis = FVector::CrossProduct(origin, FMath::VRand());
	rotationAxis.Normalize();

	return origin.RotateAngleAxis(angle, rotationAxis);

}


