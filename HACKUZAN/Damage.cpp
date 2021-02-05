#include "stdafx.h"
#include "Damage.h"
#include "ObjectManager.h"

namespace HACKUZAN
{
	namespace SDK
	{
		float Damage::CalculateAutoAttackDamage(GameObject* source, GameObject* target) {
			auto autoAttackDamageType = DamageType_Physical;
			auto rawPhysicalDamage = 0.0f;
			auto rawMagicalDamage = 0.0f;
			auto rawTrueDamage = 0.0f;
			auto calculatedPhysicalDamage = 0.0f;
			auto calculatedMagicalDamage = 0.0f;
			auto rawTotalDamage = source->TotalAttackDamage();

			//auto targetFlags = target->Flags();
			//auto sourceFlags = source->Flags();

			//if (targetFlags && GameObjectFlags_AIMinionClient) {
			if (target->Minion()) {
				if (target->MaxHealth <= 6.0f) {
					return 1.0f;
				}
			}

			//if (sourceFlags & GameObjectFlags_AIHeroClient) {
			if (source->Hero()) {
				auto damageOnHit = ComputeDamageOnHit(source, target);
				rawPhysicalDamage += damageOnHit.PhysicalDamage;
				rawMagicalDamage += damageOnHit.MagicalDamage;
				rawTrueDamage += damageOnHit.TrueDamage;
			}

			switch (autoAttackDamageType) {
			case DamageType_Physical:
				rawPhysicalDamage += rawTotalDamage;
				break;
			case DamageType_Magical:
				rawMagicalDamage += rawTotalDamage;
				break;
			}

			calculatedPhysicalDamage += CalculatePhysicalDamage(source, target, rawPhysicalDamage);
			calculatedMagicalDamage += CalculateMagicalDamage(source, target, rawMagicalDamage);

			return calculatedPhysicalDamage + calculatedMagicalDamage + rawTrueDamage;
		}

		float Damage::CalculatePhysicalDamage(GameObject* source, GameObject* target, float amount) {
			if (amount <= 0.0f) {
				return 0.0f;
			}

			auto flatArmorPenetration = source->PhysicalLethality * (0.6f + (0.4f * source->Level / 18.0f));
			auto percentArmorPenetration = source->PercentArmorPenetration;
			auto percentBonusArmorPenetration = source->PercentBonusArmorPenetration;
			auto armor = target->Armor;
			auto bonusArmor = target->BonusArmor;

			if (source->Minion()) {
				percentArmorPenetration = 1.0f;
				percentBonusArmorPenetration = 1.0f;
			}
			if (source->Turret()) {
				percentArmorPenetration = 0.7f;
				percentBonusArmorPenetration = 1.0f;
			}
			/*auto sourceType = source->GetClassData()->Type();

			switch (sourceType) { // rito fix pls...
			case GameObjectType::AIMinionClient: {
				percentArmorPenetration = 1.0f;
				percentBonusArmorPenetration = 1.0f;
				break;
			}
			case GameObjectType::AITurretClient: {
				percentArmorPenetration = 0.7f;
				percentBonusArmorPenetration = 1.0f;
				break;
			}
			}*/

			auto bonusTrueDamage = 0.0f;

			//if (sourceType == GameObjectType::AIHeroClient && target->Flags() & GameObjectFlags_AIHeroClient) {
			if (source->Hero() && target->Hero()) {
				if (source->FindBuffHash(0xD4515DC0)) { // "ASSETS/Perks/Styles/Precision/Conqueror/ConquerorEnrage.lua"
					bonusTrueDamage += amount * 0.2f;
					amount *= 0.8f;
				}
			}

			auto damageModifier = ComputeDamageModifier(source, target, DamageType_Physical);
			amount += damageModifier.Flat;
			amount *= damageModifier.Percent;

			if (armor < 0.0f) {
				amount *= 2.0f - 100.0f / (100.0f - armor);
			}
			else if (armor * percentArmorPenetration - bonusArmor * (1.0f - percentBonusArmorPenetration) - flatArmorPenetration >= 0.0f) {
				amount *= 100.0f / (100.0f + (armor * percentArmorPenetration) - (bonusArmor * (1.0f - percentBonusArmorPenetration)) - flatArmorPenetration);
			}

			return std::max(amount + bonusTrueDamage, 0.0f);
		}

