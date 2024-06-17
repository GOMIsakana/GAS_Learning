// Super Copyright


#include "Data/LevelUpInfo.h"

FLevelUpData ULevelUpInfo::FindLevelDataForExp(float Exp, bool bReturnNextLevel) const
{
	FLevelUpData LastLevelData = FLevelUpData();
	LastLevelData.TargetLevel = -2;
	bool bLevelContinuous = true;
	for (FLevelUpData CurrentLevelData : LevelUpDatas)
	{
		bLevelContinuous = LastLevelData.TargetLevel + 1 == CurrentLevelData.TargetLevel;
		// 当大于当前升级的需求时, 寻找下一个等级
		if (Exp >= CurrentLevelData.ExperienceRequirement)
		{
			LastLevelData = CurrentLevelData;
		}
		// 当小于当前升级的需求，且等级不连续：
		//   尝试寻找上一个等级到这个等级之间长度为(CurrentLevel - LastLevel)的等差数列, 且首项为LastLevel, 末项为CurrentLevel
		//   返回寻找到的等级
		// 如果等级连续:
		//   返回上一个等级
		else
		{
			// 这部分其实和循环一次一样的, 考虑到自己的猪脑不知道怎么复用就歇了
			FLevelUpData FoundLevelData = FLevelUpData();
			float ArithmeticDifference = (CurrentLevelData.ExperienceRequirement - LastLevelData.ExperienceRequirement) / (CurrentLevelData.TargetLevel - LastLevelData.TargetLevel);
			float LastLevelExpRequirement = LastLevelData.ExperienceRequirement + ArithmeticDifference;
			// 循环，直到找到下一个比当前等级小的等级
			for (int i = LastLevelData.TargetLevel + 1; i <= CurrentLevelData.TargetLevel; i++)
			{
				// 找到对的等级就返回
				if (Exp < LastLevelExpRequirement && !bReturnNextLevel)
				{
					return FoundLevelData;
				}
				// 如果经验更大就将当前等级视为找到的等级将FoundLevelData的数据设置为当前推算的数据
				FoundLevelData.TargetLevel = i;
				FoundLevelData.ExperienceRequirement = LastLevelExpRequirement;
				FoundLevelData.RewardAttributePoint = LastLevelData.RewardAttributePoint;
				FoundLevelData.RewardSpellPoint = LastLevelData.RewardSpellPoint;

				// 如果需要找下一级那就先获取当前轮次（下一级）的信息再返回
				if (Exp < LastLevelExpRequirement && bReturnNextLevel)
				{
					return FoundLevelData;
				}

				LastLevelExpRequirement += ArithmeticDifference;
			}
			/*
			if (bLevelContinuous)
			{
				return bReturnNextLevel ? CurrentLevelData : LastLevelData;
			}
			else
			{
			}
			*/
		}
	}
	return LastLevelData;
}
