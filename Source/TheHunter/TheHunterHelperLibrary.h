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

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TheHunterHelperLibrary.generated.h"

/**
 * Helper function library for TheHunter
 */
UCLASS()
class THEHUNTER_API UTheHunterHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TheHunterHelperLibrary")
	static FVector RotateTowardsRandomDirection(FVector Origin, float Angle);


	/**
	 * Given positions, gravity and projectile initial speed, solve for direction of shooting, also returns time of hitting
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TheHunterHelperLibrary")
	static FVector CalculateProjectileDirection(FVector Target, FVector Origin, FVector Gravity, float Speed, bool& bWillHit, float& Time);
	
	
};
