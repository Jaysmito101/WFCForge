#include "Generators/MazeGen2D/MazeGen2DAlgorithm.hpp"
#include "WFCForge.hpp"


namespace WFCForge
{
	void MazeGen2DAlgorithm::OnSetup() {}
	
	void MazeGen2DAlgorithm::OnDestroy() {}

	void MazeGen2DAlgorithm::Setup(int w, int h, int s) 
	{
		if (this->board != nullptr) this->Destroy();
		this->seed = s;
		this->width = w;
		this->height = h;
		srand(this->seed);
		this->board = new bool[w * h];
		this->finished = false;
		this->OnSetup();
	}

	void MazeGen2DAlgorithm::Destroy()
	{
		WFC_SAFE_DELETE(this->board);
		srand(time(NULL)); // optional
		this->OnDestroy();
	}
}