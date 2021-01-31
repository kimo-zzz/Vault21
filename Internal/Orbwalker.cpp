#include "Orbwalker.h"
#include "Globals.h"

//#define UseIssueOrder

bool DisableNextAttack = false;

/// <summary>
///     <c>true</c> if the auto attack missile was launched from the player.
/// </summary>
bool _missileLaunched;

/// <summary>
///     The last target
/// </summary>
int _lastTarget;

void PressLeftClick()
{

	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(INPUT));

	Sleep(8);

	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));
}

void PressRightClick()
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, &input, sizeof(INPUT));

	Sleep(8);

	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(INPUT));
}

void MoveCursorTo(float x, float y)
{
	static float fScreenWidth = (float)::GetSystemMetrics(SM_CXSCREEN) - 1;
	static float fScreenHeight = (float)::GetSystemMetrics(SM_CYSCREEN) - 1;

	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	input.mi.dx = (LONG)(x * (65535.0f / fScreenWidth));
	input.mi.dy = (LONG)(y * (65535.0f / fScreenHeight));

	// Sometimes this fails idk why the fuck but calling the function two times seems to solve it
	SendInput(1, &input, sizeof(INPUT));
	SendInput(1, &input, sizeof(INPUT));
}


bool Orbwalker::Orbwalk(CObject* target, float extraWindup = 90.f)
{
	if (Engine::IsChatBoxOpen())
		return false;


	if (CanAttack() && target != nullptr && target->IsVisible() && target->IsAlive() && target->IsTargetable())
	{
#if defined(UseIssueOrder)
		Engine::AttackTarget(target);
#else
		Vector TargetPos_W2S;
		Functions.WorldToScreen(&Engine::GetMouseWorldPosition(), &Original_Pos);
		Functions.WorldToScreen(&target->GetPos(), &TargetPos_W2S);
		MoveCursorTo(TargetPos_W2S.X, TargetPos_W2S.Y);
		PressRightClick();
#endif
		LastAttackCommandT = float(GetTickCount()) + 30;
	}
	else if (CanMove(extraWindup) && LastMoveCommandT < GetTickCount())
	{
#if defined(UseIssueOrder)
		Engine::MoveTo(&Engine::GetMouseWorldPosition());
#else
		if (Original_Pos.X || Original_Pos.Y) {
			MoveCursorTo(Original_Pos.X, Original_Pos.Y);
			Original_Pos.X = 0.0f;
			Original_Pos.Y = 0.0f;
		}
		PressRightClick();
#endif
		LastMoveCommandT = GetTickCount() + 50;
	}
	return true;
}

/// <summary>
/// Returns true if the LocalPlayer can Attack.
/// </summary>
/// <returns></returns>
bool Orbwalker::CanAttack()
{
	//return Engine::GetGameTimeTickCount() + 30 / 2 >= LastAttackCommandT + Engine::GetLocalObject()->GetAttackDelay() * 1000;
	return float(GetTickCount()) + 30 / 2.f  >= LastAttackCommandT +me->GetAttackDelay() * 1000.f;
}

/// <summary>
/// Returns true if the LocalPlayer can Move.
/// </summary>
/// <returns></returns>
bool Orbwalker::CanMove(float extraWindup)
{
	return me->GetChampionName() == "Kalista" ||  float(GetTickCount()) + 30 * 0.5f >= LastAttackCommandT + me->GetAttackCastDelay() * 1000.f + (30 * 1.5f) + extraWindup;
}
