using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace GameHack.Module.Modules
{
	public class GodMode : Module
	{
		public GodMode(string name, KeyCode toggleKey) : base(name, toggleKey) {}

		public override void onRender()
		{
			am.player.health = 10000;
			am.player.UpdateHealthUi();
		}

		public override void onDisable()
		{
			am.player.health = 100;
			am.player.UpdateHealthUi();
		}
	}
}