		float Damage::CalculateMagicalDamage(GameObject * source, GameObject * target, float amount) {
			if (amount <= 0.0f) {
				return 0.0f;
			}

			auto flatMagicPenetration = source->FlatMagicPenetration;
			auto percentMagicPenetration = source->PercentMagicPenetration;
			auto magicResist = target->MagicResist;

			auto bonusTrueDamage = 0.0f;

			if (target->FindBuffHash(0x8C10C58A)) { // "cursedtouch"
				bonusTrueDamage += amount * 0.1f;
			}

			//auto sourceType = source->Type();

			//if (source->Flags() & GameObjectFlags_AIHeroClient) {
			if (source->Hero()) {
				if (source->FindBuffHash(0xD4515DC0)) { // "ASSETS/Perks/Styles/Precision/Conqueror/ConquerorEnrage.lua"
					bonusTrueDamage += amount * 0.2f;
					amount *= 0.8f;
				}
			}

			auto damageModifier = ComputeDamageModifier(source, target, DamageType_Magical);
			amount += damageModifier.Flat;
			amount *= damageModifier.Percent;

			if (magicResist < 0.0f) {
				amount *= 2.0f - 100.0f / (100.0f - magicResist);
			}
			else if (magicResist * percentMagicPenetration - flatMagicPenetration >= 0.0f) {
				amount *= 100.0f / (100.0f + magicResist * percentMagicPenetration - flatMagicPenetration);
			}

			return std::max(amount + bonusTrueDamage, 0.0f);
		}

		DamageModifierResult Damage::ComputeDamageModifier(GameObject * source, GameObject * target, kDamageType damageType) {
			DamageModifierResult result;
			result.Flat = -source->FlatMagicReduction;
			result.Percent = 1.0f - source->PercentMagicReduction;

			//auto sourceFlags = source->Flags();
			//auto targetFlags = target->Flags();

#pragma region Pushing advatange
			//if (sourceFlags & GameObjectFlags_AIMinionClient && source->IsLaneMinion() && targetFlags & GameObjectFlags_AIMinionClient && target->IsLaneMinion()) {
			if (source->Minion() && source->IsLaneMinion() && target->Minion() && target->IsLaneMinion()) {
				result.Flat -= target->FlatDamageReductionFromBarracksMinionMod;
				result.Percent *= 1.0f + source->PercentDamageToBarracksMinionMod;
			}
#pragma endregion

#pragma region Target buffs
			//if (targetFlags & GameObjectFlags_AIHeroClient) {
			if (target->Hero()) {
				switch (target->BaseCharacterData->SkinHash) {
				case Character::Annie: {
					if (target->FindBuffHash(0x668C0F2F)) { // "MoltenShield"
						auto spell = target->Spellbook.GetSpell(SpellSlot_E);
						result.Percent *= 1.0f - spell->SpellData->Resource->eEffect1[spell->Level] / 100.0f;
					}
					break;
				}
				case Character::Braum: {
					if (target->FindBuffHash(0x7215EE81)) { // "braumeshieldbuff"
						auto spell = target->Spellbook.GetSpell(SpellSlot_E);
						result.Percent *= 1.0f - spell->SpellData->Resource->eEffect3[spell->Level] / 100.0f;
					}
					break;
				}
				case Character::Galio: {
					if (damageType == DamageType_Physical && target->FindBuffHash(0x22086614)) { // "GalioW"
						auto spell = target->Spellbook.GetSpell(SpellSlot_W);
						result.Percent *= 1.0f + (spell->SpellData->Resource->eEffect1[spell->Level] + spell->fEffect2 + spell->fEffect4) * spell->fEffect5 / 50.0f;
					}
					break;
				}
				case Character::Kassadin: {
					if (damageType == DamageType_Magical) {
						result.Percent *= 0.85f;
					}
					break;
				}
				case Character::Malzahar: {
					if (target->FindBuffHash(0x33DB7C71)) { // "malzaharpassiveshield"
						result.Percent *= 0.1f;
					}
					break;
				}
				case Character::MasterYi: {
					if (target->FindBuffHash(0x50E753F4)) { // "Meditate"
						auto spell = target->Spellbook.GetSpell(SpellSlot_W);
						result.Percent *= 1.0f - spell->SpellData->Resource->eEffect2[spell->Level] / 100.0f;
					}
					break;
				}
				}
			}
#pragma endregion

#pragma region Source buffs
			//if (sourceFlags & GameObjectFlags_AIHeroClient) {
			if (source->Hero()) {
				for (auto buffManager : source->Buffs) {
					auto buff = buffManager.Buff;

					if (!buff->IsActive()) {
						continue;
					}

					auto caster = ObjectManager::Instance->ObjectsArray[buff->GetScriptInstance()->CasterId];

					if (!caster)
					{
						continue;
					}

					//GameClient::PrintChat("caster found #1", IM_COL32(144, 238, 144, 69));

					if (buff == NULL) {
						continue;
					}

					switch (buff->Script->Hash) {
					case 0x6F2D5F3D: // "SummonerExhaust"
						result.Percent *= 0.6f;
						break;
					case 0xAF037A39: // "sonapassivedebuff"
						result.Percent *= 1 - (0.25f + 0.0004f * source->TotalAbilityPower());
						break;
					case 0xD68F930B: // "itemsmitechallenge"
						if (source->NetworkId == target->NetworkId) {
							result.Percent *= 0.8f;
						}
						break;
					case 0xD1F5085E: // "itemphantomdancerdebuff"
						if (source->NetworkId == target->NetworkId) {
							result.Percent *= 0.88f;
						}
						break;
					case 0xD7D32CF6: // "barontarget"
						if (target->Team == GameObjectTeam_Neutral && target->BaseCharacterData->SkinHash == Character::SRU_Baron) {
							result.Percent *= 0.5f;
						}
						break;

					}
				}
			}
#pragma endregion

			return result;
		}

