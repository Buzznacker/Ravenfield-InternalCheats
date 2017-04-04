using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace GameHack.Module.Modules
{
	public class UnlimitedAmmo : Module
	{
		public UnlimitedAmmo(string name, KeyCode toggleKey) : base(name, toggleKey) {}

		public override void onRender()
		{
			am.player.activeWeapon.ammo = 500;
			am.player.UpdateAmmoUi();
		}
	}
}
