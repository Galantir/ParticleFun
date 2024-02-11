#include <iostream>
using namespace std;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class ParticleSimulation
{
private:
	int* State;
	int* Temp;
	int Width;
	int Height;	

public:
	ParticleSimulation(int width, int height)
	{
		Width = width;
		Height = height;

		State = new int[Width * Height];
		Temp = new int[Width * Height];

		memset(State, 0, Width * Height * sizeof(int));
		memset(Temp, 0, Width * Height * sizeof(int));
	}

public:
	void SetPixel(int x, int y, int val)
	{
		State[y * Width + x] = val;
	}

	void Update()
	{
		auto cell = [&](int x, int y)
		{
			return Temp[y * Width + x];
		};

		//copy State state to Temp
		for (int i = 0; i < Width * Height; i++)
			Temp[i] = State[i];

		//clear State
		memset(State, 0, Width * Height * sizeof(int));

		for (int x = 0; x < Width; x++)
		{
			for (int y = Height - 1; y >= 0; y--)
			{
				int cellType = cell(x, y);

				//Sand
				if (cellType == 1)
				{
					if (InBounds(x, y + 1) && cell(x, y + 1) == 0)
					{
						State[(y + 1) * Width + x] = 1;
					}
					else if (InBounds(x - 1, y + 1) && InBounds(x + 1, y + 1) && cell(x - 1, y + 1) == 0 && cell(x + 1, y + 1) == 0)
					{
						if (randomBool())
						{
							State[(y + 1) * Width + (x - 1)] = 1;
						}
						else
						{
							State[(y + 1) * Width + (x + 1)] = 1;
						}
					}
					else if (InBounds(x - 1, y + 1) && cell(x - 1, y + 1) == 0)
					{
						State[(y + 1) * Width + (x - 1)] = 1;
					}
					else if (InBounds(x + 1, y + 1) && cell(x + 1, y + 1) == 0)
					{
						State[(y + 1) * Width + (x + 1)] = 1;
					}
					else
					{
						State[y * Width + x] = 1;
					}
				}
			}
		}		

		memset(Temp, 0, Width * Height * sizeof(int));
	}

	void Draw(olc::PixelGameEngine* pge)
	{
		auto cell = [&](int x, int y)
		{
			return Temp[y * Width + x];
		};

		// First Clear the screen		
		game->Clear(olc::BLACK);

		for (int x = 0; x < Width; x++)
			for (int y = 0; y < Height; y++)
			{
				int cellType = cell(x, y);

				if (cellType == 1)
					game->Draw(x, y, olc::Pixel(196, 145, 2));
			}
	}

private:
	bool InBounds(int x, int y) {
		if (x >= 0 && x < Width && y >= 0 && y < Height)
			return true;
		return false;
	}

	bool randomBool() {
		return rand() > (RAND_MAX / 2);
	}

};

class ParticleFun : public olc::PixelGameEngine
{
public:
	ParticleFun()
	{
		sAppName = "ParticleFun";
	}

private:
	ParticleSimulation* simulation;

protected:
	bool OnUserCreate() override
	{
		simulation = new ParticleSimulation(ScreenWidth(), ScreenHeight());

		return true;
	}

	bool OnUserUpdate(float fElaspedTime) override
	{		
		if (GetMouse(0).bHeld)
		{
			int x = GetMouseX();
			int y = GetMouseY();

			simulation->SetPixel(x, y, 1);
		}

		simulation->Update();
		simulation->Draw(this);
		
		/*if (GetMouse(0).bHeld)
		{
			int x = GetMouseX();
			int y = GetMouseY();

			m_state[y * ScreenWidth() + x] = 1;
		}
		if (GetMouse(1).bHeld)
		{
			int x = GetMouseX();
			int y = GetMouseY();

			m_state[y * ScreenWidth() + x] = 2;
		}	*/

		
				
		return true;
	}

};

int main() {
	ParticleFun game;
	if (game.Construct(256, 256, 4, 4))
		game.Start();
	return 0;
}