		DamageOnHitResult Damage::ComputeDamageOnHit(GameObject * source, GameObject * target) {
			DamageOnHitResult result;

			//auto targetFlags = target->Flags();

#pragma region Source items
			bool isItemChecked[4011] = { false };
			auto inventorySlots = source->HeroInventory.Slots;
			for (auto i = 0; i < 6; i++) {
				auto item = inventorySlots[i];
				auto itemInfo = item->ItemInfo;

				if (!itemInfo) {
					continue;
				}

				auto itemID = itemInfo->ItemData->ItemId;

				if (isItemChecked[static_cast<int>(itemID)]) {
					continue;
				}

				isItemChecked[static_cast<int>(itemID)] = true;

				switch (itemID) {
				case Item::Blade_of_the_Ruined_King: {
					auto itemDamage = target->Health * 0.08f;
					//if (targetFlags & GameObjectFlags_AIMinionClient) {
					if (target->Minion()) {
						itemDamage = std::min(itemDamage, 60.0f);
					}
					result.PhysicalDamage += std::max(itemDamage, 15.0f);
					break;
				}
				case Item::Stalkers_Blade_Enchantment_Bloodrazor: {
					auto itemDamage = target->MaxHealth * 0.04f;
					//if (targetFlags & GameObjectFlags_AIMinionClient) {
					if (target->Minion()) {
						itemDamage = std::min(itemDamage, 75.0f);
					}
					result.PhysicalDamage += itemDamage;
					break;
				}
				case Item::Nashors_Tooth:
					result.MagicalDamage += 15.0f + 0.15f * source->TotalAbilityPower();
					break;
				case Item::Recurve_Bow:
					result.PhysicalDamage += 15.0f;
					break;
				case Item::Titanic_Hydra:
					// "itemtitanichydracleavebuff"
					result.PhysicalDamage += source->FindBuffHash(0x9848DC51) ? 40.0f + 0.1f * source->MaxHealth : 5.0f + 0.01f * source->MaxHealth;
					break;
				case Item::Dead_Mans_Plate: {
					auto buff = source->FindBuffHash(0xBB077487); // "dreadnoughtmomentumbuff"
					if (buff) {
						result.MagicalDamage += buff->Counter;
					}
					break;
				}
				case Item::Wits_End:
					result.MagicalDamage += 42.0f;
					break;
				case Item::Muramana:
					//if (targetFlags & GameObjectFlags_AIHeroClient) {
					if (target->Hero()) {
						auto mana = source->Resource;
						if (mana / source->MaxResource > 0.2f) {
							result.PhysicalDamage += 0.06f * mana;
						}
					}
					break;
				}
			}
#pragma endregion

#pragma region Source buffs
			for (auto buffManager : source->Buffs) {
				auto buff = buffManager.Buff;

				if (!buff->IsActive()) {
					continue;
				}

				auto caster = ObjectManager::Instance->ObjectsArray[buff->GetScriptInstance()->CasterId];

				if (!caster)
				{
					continue;
				}

				//GameClient::PrintChat("caster found #2", IM_COL32(144, 238, 144, 69));

				switch (buff->Script->Hash) {
				case 0xF791AC5A: { // "sonaqprocattacker"
					result.MagicalDamage += 5.0f + 5.0f * caster->Spellbook.GetSpell(SpellSlot_Q)->Level + 0.3f * caster->TotalAbilityPower();
					break;
				}
				case 0xE503837D: { // "NamiE"
					result.MagicalDamage += 5.0f + 15.0f * caster->Spellbook.GetSpell(SpellSlot_E)->Level + 0.2f * caster->TotalAbilityPower();
					break;
				}
				case 0x1D5D9677: { // "itemangelhandbuff"
					static float values[18] = { 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f };
					result.MagicalDamage += values[std::min(caster->Level - 1, 17U)];
					break;
				}
				case 0x3519F8B0: { // "BlessingoftheLizardElder"
					if (!target->FindBuffHash(0x48208A75))// blessingofthelizardelderslow
					{
						result.TrueDamage += 2.0f + 2.0f * std::min(source->Level, 18U);
					}
					break;
				}
				case 0x4ED863B8: { // "aatroxpassivereadyr"
					auto damage = 0.07529f + 0.00471f * std::min(source->Level, 18U);

					if (target->IsMonster()) {
						damage = std::min(damage, 400.0f);
					}

					result.PhysicalDamage += damage;
					break;
				}
				case 0x553CCB5D: { // "akalipweapon"
					static float values[18] = { 39, 42, 45, 48, 51, 54, 57, 60, 69, 78, 87, 96, 105, 120, 135, 150, 165, 180 };
					result.MagicalDamage += values[std::min(caster->Level - 1, 17U)] + 0.9f * caster->TotalBonusAttackDamage() + 0.7f * caster->TotalAbilityPower();
					break;
				}
				case 0xC9742FD2: { // "alistareattack"
					result.MagicalDamage += 40 + 15 * std::min(caster->Level, 18U);
					break;
				}
				case 0xBB89EA44: { // "vaynetumblebonus"
					auto spell = caster->Spellbook.GetSpell(SpellSlot_Q);
					result.PhysicalDamage += caster->TotalAttackDamage() * spell->SpellData->Resource->eEffect1[spell->Level];

					//GameClient::PrintChat(("Resource->eEffect1" + (std::to_string(spell->SpellData->Resource->eEffect1[spell->Level]))).c_str(), IM_COL32(255, 69, 255, 255));
					break;
				}
				case 0x4AED0CE1: { // "cardmasterstackparticle"
					auto spell = caster->Spellbook.GetSpell(SpellSlot_E);
					auto resource = spell->SpellData->Resource;
					auto spellLevel = spell->Level;

					result.MagicalDamage += resource->eEffect1[spellLevel] + caster->TotalAbilityPower() * resource->aEffect1;
					break;
				}
				case 0x8B0B9A1C: { // "bluecardpreattack"
					auto spell = caster->Spellbook.GetSpell(SpellSlot_W);
					auto resource = spell->SpellData->Resource;
					auto spellLevel = spell->Level;

					result.MagicalDamage += resource->eEffect1[spellLevel] + caster->TotalAbilityPower() * resource->aEffect1 + caster->TotalAttackDamage() * resource->aEffect2;
					break;
				}
				case 0x854C3BC3: { // "redcardpreattack"
					auto spell = caster->Spellbook.GetSpell(SpellSlot_W);
					auto resource = spell->SpellData->Resource;
					auto spellLevel = spell->Level;

					result.MagicalDamage += resource->eEffect4[spellLevel] + caster->TotalAbilityPower() * resource->aEffect1 + caster->TotalAttackDamage() * resource->aEffect2;
					break;
				}
				case 0xA96339F2: { // "goldcardpreattack"
					auto spell = caster->Spellbook.GetSpell(SpellSlot_W);
					auto resource = spell->SpellData->Resource;
					auto spellLevel = spell->Level;

					result.MagicalDamage += resource->eEffect5[spellLevel] + caster->TotalAbilityPower() * resource->aEffect1 + caster->TotalAttackDamage() * resource->aEffect2;
					break;
				}
				case 0x22C648A0: // "deceive" Shaco
				{
					auto spell = caster->Spellbook.GetSpell(SpellSlot_Q);
					auto resource = spell->SpellData->Resource;
					auto spellLevel = spell->Level;
					auto damage = resource->eEffect1[spellLevel] + caster->TotalAbilityPower() * resource->aEffect2 + caster->TotalAttackDamage() * resource->aEffect1;

					result.PhysicalDamage += damage;

					break;
				}
				case 0xD9A00D4D://umbrablades
					result.PhysicalDamage *= 1.2f;
					break;
					//TODO Test
				case 0x7E98AA02://neekowpassiveready
				{
					auto spell = caster->Spellbook.GetSpell(SpellSlot_W);
					auto resource = spell->SpellData->Resource;
					auto spellLevel = spell->Level;
					auto damage = resource->eEffect1[spellLevel] + resource->aEffect1 * caster->TotalAbilityPower();

					result.MagicalDamage += damage;
				}
				break;
				case 0x24621017://nautiluspiercinggazeshield
				{
					auto spell = caster->Spellbook.GetSpell(SpellSlot_W);
					auto resource = spell->SpellData->Resource;
					auto spellLevel = spell->Level;
					auto damage = resource->eEffect4[spellLevel] + resource->aEffect1 * caster->TotalAbilityPower();

					result.MagicalDamage += damage;
				}
				break;
				case 0x5922A2FF://nautiluspassivecheck
				{
					static float values[18] =
					{
						8,
							14,
							20,
							26,
							32,
							38,
							44,
							50,
							56,
							62,
							68,
							74,
							80,
							86,
							92,
							98,
							104,
							110
					};

					result.PhysicalDamage += values[std::min(source->Level - 1, 17U)];
				}
				break;
				case 0x40DC9CE4: //wujustylesuperchargedvisual
				{
					auto spell = caster->Spellbook.GetSpell(SpellSlot_E);
					auto resource = spell->SpellData->Resource;
					auto spellLevel = spell->Level;
					auto damage = resource->eEffect3[spellLevel];//+ resource->fEffect2 * caster->TotalAttackDamage();

					result.TrueDamage += damage;
					break;
				}

				}
			}
#pragma endregion

#pragma region Target buffs
			for (auto buffManager : target->Buffs) {
				auto buff = buffManager.Buff;

				if (!buff->IsActive()) {
					continue;
				}

				auto caster = ObjectManager::Instance->ObjectsArray[buff->GetScriptInstance()->CasterId];

				if (!caster)
				{
					continue;
				}

				//GameClient::PrintChat("caster found #3", IM_COL32(144, 238, 144, 69));

				switch (buff->Script->Hash) {
				case 0xE0C2FFB4: // "LeonaSunlight"
					result.MagicalDamage += 18.0f + 7.0f * std::max(caster->Level, 18U);
					break;
				case 0xDF74BBED: // "braummarkcounter"
					result.MagicalDamage += 16.0f + 10.0f * std::max(caster->Level, 18U);
					break;
				case 0xBE5519A3: // "kalistacoopstrikemarkbuff"
					if (source->FindBuffHash(0x8A9646FD)) { // "kalistacoopstrikeally"
						//if (targetFlags & GameObjectFlags_AIMinionClient && target->IsLaneMinion() && target->Health < 125.0f) {
						if (target->Minion() && target->IsLaneMinion() && target->Health < 125.0f) {
							result.TrueDamage = target->Health;
						}
						auto level = caster->Spellbook.GetSpell(SpellSlot_W)->Level;
						auto damage = (2.5f + 2.5f * level) * target->MaxHealth;
						static float values[5] = { 75.0f, 125.0f, 150.0f, 175.0f, 200.0f };
						//result.MagicalDamage += (targetFlags & GameObjectFlags_AIHeroClient) ? damage : std::min(damage, values[level - 1]);
						result.MagicalDamage += (target->Hero()) ? damage : std::min(damage, values[level - 1]);
					}
					break;
				case 0x4BAEFCCC: { // "vaynesilvereddebuff"
					if (buff->GetCount() == 2) {
						auto spell = caster->Spellbook.GetSpell(SpellSlot_W);
						auto resource = spell->SpellData->Resource;
						auto spellLevel = spell->Level;
						auto damage = std::max(target->MaxHealth * resource->eEffect1[spellLevel], resource->eEffect2[spellLevel]);

						if (target->IsMonster()) {
							damage = std::min(damage, resource->eEffect3[spellLevel]);
						}

						result.TrueDamage += damage;

						//GameClient::PrintChat(("damage" + (std::to_string(std::max(target->MaxHealth * resource->eEffect1[spellLevel], resource->eEffect2[spellLevel])))).c_str(), IM_COL32(255, 69, 255, 255));
						break;
					}
				}
				}
			}
#pragma endregion

			return result;

		}
	}
}