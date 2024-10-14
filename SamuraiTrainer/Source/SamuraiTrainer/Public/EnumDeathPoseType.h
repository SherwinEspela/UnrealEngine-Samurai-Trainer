// Copyright 2024 Sherwin Espela. All rights reserved.
#pragma once

UENUM(BlueprintType)
enum class EDeathPoseTypes : uint8
{
	EDPT_DeathPose1				UMETA(DisplayName = "Death Pose 1"),
	EDPT_DeathPose2				UMETA(DisplayName = "Death Pose 2"),
	EDPT_DeathPose3				UMETA(DisplayName = "Death Pose 3"),
	EDPT_DeathPose4				UMETA(DisplayName = "Death Pose 4"),
	EDPT_DeathPose5				UMETA(DisplayName = "Death Pose 5"),
	EDPT_DeathPoseParryFatal1	UMETA(DisplayName = "Death Pose Parry Fatal 1"),
	EDPT_DeathPoseParryFatal2	UMETA(DisplayName = "Death Pose Parry Fatal 2")
};
