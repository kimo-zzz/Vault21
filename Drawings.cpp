#include "Drawings.h"

#include "Globals.h"

void Drawings::EvadeDrawings(CObject* &objBuffer, list<CObject*> heroList)
{
	if (g_drawEnemyMissles)
	{
		if ((me->GetPos().DistTo(objBuffer->GetPos()) >= 0.0f) && (me->GetPos().DistTo(objBuffer->GetPos()) <= 4000.0f))
		{
			if (objBuffer->GetParent(heroList) != nullptr)
			{
				if (objBuffer->GetParent(heroList)->IsHero() && objBuffer->GetParent(heroList)->IsEnemyTo(me))
				{
					auto spellStartPos = objBuffer->GetSpellStartPos();
					auto spellEndPos = objBuffer->GetSpellEndPos();

					Vector objspellstartpos_w2s;
					Functions.WorldToScreen(&spellStartPos, &objspellstartpos_w2s);

					Vector objspellendpos_w2s;
					Functions.WorldToScreen(&spellEndPos, &objspellendpos_w2s);

					auto spellWidth = objBuffer->GetMissileSpellInfo()->GetSpellData()->GetSpellWidth();

					ImColor _skillsShots = ImColor(255, 102, 102, 79);
					render.draw_line(objspellstartpos_w2s.X, objspellstartpos_w2s.Y,
						objspellendpos_w2s.X, objspellendpos_w2s.Y, _skillsShots,
						spellWidth);

					auto spellEffectRange = objBuffer->GetMissileSpellInfo()->GetSpellData()->GetSpellEffectRange();

					auto color = createRGB(220, 20, 60); // crimson
					Engine::DrawCircle(&objBuffer->GetPos(), spellEffectRange, &color, 0, 0.0f, 0, 0.5f);
					//render.draw_circle(Pos, spellEffectRange, color, c_renderer::circle_3d, 50, 0.5f);
				}
			}
		}

	}
}


void Drawings::DrawInhibitors(CObject* &objBuffer)
{
	if (objBuffer->IsInhibitor())
	{
		if (g_inhi_respawn)
		{
			if (objBuffer->IsOnScreen())
			{
				auto inhiRespawnTime = objBuffer->GetInhiRemainingRespawnTime();
				if (inhiRespawnTime > 0)
				{
					Vector objpos_w2s;
					Functions.WorldToScreen(&objBuffer->GetPos(), &objpos_w2s);
					ImColor color = ImColor(102, 255, 102, 174);
					
					std::string str_respawnTime = Engine::SecondsToClock(
						static_cast<int>(inhiRespawnTime));
					render.draw_text(objpos_w2s.X, objpos_w2s.Y, str_respawnTime.c_str(), true, color);
				}
			}
		}
	}
}

void Drawings::DrawTurrets(CObject* &objBuffer)
{
	if (objBuffer->IsTurret())
	{
		if (g_enemy_turret)
		{
			if (objBuffer->IsOnScreen())
			{
				auto IsAlive = objBuffer->IsAlive();
				if (IsAlive)
				{
					if (objBuffer->IsEnemyTo(me))
					{
						auto boundingRadius = objBuffer->GetBoundingRadius();
						auto color = createRGB(220, 20, 60); // crimson
						Engine::DrawCircle(&objBuffer->GetPos(), 800.0f + boundingRadius, &color, 0, 0.0f, 0, 0.5f);
						//render.draw_circle(Pos, 800.0f + boundingRadius, color, c_renderer::circle_3d, 50, 0.5f);
					}
				}
			}
		}
	}
}
