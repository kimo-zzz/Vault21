#include "stdafx.h"
#include "Vector3.h"


namespace V21 {
	
	class PathManager
	{
	public:
		int32_t mCurrentPathId; //0x0000
		char pad_0004[4]; //0x0004
		Vector3 mStartPosition; //0x0008
		Vector3 mEndPosition; //0x0014
		Vector3* mPath; //0x0020
		Vector3* mEnd; //0x0024

		inline int GetSize() {
			return ((DWORD)this->mEnd - (DWORD)this->mPath) / (sizeof(Vector3));
		}

		inline float GetPathLength() {
			float length = 0;
			for (int i = 0; i < this->GetSize(); ++i)
				length += this->mPath[i + 1].Distance(this->mPath[i]);

			return length;
		}

		inline std::vector<Vector3> GetPathList() {
			std::vector<Vector3> result = std::vector<Vector3>();
			int size = this->GetSize();
			for (int i = this->mCurrentPathId - 1; i < size; ++i)
				result.push_back(this->mPath[i]);

			return result;
		}

		inline std::vector<Vector3> CutPath(float distance)
		{
			std::vector<Vector3> result = std::vector<Vector3>();
			float Distance = distance;
			if (distance < 0)
			{
				this->mPath[0] = this->mPath[0] + distance * (this->mPath[1] - this->mPath[0]).Normalized();
				return this->GetPathList();
			}

			for (int i = 0; i < this->GetSize() - 1; i++)
			{
				float dist = this->mPath[i].Distance(this->mPath[i + 1]);
				if (dist > Distance)
				{
					result.push_back(this->mPath[i] + Distance * (this->mPath[i + 1] - this->mPath[i]).Normalized());
					for (int j = i + 1; j < this->GetSize(); j++)
					{
						result.push_back(this->mPath[j]);
					}

					break;
				}
				Distance -= dist;
			}
			if (!result.size())
				result.push_back(this->mPath[this->GetSize() - 1]);

			return result;
		}
	};
}